#pragma once
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "Core/Core.h"
#include <set>
#include "Rendering/GpuContext.h"
namespace ge
{



	class Renderable
	{
	public:
		


	};

	struct RenderElement 
	{
		Renderable* renderable;
		uint32		techqueId;	
		scalar		distance;
		uint32		materilId;
		uint32		meshId;
		uint32		subMeshId;
	};
	
	struct RenderablesLess
	{
		constexpr bool operator ()(const RenderElement& a, const RenderElement& b)
		{
			uint32 hl =
				(a.techqueId < b.techqueId) << 3 |
				(a.materilId > b.materilId) << 2 |
				(a.meshId > b.meshId) << 1 |
				(a.subMeshId > b.subMeshId) << 0;

			uint32 ll =
				(a.techqueId > b.techqueId) << 3 |
				(a.materilId < b.materilId) << 2 |
				(a.meshId < b.meshId) << 1 |
				(a.subMeshId < b.subMeshId) << 0;

			return hl > ll;
		}
	};

	constexpr bool distanceComporator(const RenderElement& a, const RenderElement& b) noexcept
	{
		uint32 hl =
			(a.materilId > b.materilId) << 1 |
			(a.meshId > b.meshId) << 0 |
			(a.distance > b.distance) << 2;

		uint32 ll =
			(a.materilId < b.materilId) << 1 |
			(a.meshId < b.meshId) << 0 |
			(a.distance < b.distance) << 2;

		return hl > ll;
	}
	
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
		typedef std::set<RenderElement, RenderablesLess> deffredforward_type;
		typedef std::set<InstanceInstance, InstanceInstanceLess> instanced_type;
		deffredforward_type renderables;
		instanced_type		instanced;
	};

	class RenderGraph 
	{
		std::vector<RenderElement> m_forwardRenderabees;
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

			std::sort(m_forwardRenderabees.begin(), m_forwardRenderabees.end(), distanceComporator);

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
		std::set<Renderable*> renderables;

		void draw() 
		{
			
		
		
		}
	};
}

#endif