#pragma once

#include "D3D11Context.h"
namespace ge
{

    struct D3DBuffer : public IBuffer {
        BUFFER_DESC             m_desc;
        D3DContext* m_context;
        ID3D11DeviceContext* m_dx11context;
        ID3D11Buffer* m_buffer;

        constexpr ID3D11Buffer* object() const {
            return m_buffer;
        }

        D3DBuffer(D3DContext* ctx, const BUFFER_DESC& desc);

        virtual const BUFFER_DESC& getDesc() const override;
        virtual void* map(MapType type) override;
        virtual void unmap() override;
        virtual void updateData(const void* data, uint32 offset, uint32 length) override;
        ~D3DBuffer();
    };

}