/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "umesh.h"
#include "logger.h"



void UMeshDc::resetDc()
{
    last_dc = dc;
    dc = 0;
}

int UMeshDc::getDc()
{
    return last_dc;
}

int UMeshDc::dc = 0;
int UMeshDc::last_dc = 0;
