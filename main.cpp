/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#define GLM_FORCE_RADIANS
#define _USE_MATH_DEFINES
#define GLM_SWIZZLE
#define SOCI_USE_BOOST

#include "gamewindow.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "sql.h"

int main( int argc, char* const argv[] )
{
    if(argc >= 2)
        for(int i = 1; i < argc; i++)
        {
            if(strcmp(argv[i], "-v") == 0)
                Log::max_level = verbose;
            if(strcmp(argv[i], "-i") == 0)
                Log::max_level = info;
            if(strcmp(argv[i], "-e") == 0)
                Log::max_level = error;
            if(strcmp(argv[i], "-f") == 0)
                Log::max_level = fatal;
        }

    try {
        GameWindow gw;
        gw.Mainloop();
    }
    catch( const std::exception& e )
    {
        LOG(fatal) << e.what();
    }
    catch( ... )
    {
        LOG(fatal) << "unknown exception";
    }

    SQL s;

    void* v1 = new GameWindow();
    std::shared_ptr<void> v = std::shared_ptr<void>(v1);
    v.reset();

    return 0;
}
