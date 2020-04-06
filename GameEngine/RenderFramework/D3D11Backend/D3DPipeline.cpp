#include "D3DPipeline.h"
#include "D3DShaderModule.h"
namespace ge
{

    D3DPipeline::D3DPipeline(IContext* ctx, const PIPELINE_DESC& desc) :
        m_desc(desc), m_context(static_cast<D3DContext*>(ctx)),
        m_depthStencilState(nullptr),
        m_inputLayout(nullptr),
        m_rasterState(nullptr),
        m_vertexShader(nullptr),
        m_pixelShader(nullptr),
        m_geometryShader(nullptr)
    {
        assert(m_desc.vertexShader != nullptr);
        std::vector<uint8_t> vByteCode = ((D3DShaderModule*)(IShaderModule*)m_desc.vertexShader)->getByteCode(ST_VERTEX);
        TR_HRESULT(m_context->m_device->CreateVertexShader(vByteCode.data(), vByteCode.size(), nullptr, &m_vertexShader));
        if (m_desc.fragmentShader) {
            std::vector<uint8_t> vByteCodeX = ((D3DShaderModule*)(IShaderModule*)m_desc.fragmentShader)->getByteCode(ST_FRAGMENT);
            TR_HRESULT(m_context->m_device->CreatePixelShader(vByteCodeX.data(), vByteCodeX.size(), nullptr, &m_pixelShader));
        }

        if (m_desc.geometryShader) {
            std::vector<uint8_t> vByteCodeY = ((D3DShaderModule*)(IShaderModule*)m_desc.geometryShader)->getByteCode(ST_GEOMETRY);
            TR_HRESULT(m_context->m_device->CreateGeometryShader(vByteCodeY.data(), vByteCodeY.size(), nullptr, &m_geometryShader));
        }


        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;

        for (int i = 0; i < desc.attributes.size(); i++) {
            const auto& extractedAttribute = desc.attributes[i];
            D3D11_INPUT_ELEMENT_DESC elementDesc = {};
            elementDesc.InputSlot = extractedAttribute.binding;
            elementDesc.SemanticIndex = extractedAttribute.location;
            elementDesc.InstanceDataStepRate = extractedAttribute.perInstanceRate ? 1 : 0;
            const auto vertexFormat = D3DContext::dxgiFormats.find(extractedAttribute.format);
            if (vertexFormat == end(D3DContext::dxgiFormats)) {
                throw std::runtime_error("error the pizdec");
            }
            elementDesc.Format = (DXGI_FORMAT)vertexFormat->second;
            elementDesc.InputSlotClass = !extractedAttribute.perInstanceRate ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
            elementDesc.AlignedByteOffset = extractedAttribute.offset;
            elementDesc.SemanticName = "POSITION";// semanticsIndex[extractedAttribute.location];
            m_vertexOffsets.push_back(extractedAttribute.stride);
            inputElementDescs.push_back(elementDesc);
        }
        auto res = m_context->m_device->CreateInputLayout(inputElementDescs.data(), inputElementDescs.size(), vByteCode.data(), vByteCode.size(), &m_inputLayout);
        TR_HRESULT(res);

        D3D11_RASTERIZER_DESC rasterDesc = {};
        rasterDesc.FrontCounterClockwise = desc.rasterState.frontFace == FF_CCW ? true : false;
        switch (desc.rasterState.cullFace)
        {
        case CF_FRONT:
            rasterDesc.CullMode = D3D11_CULL_FRONT;
            break;
        case CF_BACK:
            rasterDesc.CullMode = D3D11_CULL_BACK;
            break;
        case CF_NONE:
            rasterDesc.CullMode = D3D11_CULL_NONE;
            break;
        default:
            throw std::runtime_error("error the pizdec");
            break;
        }
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.DepthBias = 0.0f;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.ScissorEnable = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        rasterDesc.AntialiasedLineEnable = false;
    
        TR_HRESULT(m_context->m_device->CreateRasterizerState(&rasterDesc, &m_rasterState));

        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = desc.depthStencilState.depthTest;
        switch (desc.depthStencilState.depthOp)
        {
        case DO_G:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
            break;
        case DO_GEQUAL:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
            break;
        case DO_L:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
            break;
        case DO_LEQUAL:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        case DO_ALWAYS:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        case DO_NEVER:
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
            break;
        default:
            throw std::runtime_error("error the pizdec");
            break;
        }
        depthStencilDesc.DepthWriteMask = desc.depthStencilState.depthMask ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        depthStencilDesc.StencilEnable = false;

        TR_HRESULT(m_context->m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));

        D3D11_BLEND_DESC dblend = {};
        for (int i = 0; i < desc.blendStates.size(); i++) {
            auto& blend = dblend.RenderTarget[i];
            blend.DestBlend = D3DContext::getBlend(desc.blendStates[i].dst);
            blend.SrcBlend = D3DContext::getBlend(desc.blendStates[i].src);
            blend.BlendOp = D3DContext::getOp(desc.blendStates[i].op);
            blend.BlendOpAlpha = D3DContext::getOp(desc.blendStates[i].opAlpha);
            blend.DestBlendAlpha = D3DContext::getBlend(desc.blendStates[i].dstAlpha);
            blend.SrcBlendAlpha = D3DContext::getBlend(desc.blendStates[i].srcAlpha);
            blend.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            blend.BlendEnable = desc.blendStates[i].enabled;
        }
        dblend.AlphaToCoverageEnable = false;
        dblend.IndependentBlendEnable = false;

        TR_HRESULT(m_context->m_device->CreateBlendState(&dblend, &m_blendState));


    
    }

    void D3DPipeline::bind(ID3D11DeviceContext* context)
    {
        ID3D11DeviceContext* ctx = context;
        const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        const UINT sampleMask = 0xffffffff;
        ctx->OMSetBlendState(m_blendState, blendFactor, sampleMask);
        ctx->OMSetDepthStencilState(m_depthStencilState, 0);
        ctx->IASetInputLayout(m_inputLayout);
        ctx->RSSetState(m_rasterState);
        ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        ctx->VSSetShader(m_vertexShader, nullptr, 0);
        if (m_pixelShader)
            ctx->PSSetShader(m_pixelShader, nullptr, 0);
        else
            ctx->PSSetShader(nullptr, nullptr, 0);

      if (m_geometryShader)
            ctx->GSSetShader(m_geometryShader, nullptr, 0);
        else
            ctx->GSSetShader(nullptr, nullptr, 0);
    }

    D3DPipeline::~D3DPipeline() {
        RELEASE_COM(m_depthStencilState);
        //for (auto x : m_blendStates)
        //    RELEASE_COM(x);
        RELEASE_COM(m_blendState);
        RELEASE_COM(m_inputLayout);
        RELEASE_COM(m_rasterState);
        RELEASE_COM(m_vertexShader);
        RELEASE_COM(m_pixelShader);
        RELEASE_COM(m_geometryShader);
    }

    const PIPELINE_DESC& D3DPipeline::getDesc() const
    {
        return m_desc;
    }
}