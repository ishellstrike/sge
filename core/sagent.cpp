#include "sagent.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

SAgent::SAgent()
{

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
   if(raw_q)
   {
      raw_q = false;

      const auto &yawq = glm::angleAxis(rotation.x, glm::vec3(0,1,0));
      const auto &pitchq = glm::angleAxis(rotation.y, glm::vec3(1,0,0));
      const auto &rollq = glm::angleAxis(rotation.z, glm::vec3(0,0,1));

      precomp_quat = rollq * pitchq * yawq;
      precomp_quat = glm::normalize(precomp_quat);
   }

   if(raw)
   {
      raw = false;

      precomp = glm::translate(glm::scale(glm::mat4(1), scale) * glm::mat4_cast(precomp_quat), position);
   }

   return precomp;
}
