#ifndef SPACE_SOLVER_H
#define SPACE_SOLVER_H
#include "space_object.h"
#include "helper.h"
#include <glm/gtx/rotate_vector.hpp>

#ifndef FAST_GLM
#define sqrt glm::sqrt
#define distance glm::distance
#define rsqrt glm::inverseSqrt
#define normalize glm::normalize
#else
#define sqrt glm::fastSqrt
#define distance glm::fastDistance
#define rsqrt glm::fastInverseSqrt
#define normalize glm::fastNormalize
#endif

namespace ssolver
{

    template<typename T = double>
    T G()
    {
        return T( 6.674083131313131313131e-11 );
    }

    template<typename T = double>
    T v_1( const SpaceObject &o )
    {
        //v_1=\sqrt{G\frac{M}{R}}
        return sqrt( G<T>() * ( o.mass() / o.R() ) );
    }

    template<typename T = double>
    T v_2( const SpaceObject &o )
    {
        //v_2=\sqrt{2G\frac{M}{R}}
        return sqrt( 2 * G<T>() * ( o.mass() / o.R() ) );
    }

    template<typename T = double>
    T v_3( const SpaceObject &o )
    {
        //v_3=\sqrt{(\sqrt{2}-1)^2 v^2+v_2^2}
        return sqrt( pow((sqrt(2) - 1), 2) * pow(v_1<T>(o), 2) + pow(v_2<T>(o), 2));
    }

    template<typename T = double>
    T randomize_orbital( const SpaceObject &o )
    {
        T r  = random::next<T>( );
        T v1 =          v_1<T>(o);
        T v2 =          v_2<T>(o);

        return lerp( v1, v2, r );
    }

    template<typename T = double>
    glm::tvec3<T, glm::defaultp> make_orbital_vector(const SpaceObject &cen, const SpaceObject &sat, T speed)
    {
        typedef glm::tvec3<T, glm::defaultp> vec;

        vec set_to_cen = cen.pos - sat.pos;
        vec perp = glm::rotate<T, glm::defaultp>(set_to_cen, glm::half_pi<T>(), vec(1,0,0));
        return perp * speed;
    }
}

#undef sqrt
#undef rsqt
#undef distance
#undef normalize

#endif // SPACE_SOLVER_H

