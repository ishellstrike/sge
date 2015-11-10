#include "sagent.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

SAgent::SAgent()
{

}


uniqId SPositionAgent::get_id()
{
   return 1;
}

glm::vec3 SPositionAgent::getPosition() const
{
   return position;
}

void SPositionAgent::setPosition(const glm::vec3 &value)
{
   raw = true;
   position = value;
}
glm::quat SPositionAgent::getRotation() const
{
   return rotation;
}

void SPositionAgent::setRotation(const glm::quat &value)
{
   raw = true;
   rotation = value;
}
glm::vec3 SPositionAgent::getScale() const
{
   return scale;
}

void SPositionAgent::setScale(const glm::vec3 &value)
{
   raw = true;
   scale = value;
}

glm::mat4 SPositionAgent::getTransformation() const
{
   if(raw)
   {
      raw = false;
      precomp = glm::translate(glm::scale(glm::mat4(1), scale) * glm::mat4_cast(rotation), position);
   }
   return precomp;
}





uniqId SMeshAgent::get_id()
{
   return 2;
}
