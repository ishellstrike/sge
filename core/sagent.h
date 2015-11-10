#ifndef SAGENT_H
#define SAGENT_H
#include "uniqueid.h"
#include <glm/glm.hpp>

class SAgent
{
public:
   SAgent();

   inline virtual uniqId get_id() = 0;
};

class SMeshAgent : public SAgent
{
public:
   inline uniqId get_id();
};

class SPositionAgent : public SAgent
{
public:
   inline get_id();

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
   glm::quat rotation = glm::quat(0);
   glm::vec3 scale = glm::vec3(0);

   glm::mat4 precomp = glm::mat4(1);
   bool raw = false;
};

#endif // SAGENT_H
