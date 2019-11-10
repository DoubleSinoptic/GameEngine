#pragma once
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "Core/Core.h"
#include "Core/IndexedStorage.h"
#include <set>
#include "RenderAPI/GpuContext.h"
#include "RenderAPI/GpuPool.h"
#include "Renderable.h"
#include "RenderBase.h"
#include "Mesh.h"
#include "Material.h"

namespace ge
{
	namespace rt 
	{
		class RenderManager
		{
			IndexedStorage<Material*>	m_materials;
			IndexedStorage<Mesh*>		m_mesh;
			Ptr<GpuContext>				m_gpuContext;
			Ptr<GpuPool>				m_gpuPool;
		public:
			static RenderManager& instance();
			static void setCurrentRenderManager(Ptr<RenderManager> manager);

			void initialize();
			void prepareToDestory();

			RenderManager();
			~RenderManager();

			RenderChunk globalChunk;

			void render();

			IndexedStorage<Material*>& materialsStorage()
			{
				return m_materials;
			}

			GpuContext& gpuContext()
			{
				return *m_gpuContext;
			}

		    GpuPool& gpuPool()
			{
				return *m_gpuPool;
			}

			IndexedStorage<Mesh*>& meshStorage()
			{
				return m_mesh;
			}
		};	
	}

	class RenderManager 
	{
		Ptr<rt::RenderManager> m_rt;
	public:
		RenderManager();
		~RenderManager();
		void render();
		static RenderManager& instance();
		static void setCurrentRenderManager(Ptr<RenderManager> manager);
	};
}

#endif