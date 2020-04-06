#pragma once

#include "UnionTypes.h"
#include "IContextObject.h"
#include "IBuffer.h"
#include "ISampler.h"
#include "IPipeline.h"
#include "ITexture.h"
#include "IShaderModule.h"
#include "IFrameBuffer.h"
#include "IResourceSet.h"
#include "ICommandBuffer.h"

namespace ge
{

    class IContext : public IContextObject {
    public:
        virtual ICommandBufferRef createCommandbuffer(const COMMAND_BUFFER_DESC& desc) = 0;
        virtual IBufferRef createBuffer(const BUFFER_DESC& desc) = 0;
        virtual ITextureRef createTexture(const TEXTURE_DESC& desc) = 0;
        virtual IPipelineRef createPipeline(const PIPELINE_DESC& desc) = 0;
        virtual IFramebufferRef createFramebuffer(const FRAMEBUFFER_DESC& desc) = 0;
        virtual IShaderModuleRef createShaderModule(const SHADER_MODULE_DESC& desc) = 0;
        virtual ISamplerRef createSampler(const SAMPLER_DESC& desc) = 0;
        virtual IResourceSetRef createResourceSet(const RESOURCESET_DESC& desc) = 0;

  
        virtual void resizeDrawable(uint32 w, uint32 h) = 0;
        virtual CmPtr<ITexture> nextDrawable() = 0;
        virtual void present(ITexture* image) = 0;

        virtual void submit(ICommandBuffer* cmdBuffer) = 0;
        virtual ICommandBufferRef mainCommandBuffer() = 0;
    };

    typedef CmPtr<IContext> IContextRef;
}