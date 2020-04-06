#pragma once

#include "IContextObject.h"

namespace ge
{
    class ITexture;

    struct RENDER_TARGET_DESC
    {
        CmPtr<ITexture> surface;
        uint32 mip;
        uint32 layer;
    };

    struct FRAMEBUFFER_DESC {
        SmallVector<RENDER_TARGET_DESC, 8> colors;
        RENDER_TARGET_DESC depth;
        RENDER_TARGET_DESC stencil;
    };

    class IFramebuffer : public IContextObject {
    public:
        virtual const FRAMEBUFFER_DESC& getDesc() const = 0;    

        /*      generic
            void setTmporary(bool flag)
            {
                if (isTmp != flag) 
                {
                    isTmp = flag;
                    if (flag) {
                        for (const auto& tx : m_desc.colors) {
                            tx.surface->addTmp();
                        }
                        m_desc.depth.surface->addTmp();
                        m_desc.stencil.surface->addTmp();
                    } else {
                        for (const auto& tx : m_desc.colors) {
                            tx.surface->releaseTmp();
                        }
                        m_desc.depth.surface->releaseTmp();
                        m_desc.stencil.surface->releaseTmp();
                    }
                }
            }       
        */
        virtual void setTmporary(bool flag) = 0;
    };

    typedef CmPtr<IFramebuffer> IFramebufferRef;
}