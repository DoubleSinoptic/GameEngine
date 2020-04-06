#pragma once

#include "../IContext.h"
#include <memory>
#include <map>
#include <cassert>
#include <d3d11.h>
#include "../../Core/Any.h"

#define RELEASE_COM(x) if (x) { x->Release(); x = nullptr; }
#define TR_HRESULT(x) if (FAILED(x)) { throw std::runtime_error("ERROR"); }

namespace ge
{
    class D3DFramebuffer;
    class D3DPipeline;
    class D3DMainCommandBuffer;

    CmPtr<IContext> __cdecl CreateD3D11GraphicsContext(const Any& v);


    class D3DParamSet : public IResourceSet {
        RESOURCESET_DESC    m_desc;
        IContext*           m_context;
    public:
        D3DParamSet(IContext* ctx, const RESOURCESET_DESC& desc) :
            m_desc(desc), m_context(ctx)
        {}

        virtual const RESOURCESET_DESC& getDesc() const override
        {
            return m_desc;
        }
    };

    struct D3DContext : public IContext 
    {
        ID3D11DeviceContext* m_context;
        ID3D11Device*        m_device;
        IDXGISwapChain*      m_swapchain;
        CmPtr<D3DMainCommandBuffer> mainCommandBuffer1;
        std::map<ID3D11Texture2D*, ITextureRef> m_swapchainRefs;
        D3DContext(HWND wnd);
        ~D3DContext();

        virtual CmPtr<IBuffer> createBuffer(const BUFFER_DESC& desc) override;

        virtual CmPtr<ITexture> createTexture(const TEXTURE_DESC& desc) override;

        virtual CmPtr<IPipeline> createPipeline(const PIPELINE_DESC& desc) override;

        virtual CmPtr<IFramebuffer> createFramebuffer(const FRAMEBUFFER_DESC& desc) override;

        virtual CmPtr<IShaderModule> createShaderModule(const SHADER_MODULE_DESC& desc) override;

        virtual CmPtr<ISampler> createSampler(const SAMPLER_DESC& desc) override;

        virtual CmPtr<IResourceSet> createResourceSet(const RESOURCESET_DESC& desc) override;

        virtual CmPtr<ITexture> nextDrawable() override;
        virtual void present(ITexture* image) override;

        static D3D11_BLEND_OP getOp(BlendOp op);
        static D3D11_BLEND getBlend(BlendMod mod);
        static const std::map<uint32_t, uint32_t> dxgiFormats;

        // Унаследовано через IContext
        virtual void resizeDrawable(uint32 w, uint32 h) override;

        // Унаследовано через IContext
        virtual ICommandBufferRef createCommandbuffer(const COMMAND_BUFFER_DESC& desc) override;
        virtual void submit(ICommandBuffer* cmdBuffer) override;
        virtual ICommandBufferRef mainCommandBuffer() override;
    };
}