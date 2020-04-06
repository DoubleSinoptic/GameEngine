#include "D3D11CommandBuffer.h"
#include "D3D11Context.h"
#include "D3DBuffer.h"
#include "D3DSampler.h"
#include "D3DPipeline.h"
#include "D3D11Framebuffer.h"
#include "D3D11Texture.h"

namespace ge
{

    bool operator ==(const SmallVector<PARAM_DECL, 10>& a, const SmallVector<PARAM_DECL, 10>& b)
    {
        if (a.size() != b.size())
            return false;
        for (int i = 0; i < a.size(); i++) {
            if (memcmp(a.data() + i, b.data() + i, sizeof(PARAM_DECL))) {
                return false;
            }
        }
        return true;
    }

    void D3DMainCommandBuffer::setResources(IResourceSet* set, uint32 slot)
    {
        int start = slot * 10;
        const auto& setDesc = set->getDesc();
        assert(m_pipeline->m_desc.params[slot] == setDesc.params);
        for (int i = 0; i < setDesc.objects.size(); i++) {
            const auto& object = setDesc.objects[i];
            const auto objectType = setDesc.params[i].type;
            const auto stages = setDesc.params[i].stages;

            switch (objectType)
            {
            case RT_TEXTURE:
            {
                const D3DTexture* tex = (const D3DTexture*)(const IContextObject*)object;
                if (stages & ST_VERTEX) {
                    ID3D11ShaderResourceView* view = tex->baseView();
                    m_context->VSGetShaderResources(start + i, 1, &view);
                }
                if (stages & ST_FRAGMENT) {
                    ID3D11ShaderResourceView* view = tex->baseView();
                    m_context->PSGetShaderResources(start + i, 1, &view);
                }
            }
            break;
            case RT_BUFFER:
            {
                const D3DBuffer* buf = (const D3DBuffer*)(const IContextObject*)object;
                if (stages & ST_VERTEX) {
                    ID3D11Buffer* view = buf->object();
                    m_context->VSSetConstantBuffers(start + i, 1, &view);
                }
                if (stages & ST_FRAGMENT) {
                    ID3D11Buffer* view = buf->object();
                    m_context->PSSetConstantBuffers(start + i, 1, &view);
                }
            }
            break;
            case RT_SAMPLER:
            {
                const D3DSampler* tex = (const D3DSampler*)(const IContextObject*)object;
                if (stages & ST_VERTEX) {
                    ID3D11SamplerState* view = tex->object();
                    m_context->PSSetSamplers(start + i, 1, &view);
                }
                if (stages & ST_FRAGMENT) {
                    ID3D11SamplerState* view = tex->object();
                    m_context->PSSetSamplers(start + i, 1, &view);
                }
            }
            break;
            default:
                throw std::runtime_error("invalid resources");
                break;
            }
        }
    }

    void D3DMainCommandBuffer::setIndecesBuffer(IBuffer* buffer, uint32 offset)
    {
        ID3D11Buffer* d3dBuffer = static_cast<D3DBuffer*>(buffer)->object();
        const UINT d3doffset = offset;
        m_context->IASetIndexBuffer(d3dBuffer, DXGI_FORMAT_R32_UINT, d3doffset);
    }


    void D3DMainCommandBuffer::setRenderTarget(IFramebuffer* frm)
    {
        m_framebuffer = static_cast<D3DFramebuffer*>(frm);
        m_framebuffer->bind(m_context);
    }

    void D3DMainCommandBuffer::clear(const CLEAR_DESC& clearDesck)
    {
        m_context->ClearDepthStencilView(m_framebuffer->m_depthStencilView, 0, clearDesck.depth, clearDesck.stencil);
        for (int i = 0; i < m_framebuffer->m_targetView.size(); i++) {
            m_context->ClearRenderTargetView(m_framebuffer->m_targetView[i], clearDesck.clearValues[0].rgba);
        }
    }

    void D3DMainCommandBuffer::setVertexBuffer(IBuffer* buffer, uint32 slot, uint32 offset)
    {
        ID3D11Buffer* d3dBuffer = static_cast<D3DBuffer*>(buffer)->object();
        const UINT d3doffset = offset;
        const UINT stride = static_cast<D3DPipeline*>(m_pipeline)->m_vertexOffsets[slot];
        m_context->IASetVertexBuffers(slot, 1, &d3dBuffer, &stride, &d3doffset);
    }

    void D3DMainCommandBuffer::setPipeline(IPipeline* pipeline)
    {
        m_pipeline = static_cast<D3DPipeline*>(pipeline);
        m_pipeline->bind(m_context);
    }

    void D3DMainCommandBuffer::draw(uint32 verteceCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart)
    {
        m_context->DrawInstanced(verteceCount, instanceCount, vertexStart, instanceOffset);
    }

    void D3DMainCommandBuffer::drawIndexed(uint32 indexOffset, uint32 indexCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart)
    {
        m_context->DrawIndexedInstanced(indexCount, instanceCount, instanceOffset, vertexStart, instanceOffset);
    }


    void D3DMainCommandBuffer::setViewport(scalar x, scalar y, scalar w, scalar h)
    {
        D3D11_VIEWPORT port = {};
        port.Width = w;
        port.Height = h;
        port.TopLeftX = x;
        port.TopLeftY = y;
        port.MaxDepth = 1.0;
        port.MinDepth = 0.0f;
        m_context->RSSetViewports(1, &port);
    }


    void D3D11CommandBuffer::draw(uint32 verteceCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            c->draw(verteceCount, instanceOffset, instanceCount, vertexStart);
        });
    }
    void D3D11CommandBuffer::drawIndexed(uint32 indexOffset, uint32 indexCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            c->drawIndexed( indexOffset,  indexCount,  instanceOffset,  instanceCount, vertexStart);
        });
    }
    void D3D11CommandBuffer::setResources(IResourceSet* set, uint32 slot)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            IResourceSetRef ref = set;
            c->setResources(ref, slot);
        });
    }
    void D3D11CommandBuffer::setVertexBuffer(IBuffer* buffer, uint32 slot, uint32 offset)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            IBufferRef ref = buffer;
            c->setVertexBuffer(ref,  slot,  offset);
        });
    }
    void D3D11CommandBuffer::setIndecesBuffer(IBuffer* buffer, uint32 offset)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            IBufferRef ref = buffer;
            c->setIndecesBuffer(ref, offset);
        });
    }
    void D3D11CommandBuffer::setPipeline(IPipeline* pipeline)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            IPipelineRef ref = pipeline;
            c->setPipeline(ref);
         });
    }
    void D3D11CommandBuffer::setRenderTarget(IFramebuffer* frm)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            IFramebufferRef ref = frm;
            c->setRenderTarget(ref);
        });
    }
    void D3D11CommandBuffer::clear(const CLEAR_DESC& clearDesck)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            c->clear(clearDesck);
        });
    }
    void D3D11CommandBuffer::setViewport(scalar x, scalar y, scalar w, scalar h)
    {
        commands.push_back([=](D3DMainCommandBuffer* c) {
            c->setViewport(x, y, w, h);
        });
    }
}
