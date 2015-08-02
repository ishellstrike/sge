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

    template<typename _Ty = double>
    _Ty G()
    {
        return _Ty( 6.674083131313131313131e-11 );
    }

    template<typename _Ty = double>
    _Ty v_1( const SpaceObject &o )
    {
        //v_1=\sqrt{G\frac{M}{R}}
        return sqrt( G<_Ty>() * ( o.mass<_Ty>() / o.R<_Ty>() ) );
    }

    template<typename _Ty = double>
    _Ty v_2( const SpaceObject &o )
    {
        //v_2=\sqrt{2G\frac{M}{R}}
        return sqrt( 2 * G<_Ty>() * ( o.mass<_Ty>() / o.R<_Ty>() ) );
    }

    template<typename _Ty = double>
    _Ty v_3( const SpaceObject &o )
    {
        //v_3=\sqrt{(\sqrt{2}-1)^2 v^2+v_2^2}
        return sqrt( pow((sqrt(2) - 1), 2) * pow(v_1<_Ty>(o), 2) + pow(v_2<_Ty>(o), 2));
    }

    template<typename _Ty = double>
    _Ty randomize_orbital( const SpaceObject &o )
    {
        _Ty r  = random::next<_Ty>( );
        _Ty v1 =          v_1<_Ty>(o);
        _Ty v2 =          v_2<_Ty>(o);

        return lerp( v1, v2, r );
    }

    template<typename _Ty = double>
    glm::tvec3<_Ty, glm::defaultp> make_orbital_vector(const SpaceObject &cen, const SpaceObject &sat, _Ty speed)
    {
        typedef glm::tvec3<_Ty, glm::defaultp> vec;

        vec set_to_cen = sat.pos - cen.pos;
        vec perp = glm::rotate<_Ty, glm::defaultp>(set_to_cen, glm::half_pi<_Ty>(), vec(0,1,0));
        return perp * speed;
    }

    template<typename _Ty>
    glm::tvec3<_Ty, glm::defaultp> sphere_surface(_Ty R)
    {
        typedef glm::tvec3<_Ty, glm::defaultp> vec;

        _Ty A = glm::two_pi<_Ty>() * random::next<_Ty>();
        _Ty gamma = glm::two_pi<_Ty>() * random::next<_Ty>();

        _Ty x = R * cos(gamma) * cos(A);
        _Ty y = R * cos(gamma) * sin(A);
        _Ty z = R * sin(gamma);

        return vec(x, y, z);
    }
}

#undef sqrt
#undef rsqt
#undef distance
#undef normalize

#endif // SPACE_SOLVER_H

