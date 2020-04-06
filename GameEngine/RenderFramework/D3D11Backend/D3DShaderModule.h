#pragma once

#include "D3D11Context.h"

//сука ебанутая хуйня
#undef max
#include "../ThirdParty/spirv_hlsl.hpp"

namespace ge
{
    class D3DShaderModule : public IShaderModule {
        SHADER_MODULE_DESC m_desc;
        IContext* m_context;
        spirv_cross::CompilerHLSL m_parsed;
    public:

        D3DShaderModule(IContext* ctx, const SHADER_MODULE_DESC& desc);
        std::vector<uint8_t> getByteCode(ShaderType type);
        virtual const SHADER_MODULE_DESC& getDesc() const override;
    };
}