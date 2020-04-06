#pragma once

#include "IContextObject.h"

namespace ge
{

    enum BufferType {

        /*only updateResource
        *device local memory
        */
        BT_STATIC,

        /*map(write) or updateResource,
        * host visable memory
        */
        BT_DYNAMIC,

        /*map(read | write)
        * host visable memory
        * called sync point if read map
        */
        BT_STREAM
    };

    enum BufferUsage {
        BU_UNIFORM = 1 << 0,
        BU_STORAGE_BUFFER = 1 << 1,
        BU_VERTEX = 1 << 2,
        BU_INDECES = 1 << 3
    };

    typedef uint32 BufferUsageBits;

    struct BUFFER_DESC {
        uint32 size = 256;
        BufferType type = BT_DYNAMIC;
        BufferUsageBits usage = BU_UNIFORM;
    };

    enum MapType {
        /*
        * only write data
        */
        MT_WRITE,
        /*
        * only read data
        */
        MT_READ,
        //
        MT_READ_WRITE
    };

    class IBuffer : public IContextObject {
    public:
        virtual const BUFFER_DESC& getDesc() const = 0;
        virtual void* map(MapType type) = 0;
        virtual void unmap() = 0;
        virtual void updateData(const void* data, uint32 offset, uint32 length) = 0;
    };

    typedef CmPtr<IBuffer> IBufferRef;
}