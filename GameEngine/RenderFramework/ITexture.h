#pragma once

#include "IContextObject.h"
#include "UnionTypes.h"

namespace ge
{
    enum TextureUsage {
        TU_COLORATTACHNENT = 1 << 0,
        TU_SAMPLED = 1 << 1,
        TU_DEPTHATTACHMENT = 1 << 2
    };

    typedef uint32 TextureUsageBits;

    struct TEXTURE_DESC {
        uint32 width = 1;
        uint32 heigh = 1;
        TextureFormat format = TF_NONE;
        TextureUsageBits usage = TU_SAMPLED;
        uint32 mips = 1;
        uint32 layers = 1;
    };

    struct TextureExtend {
        uint32 w, h, l;
        uint32 x, y, z;
    };

    class ITexture : public IContextObject {
    public:
        virtual const TEXTURE_DESC& getDesc() const = 0;
        virtual void setData(void* data, size_t pitch, const TextureExtend& extend, uint32 mip, uint32 level) = 0;
    };

    typedef CmPtr<ITexture> ITextureRef;
}