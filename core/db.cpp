#include "db.h"
#include "prefecences.h"
#include "objectstatic.h"
#include "boost/filesystem.hpp"
#include "agents/agents.hpp"

DB::DB()
{

}

std::shared_ptr<Object> DB::Create(const std::string &id)
{
    auto t = data.find(id);
    if(t == data.end())
    {
        LOG(error) << "id \"" << id << "\" not found in db";
        return data["air"]->Instantiate({0,0,0}, GameTimer());
    }
    return data[id]->Instantiate({0,0,0}, GameTimer());
}

const ObjectStatic *DB::Get(const std::string &id)
{
    auto t = data.find(id);
    if(t == data.end())
    {
        LOG(error) << "id \"" << id << "\" not found in db";
        return nullptr;
    }
    return data[id].get();
}

void DB::Load()
{  
    boost::filesystem::path targetDir(Prefecences::Instance()->getJsonDir());
    boost::filesystem::recursive_directory_iterator iter(targetDir);

    ObjectStatic *air = new ObjectStatic("air");
    air->agents = std::unique_ptr<AgentContainer>( new AgentContainer() );
    air->agents->push_back( std::make_shared<Walkable>() );
    data["air"] = std::unique_ptr<ObjectStatic>( air );

    int loaded = 0;
    for(const boost::filesystem::path &file : iter){
        if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".json")
        {
            std::ifstream fs(file.string());
            std::stringstream ss;
            std::string all;
            while(!fs.eof())
            {
                std::string buf;
                fs >> buf;
                ss << buf;
            }
            fs.close();
            all = ss.str();

            LOG(trace) << "---------------";
            LOG(trace) << "parse " << file;
            LOG(trace) << "---------------";
            rapidjson::Document d;
            d.Parse<0>(all.c_str());
            if(d.HasParseError())
            {
                LOG(error) << "while parsing " << file;
                LOG(error) << d.GetParseError();
                LOG(error) << all.substr(glm::max<int>(d.GetErrorOffset() - 20, 0), glm::min<int>(all.length(), 40));
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
                        LOG(trace) << "\"" << id << "\" parsing";

                        auto b = std::unique_ptr<ObjectStatic>(new ObjectStatic(id));
                        b->agents = std::unique_ptr<AgentContainer>(new AgentContainer());

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

                        if(val.HasMember("agents")) {
                            rapidjson::Value &arr = val["agents"];
                            if(val["agents"].IsArray())
                                for(decltype(arr.Size()) a = 0; a < arr.Size(); a++)
                                {
                                    rapidjson::Value &part = arr[a];
                                    if(part.HasMember("type")) {
                                        std::string agenttype = part["type"].GetString();
                                        auto c = AgentFactory::instance().Create(agenttype);
                                        if(!c)
                                        {
                                            LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown type = " << agenttype;
                                            continue;
                                        }
                                        try {
                                            c->Deserialize(part);
                                        } catch ( ... )
                                        {
                                            LOG(error) << id << " agent " << agenttype << " deserialize failed by unkown reason (probably wrong syntax). See agents documentation";
                                            continue;
                                        }

                                        if( agenttype == "Tags" )
                                        {
                                            for( const std::string &s : std::static_pointer_cast<Tags>( c )->tags)
                                            {
                                                LOG(trace) << "tagged as " << s;
                                                tags_ref[s].push_back( b.get() );
                                            }
                                        }

                                        if( agenttype == "ItemBase" )
                                        {
                                            b->PushAgent(AgentFactory::instance().Create("Stacked"));
                                        }

                                        if( agenttype == "CreatureBase" )
                                        {
                                            b->PushAgent(AgentFactory::instance().Create("Entity"));
                                        }

                                        b->PushAgent(c);
                                    }
                                    else
                                        LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";
                                }
                            else
                                LOG(error) << "record \"" << id << "\" parts is not valid agents array";
                        }

                        if(b->agents->empty())
                            b->agents.release();

                        data[id] = std::move(b);
                        loaded ++;
                    }
                }
            }
        }
    }
    LOG(info) << loaded << " loaded. Init all of them.";

    for( auto &a : data )
    {
        a.second->onLoad();
    }

    LOG(info) << "Done.";
}

std::unordered_map<Id, std::unique_ptr<ObjectStatic>> DB::data;
std::unordered_map<std::string, std::vector<ObjectStatic *>> DB::tags_ref;
std::unordered_map<SchemeType, std::vector<Scheme>> DB::scheme_db;
