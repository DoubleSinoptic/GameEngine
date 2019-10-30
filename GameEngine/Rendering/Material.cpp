#include "Material.h"
#include "RenderManager.h"

namespace ge 
{
	namespace rt 
	{
		Material::~Material()
		{
			RenderManager::instance().materialsStorage().free(m_materialId);
		}

		void Material::initialize()
		{
			m_materialId = RenderManager::instance().materialsStorage().insert(this);
		}

		void Material::sync(void* data, uint32 flags)
		{

		}

		void Material::setPassInstancedCall(GpuContext& context)
		{
		}
		void Material::setPassCall(GpuContext& context)
		{
		}
	}

	Material::Material() :
		SyncObject(SyncTag<rt::Material>())
	{
	}

	void* Material::sync(SyncAllocator* allocator, uint32 flags) const
	{
		return nullptr;
	}

}
