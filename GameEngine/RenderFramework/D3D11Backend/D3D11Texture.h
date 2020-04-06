#pragma once
#include "D3D11Context.h"

namespace ge
{
    class D3DTexture : public ITexture {
        TEXTURE_DESC m_desc;
        D3DContext* m_context;
        ID3D11Texture2D* m_texture;
    public:
        constexpr ID3D11Texture2D* object() const {
            return m_texture;
        }

        constexpr ID3D11ShaderResourceView* baseView() const {
            return nullptr;
        }

        D3DTexture(D3DContext* ctx, ID3D11Texture2D* tx);

        D3DTexture(D3DContext* ctx, const TEXTURE_DESC& desc);

        ~D3DTexture();

        virtual const TEXTURE_DESC& getDesc() const override;

        virtual void setData(void* data, size_t pitch, const TextureExtend& extend, uint32 mip, uint32 level) override;
    };


}