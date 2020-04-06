#pragma once

#include "IContextObject.h"
#include "UnionTypes.h"

namespace ge
{
    struct RESOURCESET_DESC {
        SmallVector<CmPtr<IContextObject>, 10>  objects;
        SmallVector<PARAM_DECL, 10> params;
    };

    class IResourceSet : public IContextObject {
    public:
        virtual const RESOURCESET_DESC& getDesc() const = 0;
    };

    typedef CmPtr<IResourceSet> IResourceSetRef;
}