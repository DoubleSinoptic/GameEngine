#include "D3DShaderModule.h"
#include <d3dcompiler.h>
namespace ge
{

    D3DShaderModule::D3DShaderModule(IContext* ctx, const SHADER_MODULE_DESC& desc) :
        m_desc(desc),
        m_context(ctx),
        m_parsed(reinterpret_cast<const uint32_t*>(m_desc.code.data()), m_desc.code.size() / 4)
    {
        spirv_cross::CompilerHLSL::Options opts = {};
        opts.shader_model = 50;
        m_parsed.set_hlsl_options(opts);
    }

    std::vector<uint8_t> D3DShaderModule::getByteCode(ShaderType type)
    {
        if (type == ST_VERTEX) {
      
            for (int i = 0; i < 11; i++) {
                spirv_cross::HLSLVertexAttributeRemap remap = {};
                remap.location = i;
                remap.semantic = "POSITION" + std::to_string(i);
                m_parsed.add_vertex_attribute_remap(remap);
            }
        
        }
  
        std::string code = m_parsed.compile();

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

        LPCSTR profile;
        switch (type)
        {
        case ST_COMPUTE:
            profile = "cs_5_0";
            break;
        case ST_VERTEX:
            profile = "vs_5_0";
            break;
        case ST_FRAGMENT:
            profile = "ps_5_0";
            break;
        case ST_GEOMETRY:
            profile = "gs_5_0";
            break;
        default:
            throw std::runtime_error("invalid shader profile.");
            break;
        }

        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        const Utf8String csstring = to_utf8(m_desc.entryPoint);
        assert(csstring == "main");
        printf("%s", code.c_str());
        TR_HRESULT(D3DCompile2(code.data(), code.size(), nullptr, nullptr, nullptr, csstring.c_str(), profile, 0, 0, 0, nullptr, 0, &shaderBlob, &errorBlob));

        std::vector<uint8_t> data(shaderBlob->GetBufferSize());
        memcpy(data.data(), shaderBlob->GetBufferPointer(), data.size());
        shaderBlob->Release();
        if (errorBlob)
         errorBlob->Release();

        return data;
    }

    const SHADER_MODULE_DESC& D3DShaderModule::getDesc() const
    {
        return m_desc;
    }
}