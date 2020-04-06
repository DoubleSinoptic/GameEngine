#pragma once

#include "IContextObject.h"
#include "UnionTypes.h"

namespace ge
{
    enum BlendOp
    {
        BO_ADD,
        BO_SUB,
        BO_DIV
    };

    enum BlendMod
    {
        BM_ZERO,
        BM_ONE,
        BM_SRC_ALPHA,
        BM_DST_ALPHA,
        BM_ONE_MINUS_SRC_ALPHA,
        BM_ONE_MINUS_DST_ALPHA
    };

    struct BLEND_STATE_DESC {
        BlendMod src = BM_ONE;
        BlendMod dst = BM_ZERO;
        BlendOp  op = BO_ADD;
        BlendMod srcAlpha = BM_ONE;
        BlendMod dstAlpha = BM_ZERO;
        BlendOp  opAlpha = BO_ADD;
        bool enabled = false;
    };

    enum DepthOp
    {
        DO_L,
        DO_LEQUAL,
        DO_G,
        DO_GEQUAL,
        DO_ALWAYS,
        DO_NEVER
    };

    struct DEPTH_STENCIL_STAGE_DESC {
        DepthOp					depthOp = DO_LEQUAL;
        scalar					znear = 0.0;
        scalar					zfar = 1.0;
        bool					depthTest = false;
        bool                    depthMask = false;
    };

    enum PrimitiveType {
        PT_TRIANGELS,
        PT_LINES
    };

    enum CullFace {
        CF_NONE,
        CF_BACK,
        CF_FRONT
    };

    enum FrontFace {
        FF_CCW,
        FF_CC
    };

    struct RASTER_DESC {
        PrimitiveType primitiveType = PT_TRIANGELS;
        CullFace cullFace = CF_BACK;
        FrontFace frontFace = FF_CCW;
    };

    struct VERTEX_INPUT_DESC
    {
        uint32	        location = 0;
        TextureFormat   format = TF_NONE;
        uint32		    offset = 0;
        uint32          binding = 0;
        uint32          stride = 0;
        bool            perInstanceRate = false;
    };

    class IShaderModule;

    struct PIPELINE_DESC
    {
        Vector<SmallVector<PARAM_DECL, 10>>             params;
        CmPtr<IShaderModule>                   vertexShader;
        CmPtr<IShaderModule>                   fragmentShader;
        CmPtr<IShaderModule>                   geometryShader;

        Vector<VERTEX_INPUT_DESC>              attributes;

        RASTER_DESC                            rasterState;
        SmallVector<BLEND_STATE_DESC, 8>               blendStates;
        DEPTH_STENCIL_STAGE_DESC               depthStencilState;
    };


    class IPipeline : public IContextObject {
    public:
        virtual const PIPELINE_DESC& getDesc() const = 0;
    };

    typedef CmPtr<IPipeline> IPipelineRef;
}