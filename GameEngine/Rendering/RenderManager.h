#pragma once
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "Core/Core.h"
#include <set>
#include "RenderAPI/GpuContext.h"
#include "Renderable.h"

namespace ge
{
	namespace rt 
	{
		class InstacedInstanceInstance
		{
		public:
		};

		struct InstanceInstance
		{
			InstacedInstanceInstance* instance;
			uint32 materialId;
			uint32 meshId;
		};

		struct InstanceInstanceLess
		{
			constexpr bool operator ()(const InstanceInstance& a, const InstanceInstance& b)
			{
				uint32 hl =
					(a.materialId > b.materialId) << 1 |
					(a.meshId > b.meshId) << 0;

				uint32 ll =
					(a.materialId < b.materialId) << 1 |
					(a.meshId < b.meshId) << 0;

				return hl > ll;
			}
		};

		struct RenderChunk
		{
			typedef std::set<rt::RenderElement, rt::RenderElement::RenderablesLess> deffredforward_type;
			typedef std::set<InstanceInstance, InstanceInstanceLess> instanced_type;
			deffredforward_type renderables;
			instanced_type		instanced;
		};

		class RenderGraph
		{
			Vector<RenderElement> m_forwardRenderabees;
		public:

			void drawGeometry(RenderChunk& chunk)
			{
				GpuContext& gpuContext = GpuContext::instance();
				RenderChunk::deffredforward_type& renderables = chunk.renderables;
				RenderChunk::instanced_type& instancedRenderables = chunk.instanced;
				uint32 meshId = UInt32Max;
				for (auto& x : renderables)
				{
					if (x.meshId != meshId)
					{
						meshId = x.meshId;
						//setVertecesCall
					}
					//drawCall (subMeshInfo)
				}

				for (auto& x : instancedRenderables)
				{
					//setMeshCall
					//drawCall
				}
			}

			void draw(RenderChunk& chunk)
			{
				GpuContext& gpuContext = GpuContext::instance();
				RenderChunk::deffredforward_type& renderables = chunk.renderables;
				RenderChunk::instanced_type& instancedRenderables = chunk.instanced;
				uint32 meshId = UInt32Max;
				uint32 materialId = UInt32Max;

				for (auto& x : renderables)
				{
					if (x.techqueId == 0)
					{
						if (x.materilId != materialId)
						{
							materialId = x.materilId;
							//setPassCall
						}
						if (x.meshId != meshId)
						{
							meshId = x.meshId;
							//setVertecesCall
						}

						//drawCall (subMeshInfo)
					}
					else
						m_forwardRenderabees.push_back(x);
				}

				for (auto& x : instancedRenderables)
				{
					//setPassCall
					//setMeshCall
					//drawCall
				}

				std::sort(m_forwardRenderabees.begin(), m_forwardRenderabees.end(), RenderElement::distanceComporator);

				for (auto& x : m_forwardRenderabees)
				{
					if (x.materilId != materialId)
					{
						materialId = x.materilId;
						//setPassCall
					}
					if (x.meshId != meshId)
					{
						meshId = x.meshId;
						//setVertecesCall
					}

					//drawCall (subMeshInfo)	
				}

				m_forwardRenderabees.clear();
			}


		};


		struct RenderSettings
		{

		};

		class RenderManager
		{

		public:
			static RenderManager& instance();
			static void setCurrentRenderManager(Ptr<RenderManager> manager);

			RenderChunk globalChunk;

			
		};
	
	}
}

#endif