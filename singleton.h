#ifndef SINGLETON
#define SINGLETON
#include <mutex>

class Singleton
{
protected:
    static Singleton* m_instance;
    Singleton() {}
    virtual ~Singleton() {}

public:
    static Singleton* Instance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();

            if (!m_instance)
                m_instance = new Singleton();

            mutex.unlock();
        }
        return m_instance;
    }

    static DeleteInstance()
    {
        static std::mutex mutex;
        if(!m_instance)
        {
            mutex.lock();
            if(!m_instance)
            {
                m_instance = nullptr;
                delete m_instance;
            }
            mutex.unlock();
        }
    }
};

Singleton *Singleton ::m_instance = nullptr;

#endif // SINGLETON

