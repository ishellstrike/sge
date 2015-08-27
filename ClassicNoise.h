/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef CLASSNOISE_H
#define CLASSNOISE_H
struct Noise {
    static float noise( float x, float y, float z );
    static float simplexnoise( float x, float y, float z, float w );
    static float simplexnoise( float xin, float yin, float zin );
    static float simplexnoise( float xin, float yin );
    static float normalized_simplexnoise( float xin, float yin );
};
#endif
