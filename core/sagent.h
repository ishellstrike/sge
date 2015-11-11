#ifndef SAGENT_H
#define SAGENT_H
#include "uniqueid.h"
#include <glm/glm.hpp>
#include <boost/any.hpp>

class SAgent
{
public:
   SAgent();
};

class SMeshAgent : public SAgent
{
public:
};

class SPositionAgent : public SAgent
{
public:

   glm::mat4 GetTransform();

   glm::vec3 getPosition() const;
   void setPosition(const glm::vec3 &value);

   glm::quat getRotation() const;
   void setRotation(const glm::quat &value);

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
};

#endif // SAGENT_H
