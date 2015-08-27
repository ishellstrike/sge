/*******************************************************************************
        Copyright (C) 2015 Samsonov Andrey

        This software is distributed freely under the terms of the MIT LICENSE.
        See "LICENSE.txt"
*******************************************************************************/

#ifndef SINGLETON
#define SINGLETON
#include <mutex>

template <typename Typename>
class Singleton
{
protected:
    static Typename *m_instance;
    Singleton() {}
    virtual ~Singleton() {}

public:
    static Typename* Instance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();

            if (!m_instance)
                m_instance = new Typename();

            mutex.unlock();
        }
        return (Typename *) m_instance;
    }

    static void DeleteInstance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();
            if(!m_instance)
            {
                m_instance = nullptr;
                delete ((Typename *)m_instance);
            }
            mutex.unlock();
        }
    }
};


#endif // SINGLETON

