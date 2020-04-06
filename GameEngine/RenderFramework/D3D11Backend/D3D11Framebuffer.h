#pragma once

#include "D3D11Context.h"

namespace ge {

    class D3DFramebuffer : public IFramebuffer {
        FRAMEBUFFER_DESC                        m_desc;
        D3DContext* m_context;
        bool isTmp;
    public:

        std::vector<ID3D11RenderTargetView*>    m_targetView;
        ID3D11DepthStencilView* m_depthStencilView;

        D3DFramebuffer(D3DContext* ctx, const FRAMEBUFFER_DESC& desc);

        ~D3DFramebuffer();

        virtual const FRAMEBUFFER_DESC& getDesc() const override;

        void bind(ID3D11DeviceContext* context);

        // Унаследовано через IFramebuffer
        virtual void setTmporary(bool flag) override;
    };


}