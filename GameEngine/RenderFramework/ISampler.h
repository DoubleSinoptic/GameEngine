#pragma once

#include "IContextObject.h"

namespace ge
{
    enum SamplerFilter {
        SF_LINEAR,
        SF_NEAREST
    };

    enum SamplerAddress {
        SA_WRAP = 1,
        SA_MIRROR = 2,
        SA_CLAMP = 3,
        SA_BORDER = 4,
        SA_MIRROR_ONCE = 5
    };

    struct SAMPLER_DESC {
        SamplerFilter mag = SF_NEAREST;
        SamplerFilter min = SF_NEAREST;
        SamplerFilter mip = SF_NEAREST;
        SamplerAddress wrapU = SA_WRAP;
        SamplerAddress wrapV = SA_WRAP;
        SamplerAddress wrapW = SA_WRAP;
        bool isNoramalizedCoords = true;
    };

    class ISampler : public IContextObject {
    public:
        virtual const SAMPLER_DESC& getDesc() const = 0;
    };

    typedef CmPtr<ISampler> ISamplerRef;
}