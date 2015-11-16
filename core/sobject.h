#ifndef SOBJECT_H
#define SOBJECT_H
#include "sagent_base.h"
#include "uniqueid.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

class SObject : public SAgentBase
{
public:
   SObject();

   template<typename _Ty>
   std::shared_ptr<_Ty> Get()
   {
      auto a = agents.find(UniqueId::getTypeId<_Ty>());

      if(a != std::end(agents))
         return std::static_pointer_cast<_Ty>(a->second);
      else
         return nullptr;
   }

   template<typename _Ty>
   void Add(_Ty *val)
   {
      agents.insert(std::make_pair(val->Type(), std::shared_ptr<SAgentBase>(val)));
   }

   template<typename _Ty>
   void Add(std::shared_ptr<_Ty> val)
   {
      agents.insert(std::make_pair(val->Type(), val));
   }

   std::string Name;

private:
   typedef std::unordered_multimap<uniqId, std::shared_ptr<SAgentBase>> agent_map;
   agent_map agents;
};

#endif // SOBJECT_H
