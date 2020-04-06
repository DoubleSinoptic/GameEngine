#include "D3DSampler.h"
namespace ge
{

    D3DSampler::D3DSampler(D3DContext* ctx, const SAMPLER_DESC& desc) :
        m_desc(desc), m_context(ctx), m_state(nullptr)
    {
        D3D11_SAMPLER_DESC d3d11desc = {};
        if (desc.min == SF_NEAREST && desc.mag == SF_LINEAR && desc.mip == SF_LINEAR) {
            d3d11desc.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
        }
        else if (desc.min == SF_LINEAR && desc.mag == SF_NEAREST && desc.mip == SF_LINEAR) {
            d3d11desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
        }
        else if (desc.min == SF_NEAREST && desc.mag == SF_NEAREST && desc.mip == SF_LINEAR) {
            d3d11desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        }
        else if (desc.min == SF_LINEAR && desc.mag == SF_LINEAR && desc.mip == SF_NEAREST) {
            d3d11desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        }
        else if (desc.min == SF_NEAREST && desc.mag == SF_LINEAR && desc.mip == SF_NEAREST) {
            d3d11desc.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        }
        else if (desc.min == SF_LINEAR && desc.mag == SF_NEAREST && desc.mip == SF_NEAREST) {
            d3d11desc.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        }
        else if (desc.min == SF_NEAREST && desc.mag == SF_NEAREST && desc.mip == SF_NEAREST) {
            d3d11desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else if (desc.min == SF_LINEAR && desc.mag == SF_LINEAR && desc.mip == SF_LINEAR) {
            d3d11desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        }

        d3d11desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)desc.wrapU;
        d3d11desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)desc.wrapV;
        d3d11desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)desc.wrapW;
        d3d11desc.MinLOD = -FLT_MAX;
        d3d11desc.MaxLOD = FLT_MAX;
        d3d11desc.MipLODBias = 0.0;
        d3d11desc.MaxAnisotropy = 1.0f;
        TR_HRESULT(m_context->m_device->CreateSamplerState(&d3d11desc, &m_state));
    }

    D3DSampler::~D3DSampler()
    {
        RELEASE_COM(m_state);
    }

    const SAMPLER_DESC& D3DSampler::getDesc() const
    {
        return m_desc;
    }
}