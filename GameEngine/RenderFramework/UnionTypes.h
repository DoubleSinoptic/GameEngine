#pragma once

#include "../Core/Core.h"
#include "../Core/SmallVector.h"

namespace ge
{
    enum TextureFormat 
    {
        TF_NONE = 0,
        TF_R8G8B8A8_UNORM,
        TF_R8G8B8_UNORM,

        TF_R32G32B32A32_SINT,
        TF_R16G16B16A16_SINT,
        TF_R32G32B32A32_FLOAT,
        TF_R16G16B16A16_FLOAT,

        TF_R32G32B32_FLOAT,
        TF_R32G32_FLOAT,
        TF_R32_FLOAT,

        TF_R32G32B32_SINT,
        TF_R32G32_SINT,
        TF_R32_SINT,

        TF_R16G16_SINT,
        TF_R16_SINT,

        TF_D16_FLOAT,
        TF_D32_FLOAT,
        TF_D32,
        TF_D24,
        TF_D16,
        TF_D8
    };

    enum ResourceType {
        RT_NONE = 0,
        RT_BUFFER,
        RT_STORAGE_BUFFER,
        RT_SAMPLER,
        RT_TEXTURE
    };

    enum ParamStage
    {
        PS_VERTEX = 1 << 0,
        PS_FRAGMENT = 1 << 1,
        PS_COMPUTE = 1 << 2
    };

    typedef uint32 ParamStageBits;

    struct PARAM_DECL {
        ParamStageBits   stages = PS_VERTEX | PS_FRAGMENT;
        ResourceType     type = RT_NONE;
    };

}