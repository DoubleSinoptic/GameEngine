#pragma once
#ifndef __RENDERGRPAH_H__
#define __RENDERGRPAH_H__

#include "Core/Core.h"
#include "Renderable.h"
#include "RenderBase.h"

namespace ge
{
	namespace rt
	{
		class RenderGraph
		{
			Vector<RenderElement> m_forwardRenderabees;
		public:
			void drawGeometryAll(
				RenderChunk& chunk, 
				const std::function<void(rt::Mesh*, ICommandBuffer& context)>& setMeshCall, 
				const std::function<void(rt::Mesh*, ICommandBuffer& context)>& setMeshInstanced,
				uint32 transformSet, 
				const Vector3& viewPosition
			);

			void drawAll(
				RenderChunk& chunk, 
				uint32 transformSet, 
				const Vector3& viewPosition
			);
		};
	}
}

#endif