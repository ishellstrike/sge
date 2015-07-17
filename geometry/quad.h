#ifndef QUAD_H
#define QUAD_H
#include "mesh.h"
#include <memory>

class Quad
{
public:
    /*!
     * \brief меш куба
     * \return меш
     *
     * Получение копии меша, содержащего геометрию куба 1х1х1 без нормалей, тангентов, бинормалей
     */
    static std::shared_ptr<Mesh> getMesh(float scale = 1.0f);
private:
    static std::shared_ptr<Mesh> sm_mesh;
};


#endif // QUAD_H
