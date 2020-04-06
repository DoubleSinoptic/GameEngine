#pragma once
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "Core/Core.h"
#include "Core/IndexedStorage.h"
#include <set>
#include "RenderFramework/IContext.h"
#include "Renderable.h"
#include "RenderBase.h"
#include "Mesh.h"
#include "Material.h"

namespace ge
{
	namespace rt 
	{
		class Camera;
		class Renderer;
		class RenderManager
		{
			IndexedStorage<Material*>	m_materials;
			IndexedStorage<Mesh*>		m_mesh;
			IContextRef					m_gpuContext;
			Vector<Camera*>				m_camers;
		public:

			void addCamera(Camera* x)
			{
				m_camers.push_back(x);
			}

			void removeCamera(Camera* x)
			{
				removeFind(m_camers, x);
			}

			static RenderManager& instance();
			static void setCurrentRenderManager(RenderManager* manager);

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

			IContext& gpuContext()
			{
				return *m_gpuContext;
			}


			IndexedStorage<Mesh*>& meshStorage()
			{
				return m_mesh;
			}
		};	
	}
	class Camera;
	class RenderManager 
	{
		rt::RenderManager* m_rt;
	public:
		RenderManager();
		~RenderManager();
		void render();
		void setMainCamera(const Ptr<Camera>& cam) {

		}

		constexpr rt::RenderManager* rtObject() const {
			return m_rt;
		}

		static RenderManager& instance();
		static void setCurrentRenderManager(Ptr<RenderManager> manager);
	};
}

#endif