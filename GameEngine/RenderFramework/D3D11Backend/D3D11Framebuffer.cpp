#include "D3D11Framebuffer.h"
#include "D3D11Context.h"
#include "D3D11Texture.h"

namespace ge{
    D3DFramebuffer::D3DFramebuffer(D3DContext* ctx, const FRAMEBUFFER_DESC& desc) :
        m_desc(desc), m_context(static_cast<D3DContext*>(ctx)), m_depthStencilView(nullptr), isTmp(false)
    {
        for (const auto& x : desc.colors) {
            if (x.surface == nullptr)
                continue;
            D3D11_RENDER_TARGET_VIEW_DESC d3d11desc = {};
            if (x.surface->getDesc().layers == 1) {
                d3d11desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
                d3d11desc.Texture2D.MipSlice = x.mip;
                assert(x.layer == 0);
            }
            else {
                d3d11desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
                d3d11desc.Texture2DArray.ArraySize = 1;
                d3d11desc.Texture2DArray.MipSlice = x.mip;
                d3d11desc.Texture2DArray.FirstArraySlice = x.layer;
            }
            auto iter = D3DContext::dxgiFormats.find(x.surface->getDesc().format);
            if (iter == end(D3DContext::dxgiFormats)) {
                throw std::runtime_error("");
            }
            d3d11desc.Format = (DXGI_FORMAT)iter->second;
            ID3D11RenderTargetView* rt;
            auto res = m_context->m_device->CreateRenderTargetView(((D3DTexture*)(IContextObject*)x.surface)->object(), &d3d11desc, &rt);
            TR_HRESULT(res);
            m_targetView.push_back(rt);
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencil = {};
        if (desc.depth.surface) {
            depthStencil.Flags = 0;
            auto iter = D3DContext::dxgiFormats.find(desc.depth.surface->getDesc().format);
            if (iter == end(D3DContext::dxgiFormats)) {
                throw std::runtime_error("");
            }
            depthStencil.Format = (DXGI_FORMAT)iter->second;
            if (desc.depth.surface->getDesc().layers == 1) {
                depthStencil.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                depthStencil.Texture2D.MipSlice = desc.depth.mip;
                assert(desc.depth.layer == 0);
            }
            else {
                depthStencil.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
                depthStencil.Texture2DArray.ArraySize = 1;
                depthStencil.Texture2DArray.MipSlice = desc.depth.mip;
                depthStencil.Texture2DArray.FirstArraySlice = desc.depth.layer;
            }
            TR_HRESULT(m_context->m_device->CreateDepthStencilView(((D3DTexture*)(IContextObject*)desc.depth.surface)->object(), &depthStencil, &m_depthStencilView));
        }
        else {
            m_depthStencilView = nullptr;
        }

    }

    D3DFramebuffer::~D3DFramebuffer() {
        setTmporary(false);
        for (auto r : m_targetView) {
            RELEASE_COM(r);
        }
        RELEASE_COM(m_depthStencilView);
    }

    const FRAMEBUFFER_DESC& D3DFramebuffer::getDesc() const
    {
        return m_desc;
    }

    void D3DFramebuffer::bind(ID3D11DeviceContext* context)
    {
        context->OMSetRenderTargets(m_targetView.size(), m_targetView.data(), m_depthStencilView);
    }

    void D3DFramebuffer::setTmporary(bool flag)
    {
        if (isTmp != flag) 
        {
            isTmp = flag;
            if (flag) {
                for (const auto& tx : m_desc.colors) {
                    tx.surface->addTmp();
                }
                if (m_desc.depth.surface)
                    m_desc.depth.surface->addTmp();
                if (m_desc.stencil.surface)
                    m_desc.stencil.surface->addTmp();
            } else {
                for (const auto& tx : m_desc.colors) {
                    tx.surface->releaseTmp();
                } 
                if (m_desc.depth.surface)
                    m_desc.depth.surface->releaseTmp();
                if (m_desc.stencil.surface)
                    m_desc.stencil.surface->releaseTmp();
            }
        }
    }

}