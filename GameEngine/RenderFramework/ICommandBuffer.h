#pragma once

#include "IContextObject.h"
#include "UnionTypes.h"
namespace ge
{
    class IResourceSet;
    class IBuffer;
    class IFramebuffer;
    class IPipeline;

    union CLEAR_VALUE
    {
        scalar rgba[4];
        struct {
            scalar r, g, b, a;
        };
        struct {
            scalar x, y, z, w;
        };
    };

    enum ClearMask
    {
        CM_COLOR = 1 << 0,
        CM_DEPTH = 1 << 1,
        CM_STENCI = 1 << 2
    };

    typedef uint32 ClearMaskBits;

    struct CLEAR_DESC {
        ClearMaskBits mask;
        SmallVector<CLEAR_VALUE, 8> clearValues;
        scalar depth;
        uint32 stencil;
    };

    enum QueueIndex {
        /*
        * main queue
        */
        QI_GRAPHICS_TRANSFER_COMPUTE,

        QI_TRANSFER,
        QI_COMPUTE,
        QI_GRAPHICS,
        QI_GRAPHICS_TRANSFER
    };

    struct COMMAND_BUFFER_DESC {
        QueueIndex queueIndex;
    };

  /*  struct TEMP_TEXTURE_VIEW_DESC {
        uint32    mip;
        uint32    layer;
        ITexture* color;
    };

    struct TEMP_RENDER_TARGET_DESC {
        SmallVector<TEMP_TEXTURE_VIEW_DESC, 8> colors;
        TEMP_TEXTURE_VIEW_DESC  depth;
        TEMP_TEXTURE_VIEW_DESC  stencil;
    };*/

    class ICommandBuffer : public IContextObject {
    public:
        virtual void draw(uint32 verteceCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) = 0;
        virtual void drawIndexed(uint32 indexOffset, uint32 indexCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) = 0;

        virtual void setResources(IResourceSet* set, uint32 slot) = 0;
        virtual void setVertexBuffer(IBuffer* buffer, uint32 slot, uint32 offset) = 0;
        virtual void setIndecesBuffer(IBuffer* buffer, uint32 offset) = 0;
        virtual void setPipeline(IPipeline* pipeline) = 0;
        virtual void setRenderTarget(IFramebuffer* frm) = 0;
        virtual void clear(const CLEAR_DESC& clearDesck) = 0;
        virtual void setViewport(scalar x, scalar y, scalar w, scalar h) = 0;
    };

    typedef CmPtr<ICommandBuffer> ICommandBufferRef;
}