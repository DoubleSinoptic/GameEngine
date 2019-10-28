#include "Renderable.h"
#include "RenderManager.h"

namespace ge 
{
	namespace rt
	{
		void Renderable::updateRenderables()
		{
			for (auto& x : m_renderables)
			{
				RenderManager::instance().globalChunk.renderables.erase(x);
			}
			m_renderables.clear();
			if (m_mesh)
			{
				for (usize i = 0; i < m_mesh->subMeshCount(); i++)
				{
					RenderElement element;
					element.distance = 0.0;
					element.materilId = m_materials[i]->materialId();
					element.meshId = m_mesh->meshId();
					element.renderable = this;
					element.subMeshId = i;
					element.techqueId = m_techque;
					m_renderables.push_back(element);
					RenderManager::instance().globalChunk.renderables.insert(element);
				}
			}		
		}

		void Renderable::initialize()
		{
			m_mesh = nullptr;

			BUFFER_DESC transformBufferDesc = {};
			transformBufferDesc.size = sizeof(Matrix4);
			transformBufferDesc.memType = MT_DYNAMIC;
			transformBufferDesc.usage = BU_UNIFORM;

			m_globalTransform = GpuContext::instance().createBuffer(transformBufferDesc);

			UNIFORM_DESC uniform = {};
			uniform.resources[0].stages = PS_VERTEX;
			uniform.resources[0].type = UT_BUFFER;

			m_globalTransformUniform = GpuContext::instance().createUniform(uniform);
			m_globalTransformUniform->setBuffer(m_globalTransform, 0);
		}

		Renderable::~Renderable()
		{
			m_materials.clear();
			m_mesh = nullptr;
			updateRenderables();
		}

		void Renderable::sync(void* data, uint32 flags)
		{
			if (flags == RSF_TRANSFORM)
			{
				Buffer* transformBuffer = m_globalTransform.get();
				Matrix4* realTransform = (Matrix4*)transformBuffer->map(0, sizeof(Matrix4), AF_WRITE);
				*realTransform = *reinterpret_cast<Matrix4*>(data);
				transformBuffer->unmap();
			}
			else
			{
				RenderableAllSyncInfo* syncData = reinterpret_cast<RenderableAllSyncInfo*>(data);

				if (flags & RSF_TRANSFORM)
				{
					Buffer* transformBuffer = m_globalTransform.get();
					Matrix4* realTransform = (Matrix4*)transformBuffer->map(0, sizeof(Matrix4), AF_WRITE);
					*realTransform = syncData->m_transform;
					transformBuffer->unmap();
				}
				if (flags & RSF_LOD_RANGE)
					m_lodRange = syncData->m_lodRange;
				if (flags & RSF_CULL_DISTANCE)
					m_cullDistance = syncData->m_cullDistance;

				bool updateRenderable = false;
				if (flags & RSF_MATERIAL)
				{
					m_materials = std::move(syncData->m_materials);
					updateRenderable = true;
				}
				if (flags & RSF_MESH)
				{
					m_mesh = syncData->m_mesh;
					updateRenderable = true;
				}
				if (flags & RSF_TECHQUE)
				{
					m_techque = syncData->m_techque;
					updateRenderable = true;
				}
				if (updateRenderable)
					updateRenderables();
				SyncAllocator::destroy(syncData);
			}
		}

		void InstancedRenderable::initialize()
		{
			m_materials = nullptr;
			m_mesh = nullptr;
		}

		InstancedRenderable::~InstancedRenderable()
		{
			m_materials = nullptr;
			m_mesh = nullptr;
		}

		void InstancedRenderable::sync(void* data, uint32 flags)
		{
			if (flags == RSF_TRANSFORM)
			{
				m_transform = *reinterpret_cast<Matrix4*>(data);
				if (m_instance)
					m_instance->updateTransform(m_transform, m_instanceId);
			}
			else
			{
				RenderableAllSyncInfo* syncData = reinterpret_cast<RenderableAllSyncInfo*>(data);

				if (flags & RSF_TRANSFORM)
				{
					m_transform = syncData->m_transform;
					if (m_instance)
						m_instance->updateTransform(m_transform, m_instanceId);
				}

				if (m_instance)
				{
					m_instance->removeInstance(m_instanceId);
					m_instance = nullptr;
					m_instanceId = (usize)-1;
				}
	
				if (flags & RSF_MATERIAL)
					m_materials = syncData->m_materials[0];

				if (flags & RSF_MESH)
					m_mesh = syncData->m_mesh;
				
				if (m_mesh && m_materials)
				{
					InstanceElement wantedInstance = {};
					wantedInstance.materialId = m_materials->materialId();
					wantedInstance.meshId = m_mesh->meshId();

					auto& instances = RenderManager::instance().globalChunk.instanced;
					auto foundedInstnace = instances.find(wantedInstance);
					if (foundedInstnace != instances.end())
						m_instance = foundedInstnace->instance;
					else 
					{
						wantedInstance.instance = snew<InstacedInstance>(m_mesh, m_materials);
						instances.insert(wantedInstance);
						m_instance = wantedInstance.instance;
					}

					m_instanceId = m_instance->addInstance(m_transform);
				}

				SyncAllocator::destroy(syncData);
			}
		}
	}


	Renderable::Renderable() :
		SyncObject(SyncTag<rt::Renderable>()),
		m_isInstanced(false)
	{
	}

	void Renderable::setTransform(const Matrix4& mat)
	{
		m_globalTransform = mat;
		markAsDirty(RSF_TRANSFORM);
	}

	void Renderable::setCullDistance(scalar cullDistance)
	{
		m_cullDistance = cullDistance;
		markAsDirty(RSF_CULL_DISTANCE);
	}

	scalar Renderable::getCullDistance() const
	{
		return m_cullDistance;
	}

	void Renderable::setLodRange(const LodRange& range)
	{
		m_lodRange = range;
		markAsDirty(RSF_LOD_RANGE);
	}

	LodRange Renderable::getLodRange() const
	{
		return m_lodRange;
	}

	void Renderable::setMaterial(const Ptr<Material>& material, usize index)
	{
		if (m_materials.size() <= index)
			m_materials.resize(index + 1);
		m_materials[index] = material;
		markAsDirty(RSF_MATERIAL);
	}

	Ptr<Material> Renderable::getMaterial(usize index) const
	{
		return m_materials[index];
	}

	void Renderable::clearMaterials()
	{
		m_materials.clear();
		markAsDirty(RSF_MATERIAL);
	}

	void Renderable::setMesh(const Ptr<Mesh>& mesh)
	{
		m_mesh = mesh;
		markAsDirty(RSF_MESH);
	}

	Ptr<Mesh> Renderable::getMesh() const
	{
		return m_mesh;
	}

	void Renderable::setInstanced(bool isInstanced)
	{
		if (m_isInstanced != isInstanced)
		{
			if (isInstanced)
				create<rt::InstancedRenderable>();		
			else 
				create<rt::Renderable>();
			
			markAsDirty(RSF_ALL);
			m_isInstanced = isInstanced;
		}
	}

	bool Renderable::isInstanced() const
	{
		return m_isInstanced;
	}

	void Renderable::setTechqueId(uint32 index)
	{
		m_techque = index;
		markAsDirty(RSF_TECHQUE);
	}

	uint32 Renderable::getTechque() const
	{
		return m_techque;
	}

	void* Renderable::sync(SyncAllocator* allocator, uint32 flags) const
	{
		if (flags == RSF_TRANSFORM)
		{
			Matrix4* mat = allocator->allocate<Matrix4>();
			*mat = m_globalTransform;
			return mat;
		}
		else 
		{
			RenderableAllSyncInfo* syncData = allocator->allocate<RenderableAllSyncInfo>();
			syncData->m_cullDistance = m_cullDistance;
			syncData->m_techque = m_techque;
			syncData->m_transform = m_globalTransform;
			syncData->m_lodRange = m_lodRange;
			if (flags & RSF_MESH)
			{
				if (m_mesh)
				{
					syncData->m_mesh = m_mesh->getRtObjectT<rt::Mesh>();
				}
				else
					syncData->m_mesh = nullptr;
			}
			if (flags & RSF_MATERIAL)
			{			
				for (auto& x : m_materials) 
				{
					syncData->m_materials.push_back(x->getRtObjectT<rt::Material>());
				}			
			}
			return syncData;
		}
		
	}

}
