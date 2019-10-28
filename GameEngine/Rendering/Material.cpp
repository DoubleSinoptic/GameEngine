#include "Material.h"

namespace ge 
{
	namespace rt 
	{
		uint32 staticMaterialId = 0;
		void Material::initialize()
		{
			m_materialId = staticMaterialId++;
		}

		void Material::sync(void* data, uint32 flags)
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
