#ifndef SPACE_OBJECT_H
#define SPACE_OBJECT_H
#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include "spacesystem.h"
#include "gametimer.h"
#include "space_solver.h"

class SpaceObject
{
public:
    SpaceObject();
    SpaceObject(float __mass, float __ro, glm::vec3 p0 = glm::vec3(0));

    glm::dvec3 pos = glm::dvec3(0), speed = glm::dvec3(0), acc = glm::dvec3(0);
    std::vector<glm::dvec3> hist;
    float last = 0;
    float moving = 0;

    glm::vec4 color;
    const int max_h = 1000;
    int cur_h = 0;

    double ro() const; /*< density*/
    void ro(double __ro);

    double V() const; /*< volume (indirect)*/

    double R() const; /*< radius (indirect)*/

    double mass() const; /*< mass*/
    void mass(double __mass);

    double fx(double local_x, SpaceSystem &syst);
    double fy(double local_y, SpaceSystem &syst);
    double fz(double local_y, SpaceSystem &syst);
    void calcX(SpaceSystem &syst);
    void calcY(SpaceSystem &syst);
    void calcZ(SpaceSystem &syst);
    const double T = 0.0000001;

    std::string GetDebugInfo();

    void Update(SpaceSystem &syst, GameTimer &gt);
private:
    double m_mass = 1.0;
    double m_R = 1.0;
    double m_ro = 1.0;
    double m_V = 1.0;
    void BuildVR();
};

#endif // SPACE_OBJECT_H
