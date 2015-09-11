#ifndef MEMORYCHUNKCONTROLLER_H
#define MEMORYCHUNKCONTROLLER_H
#include "core/uniqueid.h"
#include <tuple>
#include <list>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>

class MemoryChunk
{
public:
     MemoryChunk();
     MemoryChunk(size_t _size, void* _data, uniqId _id);
     size_t size;
     size_t offset;
     void* data;
     uniqId id;

     struct ByOffset {}; struct ById {};
};

using namespace boost;
using namespace boost::multi_index;

class MemoryChunkController
{
public:
    typedef multi_index_container<MemoryChunk,
        indexed_by<
            ordered_unique<
                tag<MemoryChunk::ById>, member<MemoryChunk, uniqId, &MemoryChunk::id>
            >,
            ordered_non_unique<
                tag<MemoryChunk::ByOffset>, member<MemoryChunk, size_t, &MemoryChunk::offset>
            >
        >
    > OffsetIdOrdered;


    MemoryChunkController(size_t _s);
    size_t size;
    std::list<MemoryChunk> to_load;
    OffsetIdOrdered loaded;

    void LoadOne();
    MemoryChunk Store(void* chunk, size_t size);
    bool Unstore(uniqId id);
};

#endif // MEMORYCHUNKCONTROLLER_H
