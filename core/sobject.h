#ifndef SOBJECT_H
#define SOBJECT_H
#include "sagent.h"
#include "uniqueid.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

class SObject : public SAgent
{
public:
   SObject();

   template<typename _Ty>
   std::shared_ptr<SAgent> Get()
   {
      auto a = std::find(std::begin(agents), std::end(agents), &typeid(SAgent));
      if(a != std::end(agents))
         return *a;
      else
         return nullptr;
   }

   template<typename _Ty>
   void Add(std::shared_ptr<SAgent> val)
   {
      agents.insert(&typeid(SAgent), val);
   }

private:
   std::multimap<uniqId, std::shared_ptr<SAgent>> agents;
};

#endif // SOBJECT_H
