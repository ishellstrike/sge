#pragma once
#include "mesh.h"
#include <memory>
class Cube
{
public:
    /*!
     * \brief меш куба
     * \return меш
     *
     * Получение копии меша, содержащего геометрию куба 1х1х1 без нормалей, тангентов, бинормалей
     */
    static std::shared_ptr<Mesh> getMesh();
private:
    static std::shared_ptr<Mesh> sm_mesh;
};

