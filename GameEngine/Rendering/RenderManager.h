#pragma once
#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

#include "Core/Core.h"
#include "Core/IndexedStorage.h"
#include <set>
#include "RenderAPI/GpuContext.h"
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
		public:
			static RenderManager& instance();
			static void setCurrentRenderManager(Ptr<RenderManager> manager);

			RenderChunk globalChunk;

			IndexedStorage<Material*>& materialsStorage() 
			{
				return m_materials;
			}

			IndexedStorage<Mesh*>& meshStorage()
			{
				return m_mesh;
			}
		};
	
	}
}

#endif