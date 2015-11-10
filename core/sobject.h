#ifndef SOBJECT_H
#define SOBJECT_H
#include "sagent.h"
#include "uniqueid.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <map>

class SObject
{
public:
   SObject();

   template<typename _Ty>
   std::shared_ptr<SAgent> Get()
   {
      auto a = std::find(std::begin(agents), std::end(agents), UniqueId::getTypeId<_Ty>());
      if(a != std::end(agents))
         return *a;
      else
         return nullptr;
   }

private:
   std::map<uniqId, std::shared_ptr<SAgent>> agents;
};

#endif // SOBJECT_H
