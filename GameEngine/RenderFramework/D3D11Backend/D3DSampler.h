#pragma once

#include "D3D11Context.h"
namespace ge
{

    struct D3DSampler : public ISampler {
        SAMPLER_DESC            m_desc;
        D3DContext*             m_context;
        ID3D11SamplerState*     m_state;

        constexpr ID3D11SamplerState* object() const {
            return m_state;
        }

        D3DSampler(D3DContext* ctx, const SAMPLER_DESC& desc);

        ~D3DSampler();

        virtual const SAMPLER_DESC& getDesc() const override;

    };
}