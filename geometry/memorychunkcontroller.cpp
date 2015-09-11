#include "memorychunkcontroller.h"

MemoryChunkController::MemoryChunkController(size_t _s) : size(_s)
{

}

void MemoryChunkController::LoadOne()
{
    if(to_load.size() == 0)
        return;


}

MemoryChunk MemoryChunkController::Store(void *chunk, size_t size)
{
    MemoryChunk t;
    t.size = size;
    t.data = chunk;

    auto beg = loaded.get<1>().begin();
    auto next = beg;
    for(;;)
    {
        ++next;
        if(next == loaded.get<1>().end())
        {
            size_t endpoint = beg->offset + beg->size;
            if(endpoint >= size)
                return {0,0,0};
            t.offset = endpoint;

            loaded.insert(t);
            return t;
        }
    }
}

bool MemoryChunkController::Unstore(uniqId id)
{
    auto t = loaded.find(id);
    if(t == loaded.end())
        return false;

    loaded.erase(t);
    return true;
}



MemoryChunk::MemoryChunk()
{
    static uniqId uniq_id = UniqueId::next<MemoryChunk>();
    id = uniq_id;
    uniq_id++;
}

MemoryChunk::MemoryChunk(size_t _size, void* _data, uniqId _id) : size(_size), data(_data), id(_id)
{

}
