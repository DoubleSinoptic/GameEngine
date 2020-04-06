#pragma once

#include "D3D11Context.h"

namespace ge
{
    class D3DMainCommandBuffer;
    struct D3D11CommandBuffer : public ICommandBuffer {
        std::vector<std::function<void(D3DMainCommandBuffer* main)>> commands;

        void excec(D3DMainCommandBuffer* main) {
            for (const auto& x : commands) {
                x(main);
            }
        }
        virtual void draw(uint32 verteceCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) override;
        virtual void drawIndexed(uint32 indexOffset, uint32 indexCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) override;
        virtual void setResources(IResourceSet* set, uint32 slot) override;
        virtual void setVertexBuffer(IBuffer* buffer, uint32 slot, uint32 offset) override;
        virtual void setIndecesBuffer(IBuffer* buffer, uint32 offset) override;
        virtual void setPipeline(IPipeline* pipeline) override;
        virtual void setRenderTarget(IFramebuffer* frm) override;

        virtual void clear(const CLEAR_DESC& clearDesck) override;
        virtual void setViewport(scalar x, scalar y, scalar w, scalar h) override;
    };

    struct D3DMainCommandBuffer : public ICommandBuffer {
        ID3D11DeviceContext* m_context;
        D3DFramebuffer*      m_framebuffer;
        D3DPipeline*         m_pipeline;

     

        virtual void draw(uint32 verteceCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) override;
        virtual void drawIndexed(uint32 indexOffset, uint32 indexCount, uint32 instanceOffset, uint32 instanceCount, uint32 vertexStart) override;
        virtual void setResources(IResourceSet* set, uint32 slot) override;
        virtual void setVertexBuffer(IBuffer* buffer, uint32 slot, uint32 offset) override;
        virtual void setIndecesBuffer(IBuffer* buffer, uint32 offset) override;
        virtual void setPipeline(IPipeline* pipeline) override;
        virtual void setRenderTarget(IFramebuffer* frm) override;
        virtual void clear(const CLEAR_DESC& clearDesck) override;
 
        virtual void setViewport(scalar x, scalar y, scalar w, scalar h) override;
    };
}