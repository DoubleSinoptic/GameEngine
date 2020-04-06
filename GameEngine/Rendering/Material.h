#pragma once
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "Core/Core.h"
#include "RenderFramework/IContext.h"
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
			~Material();

			constexpr uint32 materialId() const 
			{
				return m_materialId;
			}
			virtual void initialize() override;
			virtual void sync(void* data, uint32 flags) override;
	
			virtual void setPassInstancedCall(ICommandBuffer& context);
			virtual void setPassCall(ICommandBuffer& context);
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