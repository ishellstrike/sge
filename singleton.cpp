/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#include "singleton.h"

template<typename Typename>
Typename *Singleton<Typename>::m_instance = nullptr;
