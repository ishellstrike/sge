#pragma once
#ifndef CLASSNOISE_H
#define CLASSNOISE_H
float noise( float x, float y, float z );
float simplexnoise( float x, float y, float z, float w );
float simplexnoise( float xin, float yin, float zin );
float simplexnoise( float xin, float yin );
float normalized_simplexnoise( float xin, float yin );
#endif
