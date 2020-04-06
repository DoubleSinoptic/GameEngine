#pragma once

#include "D3D11Context.h"
namespace ge
{
    struct D3DPipeline : public IPipeline {
        PIPELINE_DESC                           m_desc;
        D3DContext*                             m_context;

        ID3D11DepthStencilState*                m_depthStencilState;

        ID3D11InputLayout*                      m_inputLayout;
        ID3D11RasterizerState*                  m_rasterState;
        ID3D11BlendState*                       m_blendState;


        ID3D11VertexShader*                     m_vertexShader;
        ID3D11PixelShader*                      m_pixelShader;
        ID3D11GeometryShader*                   m_geometryShader;

        std::vector<UINT>                       m_vertexOffsets;

        D3DPipeline(IContext* ctx, const PIPELINE_DESC& desc);

        void bind(ID3D11DeviceContext* context);

        ~D3DPipeline();

        virtual const PIPELINE_DESC& getDesc() const override;
    };
}