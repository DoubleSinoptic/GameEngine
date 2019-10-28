#pragma once
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Core/Core.h"
#include "RenderAPI/Texture2D.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/GpuContext.h"
#include "Mesh.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"

namespace ge
{
	namespace rt 
	{
		class Material : public SyncObject 
		{
			uint32 m_materialId;
		public:
			constexpr uint32 materialId() const 
			{
				return m_materialId;
			}
			virtual void initialize() override;
			virtual void sync(void* data, uint32 flags) override;

			virtual void setMeshCall(rt::Mesh* mesh, GpuContext& context) = 0;
			virtual void setPassCall(GpuContext& context) = 0;

			virtual void setMeshInstancedCall(rt::Mesh* mesh, GpuContext& context) = 0;
			virtual void setPassInstancedCall(GpuContext& context) = 0;
		};
	
	}

	class Material : public SyncObject
	{
	public:
		Material();


		virtual void* sync(SyncAllocator* allocator, uint32 flags) const override;
	};
}

#endif