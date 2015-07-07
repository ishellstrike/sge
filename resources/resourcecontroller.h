#ifndef RESOURCECONTROLLER_H
#define RESOURCECONTROLLER_H
#include <unordered_map>
#include "logger.h"
#include <mutex>

class Resources
{
public:
    static Resources *instance()
    {
        static std::mutex mutex;
        if(!m_inst)
        {
            mutex.lock();

            if (!m_inst)
                m_inst = new Resources();

            mutex.unlock();
            LOG(verbose) << "making resources controller instance";
        }
        return m_inst;
    }

    static void drop()
    {
        static std::mutex mutex;
        mutex.lock();
        if(m_inst)
        {
            delete m_inst;
            m_inst = nullptr;
        }
        mutex.unlock();
        LOG(verbose) << "droping resources controller instance";
    }



private:
    static Resources *m_inst;
    Resources(){}
    ~Resources(){}
    Resources(const Resources &root) = delete;
    Resources &operator=(const Resources &) = delete;
};

#endif // RESOURCECONTROLLER_H
