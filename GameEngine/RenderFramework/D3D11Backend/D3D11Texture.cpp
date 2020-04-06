#include "D3D11Texture.h"
#include "../../Core/Debug.h"

namespace ge 
{
    D3DTexture::D3DTexture(D3DContext* ctx, ID3D11Texture2D* tx) :
        m_desc(TEXTURE_DESC()), m_context(ctx), m_texture(tx)
    {
        if (m_texture)
            m_texture->AddRef();

        D3D11_TEXTURE2D_DESC desc = {};
        tx->GetDesc(&desc);

        m_desc.layers = 1;
        m_desc.mips = 1;
        m_desc.format = TF_R8G8B8A8_UNORM;
        m_desc.width = desc.Width;
        m_desc.heigh = desc.Height;
        Debug::log("TEXTURE: {0}", this);

    }

    D3DTexture::D3DTexture(D3DContext* ctx, const TEXTURE_DESC& desc) :
        m_desc(desc), m_context(ctx), m_texture(nullptr)
    {
        D3D11_TEXTURE2D_DESC d3d11desc = {};
        auto i = D3DContext::dxgiFormats.find((uint32_t)m_desc.format);
        if (i == end(D3DContext::dxgiFormats)) {
            throw std::runtime_error("the pizdec");
        }
        d3d11desc.Format = (DXGI_FORMAT)i->second;
        d3d11desc.ArraySize = desc.layers;

        d3d11desc.MipLevels = desc.mips;
        d3d11desc.Width = desc.width;
        d3d11desc.Height = desc.heigh;
        d3d11desc.Usage = D3D11_USAGE_DEFAULT;
        d3d11desc.CPUAccessFlags = 0;
        d3d11desc.BindFlags = ((desc.usage & TU_COLORATTACHNENT) ? D3D11_BIND_RENDER_TARGET : 0) |
            ((desc.usage & TU_DEPTHATTACHMENT) ? D3D11_BIND_DEPTH_STENCIL : 0) |
            ((desc.usage & TU_SAMPLED) ? D3D11_BIND_SHADER_RESOURCE : 0);
        d3d11desc.MiscFlags = desc.layers > 5 ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
        
        d3d11desc.SampleDesc.Count = 1;
        d3d11desc.SampleDesc.Quality = 0;
        auto result = m_context->m_device->CreateTexture2D(&d3d11desc, nullptr, &m_texture);
        TR_HRESULT(result);
        Debug::log("TEXTURE: {0}", this);
    }

    D3DTexture::~D3DTexture()
    {
        Debug::log("~TEXTURE: {0}", this);
        RELEASE_COM(m_texture);
    }

    const TEXTURE_DESC& D3DTexture::getDesc() const
    {
        return m_desc;
    }

    void D3DTexture::setData(void* data, size_t pitch, const TextureExtend& extend, uint32 mip, uint32 level)
    {
        D3D11_BOX destRegion;
        destRegion.left = extend.x;
        destRegion.right = extend.x + extend.w;
        destRegion.top = extend.y;
        destRegion.bottom = extend.y + extend.h;
        destRegion.front = 0;
        destRegion.back = 1;
        m_context->m_context->UpdateSubresource(m_texture, D3D11CalcSubresource(mip, level, m_desc.mips), &destRegion, data, pitch, 0);
    }
}
