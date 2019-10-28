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
			void drawGeometry(RenderChunk& chunk, const std::function<void(rt::Mesh*, GpuContext& context)>& setMeshCall, uint32 transformSet, const Vector3& viewPosition)
			{
				GpuContext& gpuContext = GpuContext::instance();
				RenderChunk::deffredforward_type& renderables = chunk.renderables;
				RenderChunk::instanced_type& instancedRenderables = chunk.instanced;
				uint32 meshId = UInt32Max;
				for (auto& x : renderables)
				{
					Renderable* renderable = x.renderable;
					scalar distance = /*squareLength(renderable->positon() - viewPosition)*/0.0;
					if (distance > renderable->cullDistance())
						continue;

					if (x.meshId != meshId)
					{
						meshId = x.meshId;
						setMeshCall(renderable->mesh, gpuContext);
					}
					
					auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);
					gpuContext.setUniform(renderable->transform(), transformSet);
					gpuContext.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
				}

				for (auto& x : instancedRenderables)
				{
					//setMeshCall
					//drawCall
				}
			}

			void draw(RenderChunk& chunk, uint32 transformSet, const Vector3& viewPosition)
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
						Renderable* renderable = x.renderable;
						scalar distance = /*squareLength(renderable->positon() - viewPosition)*/0.0;
						if (distance > renderable->cullDistance())
							continue;

						if (x.materilId != materialId)
						{
							materialId = x.materilId;
							renderable->materials()[x.subMeshId]->setPassCall(gpuContext);
						}
						if (x.meshId != meshId)
						{
							meshId = x.meshId;
							renderable->materials()[x.subMeshId]->setMeshCall(renderable->mesh, gpuContext);
						}
				
						auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);

						gpuContext.setUniform(renderable->transform(), transformSet);
						gpuContext.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
					}
					else
					{
						Renderable* renderable = x.renderable;
						scalar distance = /*squareLength(renderable->positon() - viewPosition)*/0.0;
						if (distance > renderable->cullDistance())
							continue;
						m_forwardRenderabees.push_back(x);
						m_forwardRenderabees.back().distance = distance;
					}
						
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
					Renderable* renderable = x.renderable;

					if (x.materilId != materialId)
					{
						materialId = x.materilId;
						renderable->materials()[x.subMeshId]->setPassCall(gpuContext);
					}
					if (x.meshId != meshId)
					{
						meshId = x.meshId;
						renderable->materials()[x.subMeshId]->setMeshCall(renderable->mesh, gpuContext);
					}
					
					auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);
					gpuContext.setUniform(renderable->transform(), transformSet);
					gpuContext.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
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