#pragma once

#include "IContextObject.h"

namespace ge
{
    enum ShaderType {
        ST_COMPUTE,
        ST_VERTEX,
        ST_FRAGMENT,
        ST_GEOMETRY
    };


    struct SHADER_MODULE_DESC {
        ShaderType                    type;

        /**
        * spir-v code
        */
        String                   entryPoint;
        Vector<byte>             code;
    };

    class IShaderModule : public IContextObject
    {
    public:
        virtual const SHADER_MODULE_DESC& getDesc() const = 0;
    };

    typedef CmPtr<IShaderModule> IShaderModuleRef;
}