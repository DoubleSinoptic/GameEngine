#include "D3D11Context.h"
#include "D3DBuffer.h"
#include "D3DSampler.h"
#include "D3DShaderModule.h"
#include "D3DPipeline.h"
#include "D3D11Texture.h"
#include "D3D11Framebuffer.h"
#include "D3D11CommandBuffer.h"


#define TRANSLATE_FORMAT(x) { TF_ ## x , DXGI_FORMAT_ ## x}
namespace ge
{

    const std::map<uint32_t, uint32_t> D3DContext::dxgiFormats = {
        { TF_R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM },
        { TF_R32G32B32A32_SINT, DXGI_FORMAT_R32G32B32A32_SINT },
        { TF_R16G16B16A16_SINT, DXGI_FORMAT_R16G16B16A16_SINT },
        { TF_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT },
        { TF_R16G16B16A16_FLOAT,DXGI_FORMAT_R16G16B16A16_FLOAT },
        { TF_D32_FLOAT, DXGI_FORMAT_D32_FLOAT },
          { TF_D24, DXGI_FORMAT_D24_UNORM_S8_UINT },
        
        TRANSLATE_FORMAT(R32G32B32_FLOAT),
        TRANSLATE_FORMAT(R32G32_FLOAT),
        TRANSLATE_FORMAT(R32_FLOAT),
        TRANSLATE_FORMAT(R32G32B32_SINT),
        TRANSLATE_FORMAT(R32G32_SINT),
        TRANSLATE_FORMAT(R32_SINT),
        TRANSLATE_FORMAT(R16G16_SINT),
        TRANSLATE_FORMAT(R16_SINT)
    };

    D3DContext::D3DContext(HWND wnd) :
        m_swapchain(nullptr),
        m_device(nullptr),
        m_context(nullptr)
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = 800;
        swapChainDesc.BufferDesc.Height = 600;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
        swapChainDesc.OutputWindow = wnd;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.Windowed = true;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;
        auto fl = D3D_FEATURE_LEVEL_11_0;
        TR_HRESULT(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1,
            D3D11_SDK_VERSION, &swapChainDesc, &m_swapchain,&m_device, NULL, &m_context));

        mainCommandBuffer1 = new D3DMainCommandBuffer();
        mainCommandBuffer1->m_context = m_context;
        mainCommandBuffer1->m_framebuffer = nullptr;
        mainCommandBuffer1->m_pipeline = nullptr;
    }

    D3DContext::~D3DContext()
    {
        RELEASE_COM(m_swapchain);
        RELEASE_COM(m_device);
        RELEASE_COM(m_context);  
    }

    CmPtr<ISampler> D3DContext::createSampler(const SAMPLER_DESC& desc)
    {
        return new D3DSampler(this, desc);
    }

    CmPtr<IBuffer> D3DContext::createBuffer(const BUFFER_DESC& desc)
    {
        return new D3DBuffer(this, desc);
    }

    CmPtr<IPipeline> D3DContext::createPipeline(const PIPELINE_DESC& desc)
    {
        return new D3DPipeline(this, desc);
    }

    CmPtr<IShaderModule> D3DContext::createShaderModule(const SHADER_MODULE_DESC& desc)
    {
        return new D3DShaderModule(this, desc);
    }

    void D3DContext::present(ITexture* image)
    {
        m_swapchain->Present(0, 0);
    }

    D3D11_BLEND_OP D3DContext::getOp(BlendOp op)
    {
        switch (op)
        {
        case BO_ADD:
            return D3D11_BLEND_OP_ADD;
            break;
        case BO_SUB:
            return D3D11_BLEND_OP_SUBTRACT;
            break;
        case BO_DIV:
            throw std::runtime_error("the pizdec");
            break;
        default:
            throw std::runtime_error("the pizdec");
            break;
        }
    }

    D3D11_BLEND D3DContext::getBlend(BlendMod mod)
    {
        switch (mod)
        {
        case BM_ZERO:
            return D3D11_BLEND_ZERO;
            break;
        case BM_ONE:
            return D3D11_BLEND_ONE;
            break;
        case BM_SRC_ALPHA:
            return D3D11_BLEND_SRC_ALPHA;
            break;
        case BM_DST_ALPHA:
            return D3D11_BLEND_DEST_ALPHA;
            break;
        case BM_ONE_MINUS_SRC_ALPHA:
            return D3D11_BLEND_INV_SRC_ALPHA;
            break;
        case BM_ONE_MINUS_DST_ALPHA:
            return D3D11_BLEND_INV_DEST_ALPHA;
            break;
        default:
            throw std::runtime_error("the pizdec");
            break;
        }
    }

    void D3DContext::resizeDrawable(uint32 w, uint32 h)
    {
        m_swapchainRefs.clear();
        m_swapchain->ResizeBuffers(1, w, h, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    }

    ICommandBufferRef D3DContext::createCommandbuffer(const COMMAND_BUFFER_DESC& desc)
    {
        return new D3D11CommandBuffer();
    }

    void D3DContext::submit(ICommandBuffer* cmdBuffer)
    {
        geAssert(cmdBuffer != nullptr);
        if (cmdBuffer != mainCommandBuffer1)     
            ((D3D11CommandBuffer*)cmdBuffer)->excec(mainCommandBuffer1);
        
        m_context->Flush();
    }

    ICommandBufferRef D3DContext::mainCommandBuffer()
    {
        return ref_static_cast<ICommandBuffer>(mainCommandBuffer1);
    }

    CmPtr<IContext> __cdecl CreateD3D11GraphicsContext(const Any& v)
    {
        geAssert(v.is<HWND>());
        return new D3DContext(v.get<HWND>());
    }

    CmPtr<ITexture> D3DContext::nextDrawable()
    {
        ID3D11Texture2D* tx = nullptr;
        TR_HRESULT(m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&tx));

        auto it = m_swapchainRefs.find(tx);
        if (it == end(m_swapchainRefs)) {
            CmPtr<ITexture> realTx = new D3DTexture(this, tx);
            tx->Release();
            m_swapchainRefs.emplace(tx, realTx);
            return realTx;
        }
        CmPtr<ITexture> realTx = it->second;
        tx->Release();    
        return realTx;
    }

    CmPtr<ITexture> D3DContext::createTexture(const TEXTURE_DESC& desc)
    {
        return new D3DTexture(this, desc);
    }

    CmPtr<IFramebuffer> D3DContext::createFramebuffer(const FRAMEBUFFER_DESC& desc)
    {
        return new D3DFramebuffer(this, desc);
    }


    CmPtr<IResourceSet> D3DContext::createResourceSet(const RESOURCESET_DESC& desc)
    {
        return new D3DParamSet(this, desc);
    }
 }