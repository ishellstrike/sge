#ifndef STRANSFORMAGENT_H
#define STRANSFORMAGENT_H
#include "core/sagent_base.h"
#include <glm/glm.hpp>

class STransformAgent : public SAgentBase
{
public:

   glm::mat4 GetTransform();

   glm::vec3 getPosition() const;
   void setPosition(const glm::vec3 &value);

   glm::vec3 getRotation() const;
   void setRotation(const glm::vec3 &value);

   glm::vec3 getScale() const;
   void setScale(const glm::vec3 &value);

   glm::mat4 getTransformation() const;

private:
   glm::vec3 position = glm::vec3(0);
   glm::vec3 rotation = glm::vec3(0); // Yaw Pitch Roll
   glm::vec3 scale = glm::vec3(0);

   mutable glm::mat4 precomp = glm::mat4(1);
   mutable glm::quat precomp_quat;
   mutable bool raw = false;
   mutable bool raw_q = false;

   // SAgentBase interface
public:
   AGENT(STransformAgent)
};

#endif // STRANSFORMAGENT_H
