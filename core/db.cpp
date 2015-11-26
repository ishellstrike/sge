#include "db.h"
#include "agents.hpp"
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
        return ObjectStatic::air->Instantiate();
    }
    return data[id]->Instantiate();
}

void DB::Load()
{
   std::vector<std::string> files;
   getFiles(Prefecences::Instance()->getJsonDir(), files);

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
           LOG(error) << d.GetParseError();
           LOG(error) << all.substr(max(d.GetErrorOffset() - 20, 0), min(all.length(), 40));
           LOG(error) << "                    ^";
       }

       int loaded = 0;
       if(d.IsArray())
       {
           for(decltype(d.Size()) i=0; i < d.Size(); i++)
           {
               rapidjson::Value &val = d[i];
               if(!val.HasMember("id"))
               {
                   LOG(error) << "record #" << i+1 << " from " << file << " has no \"id\"";
                   continue;
               }
               std::string id = val["id"].GetString();

               ObjectStatic *b = new ObjectStatic(id);
               b->agents = std::unique_ptr<AgentContainer>(new AgentContainer());

               if(!val.HasMember("name"))
               {
                   LOG(error) << "record #" << i+1 << " from " << file << " has no \"id\"";
                   continue;
               }
               b->name = val["name"].GetString();

               if(val.HasMember("tex"))
                   b->tex = val["tex"].GetString();

               PARTS_PARSER

               if(b->agents->empty())
               {
                   b->agents.release();
                   b->is_static = true;
               }
               else
                   b->is_static = false;

               data[id] = std::unique_ptr<ObjectStatic>(b);
               loaded ++;
           }
       }
   }
   LOG(info) << loaded << " loaded";
}

std::unordered_map<std::string, std::unique_ptr<ObjectStatic>> DB::data;
