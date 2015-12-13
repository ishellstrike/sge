#include "db.h"
#include "sge_fielsystem.h"
#include "prefecences.h"
#include "objectstatic.h"

DB::DB()
{

}

std::unique_ptr<Object> DB::Create(const std::string &id)
{
    auto t = data.find(id);
    if(t == data.end())
    {
        LOG(error) << "id " << id << " not found in db";
        return data["air"]->Instantiate();
    }
    return data[id]->Instantiate();
}

const ObjectStatic *DB::Get(const std::string &id)
{
    auto t = data.find(id);
    if(t == data.end())
    {
        LOG(error) << "id " << id << " not found in db";
        return nullptr;
    }
    return data[id].get();
}

void DB::Load()
{
   std::vector<std::string> files;
   getFiles(Prefecences::Instance()->getJsonDir(), files);

   data["air"] = std::unique_ptr<ObjectStatic>( new ObjectStatic("air"));

   int loaded = 0;
   for(std::string file : files)
   {
       std::ifstream fs(Prefecences::Instance()->getJsonDir() + file);
       std::stringstream ss;
       std::string all;
       all.reserve(65536);
       while(!fs.eof())
       {
           std::string buf;
           fs >> buf;
           ss << buf;
       }
       fs.close();
       all = ss.str();

       LOG(verbose) << "parse " << file;
       rapidjson::Document d;
        d.Parse<0>(all.c_str());
       if(d.HasParseError())
       {
           LOG(error) << "while parsing " << file;
           LOG(error) << d.GetParseError();
           LOG(error) << all.substr(max(d.GetErrorOffset() - 20, 0), min(all.length(), 40));
           LOG(error) << "                    ^";
       }

       if(d.IsArray())
       {
           for(decltype(d.Size()) i=0; i < d.Size(); i++)
           {
               rapidjson::Value &val = d[i];

               std::string type;
               if(val.HasMember("type"))
                   type = val["type"].GetString();
               else
               {
                   LOG(error) << "record #" << i+1 << " from " << file << " has no \"type\"";
                   continue;
               }

               if(type == "scheme")
               {
                   Scheme s;
                   if(!s.Deserialize(val))
                   {
                       LOG(error) << "record #" << i+1 << " from " << file << " broken, skipped";
                       continue;
                   }

                   scheme_db[s.type].push_back(s);
                   loaded ++;
               }
               else
               {
                   if(!val.HasMember("id"))
                   {
                       LOG(error) << "record #" << i+1 << " from " << file << " has no \"id\"";
                       continue;
                   }
                   std::string id = val["id"].GetString();

                   auto b = std::unique_ptr<ObjectStatic>(new ObjectStatic(id));
                   b->agents = std::unique_ptr<AgentContainer>(new AgentContainer());

                   if(type == "item")
                       b->type = ObjectItem;
                   else if(type == "creature")
                       b->type = ObjectCreature;
                   else if(type == "creature")
                       b->type = ObjectBlock;
                   else
                       b->type = ObjectSpecial;

                   if(val.HasMember("name"))
                       b->name = val["name"].GetString();

                   if(val.HasMember("tex"))
                   {
                       rapidjson::Value &ar = val["tex"];
                       if(!ar.IsArray())
                       {
                           LOG(error) << "record #" << i+1 << " from " << file << " tex wrond format. Must be \"tex\":[\"tex1\",\"tex2\"]";
                           continue;
                       }

                       for(decltype(ar.Size()) k=0; k < ar.Size(); k++)
                           b->tex.push_back(ar[k].GetString());
                   }

                   if(val.HasMember("ground"))
                       b->ground = val["ground"].GetBool_();

                   if(val.HasMember("parts")) {
                       LOG(verbose) << "found parts";
                       rapidjson::Value &arr = val["parts"];
                       if(val["parts"].IsArray())
                       for(decltype(arr.Size()) a = 0; a < arr.Size(); a++)
                       {
                           rapidjson::Value &part = arr[a];
                           if(part.HasMember("type")) {
                               auto c = Agent::AgentFactory.Create(part["type"].GetString());
                               if(!c)
                               {
                                   LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown \"type\"";
                                   continue;
                               }
                               c->Deserialize(part);
                               b->PushAgent(std::move(c));
                               c->onInit(b.get());
                           }
                           else
                               LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";
                       }
                       else
                           LOG(error) << "record \"" << id << "\" parts is not valid agents array";
                   }

                   if(b->agents->empty())
                   {
                       b->agents.release();
                       b->is_static = true;
                   }
                   else
                       b->is_static = false;

                   data[id] = std::move(b);
                   loaded ++;
               }
           }
       }
   }
   LOG(info) << loaded << " loaded";
}

std::unordered_map<Id, std::unique_ptr<ObjectStatic>> DB::data;
std::unordered_map<SchemeType, std::vector<Scheme>> DB::scheme_db;
