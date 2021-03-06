#pragma once
#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include "Core/Core.h"
#include "Material.h"
#include "Mesh.h"
#include "Math/Matrix4.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"
#include "RenderFramework/IContext.h"

namespace ge
{
	enum RenderableSyncFlags
	{
		RSF_TRANSFORM = 1 << 0,
		RSF_MESH = 1 << 1,
		RSF_MATERIAL = 1 << 2,
		RSF_TECHQUE = 1 << 3,
		RSF_CULL_DISTANCE = 1 << 4,
		RSF_LOD_RANGE = 1 << 5,
		RSF_ALL = 0xFFFFFFFF
	};

	struct LodRange 
	{
		scalar a;
		scalar b;
		scalar c;
	};

	namespace rt 
	{
		class Renderable;
		class InstacedInstance;

		struct RenderElement
		{
			Renderable* renderable;
			uint32		techqueId;
			scalar		distance;
			uint32		materilId;
			uint32		meshId;
			uint32		subMeshId;

			struct RenderablesLess
			{
				constexpr bool operator ()(const RenderElement& a, const RenderElement& b) const noexcept
				{
					uint32 hl =
						(a.techqueId < b.techqueId) << 4 |
						(a.materilId > b.materilId) << 3 |
						(a.meshId > b.meshId) << 2 |
						(a.subMeshId > b.subMeshId) << 1 |
						(a.renderable > b.renderable) << 0;

					uint32 ll =
						(a.techqueId > b.techqueId) << 4 |
						(a.materilId < b.materilId) << 3 |
						(a.meshId < b.meshId) << 2 |
						(a.subMeshId < b.subMeshId) << 1 |
						(a.renderable < b.renderable) << 0;
					return hl > ll;
				}
			};

			static constexpr bool distanceComporator(const RenderElement& a, const RenderElement& b) noexcept
			{
				uint32 hl =
					(a.renderable > b.renderable) << 0 |
					(a.materilId > b.materilId) << 2 |
					(a.meshId > b.meshId) << 1 |
					(a.distance > b.distance) << 3;

				uint32 ll =
					(a.renderable < b.renderable) << 0 |
					(a.materilId < b.materilId) << 2 |
					(a.meshId < b.meshId) << 1 |
					(a.distance < b.distance) << 3;

				return hl > ll;
			}
		};

		class Renderable : public SyncObject 
		{
			Vector<Material*>		m_materials;
			Mesh*					m_mesh;
			Vector<RenderElement>	m_renderables;

			IBufferRef				m_globalTransform;
			IResourceSetRef			m_globalTransformUniform;

			uint32					m_techque;
			scalar					m_cullDistance;
			LodRange				m_lodRange;

			void updateRenderables();
		public:

			constexpr scalar cullDistance() const 
			{
				return m_cullDistance;
			}

			const Vector<Material*>& materials() const 
			{
				return m_materials;
			}

			constexpr Mesh* mesh() const
			{
				return m_mesh;
			}

			constexpr IResourceSet* transform() const 
			{
				return m_globalTransformUniform;
			}

			virtual void initialize() override;
			~Renderable();
			virtual void sync(void* data, uint32 flags) override;
		};

		class InstancedRenderable : public SyncObject
		{
			Ptr<InstacedInstance>	m_instance;
			Material*						m_materials;
			Mesh*							m_mesh;
			Matrix4							m_transform;
			usize							m_instanceId;
		public:
			const Material* material() const
			{
				return m_materials;
			}

			constexpr Mesh* mesh() const
			{
				return m_mesh;
			}

			constexpr const Matrix4& transform() const
			{
				return m_transform;
			}

			virtual void initialize() override;
			~InstancedRenderable();
			virtual void sync(void* data, uint32 flags) override;
		};
	}

	struct RenderableAllSyncInfo
	{
		Vector<rt::Material*> m_materials;
		rt::Mesh*			  m_mesh;
		Matrix4				  m_transform;
		scalar				  m_cullDistance;
		uint32				  m_techque;
		LodRange			  m_lodRange;
	};

	class Renderable : public SyncObject
	{
		Vector<Ptr<Material>> m_materials;
		Ptr<Mesh>			  m_mesh;
		Matrix4				  m_globalTransform;
		scalar				  m_cullDistance;
		bool				  m_isInstanced;
		uint32				  m_techque;
		LodRange			  m_lodRange;
	public:		
		Renderable();

		void setTransform(const Matrix4& mat);

		void setCullDistance(scalar cullDistance);
		scalar getCullDistance() const;

		void setLodRange(const LodRange& range);
		LodRange getLodRange() const;

		void setMaterial(const Ptr<Material>& material, usize index);
		Ptr<Material> getMaterial(usize index = 0) const;
		void clearMaterials();

		void setMesh(const Ptr<Mesh>& mesh);
		Ptr<Mesh>	getMesh() const;

		void setInstanced(bool isInstanced);
		bool isInstanced() const;

		void setTechqueId(uint32 index);
		uint32 getTechque() const;

		virtual void* sync(SyncAllocator* allocator, uint32 flags) const override;
	};
}

#endif