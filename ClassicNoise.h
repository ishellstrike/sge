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
