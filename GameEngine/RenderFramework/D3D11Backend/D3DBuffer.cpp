#include "D3DBuffer.h"
namespace ge
{

    D3DBuffer::D3DBuffer(D3DContext* ctx, const BUFFER_DESC& desc) :
        m_desc(desc), m_context(ctx), m_buffer(nullptr)
    {
        D3D11_BUFFER_DESC dx11desc = {};

        if (desc.type == BT_DYNAMIC) {
            dx11desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            dx11desc.Usage = D3D11_USAGE_DYNAMIC;
        }
        else if (desc.type == BT_STREAM) {
            dx11desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
            dx11desc.Usage = D3D11_USAGE_DYNAMIC;
        }
        else if (desc.type == BT_STATIC) {
            dx11desc.CPUAccessFlags = 0;
            dx11desc.Usage = D3D11_USAGE_DEFAULT;
        }

        dx11desc.ByteWidth = (desc.size + 255) & ~255;
        dx11desc.BindFlags = ((desc.usage & BU_VERTEX) ? D3D11_BIND_VERTEX_BUFFER : 0) |
            ((desc.usage & BU_INDECES) ? D3D11_BIND_INDEX_BUFFER : 0) |
            ((desc.usage & BU_UNIFORM) ? D3D11_BIND_CONSTANT_BUFFER : 0);

        m_dx11context = m_context->m_context;
        TR_HRESULT(m_context->m_device->CreateBuffer(&dx11desc, NULL, &m_buffer));
    }

    const BUFFER_DESC& D3DBuffer::getDesc() const
    {
        return m_desc;
    }

    void* D3DBuffer::map(MapType type)
    {
        if (type == MT_WRITE) {
            D3D11_MAPPED_SUBRESOURCE res = {};
            TR_HRESULT(m_dx11context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res));
            return  res.pData;
        }
        if (type == MT_READ) {
            D3D11_MAPPED_SUBRESOURCE res = {};
            TR_HRESULT(m_dx11context->Map(m_buffer, 0, D3D11_MAP_READ, 0, &res));
            return  res.pData;
        }
        if (type == MT_READ_WRITE) {
            D3D11_MAPPED_SUBRESOURCE res = {};
            TR_HRESULT(m_dx11context->Map(m_buffer, 0, D3D11_MAP_READ_WRITE, 0, &res));
            return  res.pData;
        }
    }

    void D3DBuffer::unmap()
    {
        m_dx11context->Unmap(m_buffer, 0);
    }

    void D3DBuffer::updateData(const void* data, uint32 offset, uint32 length)
    {
        D3D11_BOX dstBox;
        dstBox.left = (UINT)offset;
        dstBox.right = (UINT)offset + length;
        dstBox.top = 0;
        dstBox.bottom = 1;
        dstBox.front = 0;
        dstBox.back = 1;
        m_dx11context->UpdateSubresource(m_buffer, 0, &dstBox, data, 0, 0);
    }

    D3DBuffer::~D3DBuffer() {
        RELEASE_COM(m_buffer);
    }
}