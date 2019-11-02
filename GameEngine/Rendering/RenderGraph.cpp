#include "RenderGraph.h"
#include "RenderManager.h"
#include "RenderBase.h"

namespace ge
{
	namespace rt
	{
		void RenderGraph::drawGeometryAll(RenderChunk& chunk, const std::function<void(rt::Mesh*, CommandBuffer& context)>& setMeshCall, const std::function<void(rt::Mesh*, CommandBuffer& context)>& setMeshInstanced, uint32 transformSet, const Vector3& viewPosition)
		{
			GpuContext& gpuContext = GpuContext::instance();
			CommandBuffer& commandBuffer = *gpuContext.mainCb();
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
					setMeshCall(renderable->mesh(), commandBuffer);
				}

				auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);
				commandBuffer.setUniform(renderable->transform(), transformSet);
				commandBuffer.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
			}

			meshId = UInt32Max;
			for (auto& x : instancedRenderables)
			{
				Mesh* mesh = x.instance->mesh();
				if (x.meshId != meshId)
				{
					meshId = x.meshId;
					setMeshInstanced(mesh, commandBuffer);
				}

				x.instance->bindBuffers(commandBuffer);
				for (usize i = 0; i < mesh->subMeshCount(); i++)
				{
					const SubMesh& subMesh = mesh->subMeshAt(i);
					commandBuffer.drawIndexedInstanced(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset, 0, x.instance->instanceCount());
				}
			}
		}

		void RenderGraph::drawAll(RenderChunk& chunk, uint32 transformSet, const Vector3& viewPosition)
		{
			auto& materials = RenderManager::instance().materialsStorage();
			auto& meshs = RenderManager::instance().meshStorage();
			GpuContext& gpuContext = GpuContext::instance();
			CommandBuffer& commandBuffer = *gpuContext.mainCb();
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
						materials[materialId]->setPassCall(commandBuffer);
					}
					if (x.meshId != meshId)
					{
						meshId = x.meshId;
						meshs[meshId]->setMeshCall(commandBuffer);
					}

					auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);

					commandBuffer.setUniform(renderable->transform(), transformSet);
					commandBuffer.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
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

			meshId = UInt32Max;
			materialId = UInt32Max;

			for (auto& x : instancedRenderables)
			{
				Mesh* mesh = x.instance->mesh();
				if (x.materialId != materialId)
				{
					materialId = x.materialId;
					materials[materialId]->setPassInstancedCall(commandBuffer);
				}
				if (x.meshId != meshId)
				{
					meshId = x.meshId;
					meshs[meshId]->setMeshInstancedCall(commandBuffer);
				}

				x.instance->bindBuffers(commandBuffer);
				for (usize i = 0; i < mesh->subMeshCount(); i++)
				{
					const SubMesh& subMesh = mesh->subMeshAt(i);
					commandBuffer.drawIndexedInstanced(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset, 0, x.instance->instanceCount());
				}
			}

			std::sort(m_forwardRenderabees.begin(), m_forwardRenderabees.end(), RenderElement::distanceComporator);
			meshId = UInt32Max;
			materialId = UInt32Max;

			for (auto& x : m_forwardRenderabees)
			{
				Renderable* renderable = x.renderable;

				if (x.materilId != materialId)
				{
					materialId = x.materilId;
					materials[materialId]->setPassCall(commandBuffer);
				}
				if (x.meshId != meshId)
				{
					meshId = x.meshId;
					meshs[meshId]->setMeshCall(commandBuffer);
				}

				auto& subMesh = renderable->mesh()->subMeshAt(x.subMeshId);
				commandBuffer.setUniform(renderable->transform(), transformSet);
				commandBuffer.drawIndexed(subMesh.indecesOffset, subMesh.indecesCount, subMesh.vertecesOffset);
			}

			m_forwardRenderabees.clear();
		}
	}
}