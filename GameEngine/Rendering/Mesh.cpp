#include "Mesh.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/GpuContext.h"

namespace ge 
{
	namespace rt 
	{
		uint32 staticMeshId = 0;

		void Mesh::initialize()
		{
			m_meshId = staticMeshId++;
			m_subMeshCount = 0;
		}

		void Mesh::sync(void* data, uint32 flags)
		{
			MeshSyncData* realData = reinterpret_cast<MeshSyncData*>(data);

			if (flags & MSF_VERTECES)
			{
				usize dataSize = realData->m_verteces->size() * sizeof(Vector3);

				BUFFER_DESC bufferDesc = {};
				bufferDesc.memType = MT_STATIC;
				bufferDesc.usage = BU_VERTECES;
				bufferDesc.size = dataSize;

				m_verteces = GpuContext::instance().createBuffer(bufferDesc);
				void* data = m_verteces->map(0, dataSize, AccessFlags::AF_WRITE);
				memcpy(data, realData->m_verteces->data(), dataSize);
				m_verteces->unmap();

				m_firstSubMesh.vertecesOffset = 0;
				m_firstSubMesh.vertecesCount = realData->m_verteces->size();
			}

			if (flags & MSF_NORMALS)
			{
				usize dataSize = realData->m_normals->size() * sizeof(Vector3);

				BUFFER_DESC bufferDesc = {};
				bufferDesc.memType = MT_STATIC;
				bufferDesc.usage = BU_VERTECES;
				bufferDesc.size = dataSize;

				m_normals = GpuContext::instance().createBuffer(bufferDesc);
				void* data = m_normals->map(0, dataSize, AccessFlags::AF_WRITE);
				memcpy(data, realData->m_normals->data(), dataSize);
				m_normals->unmap();
			}

			if (flags & MSF_TANGENTS)
			{
				usize dataSize = realData->m_tangets->size() * sizeof(Vector3);

				BUFFER_DESC bufferDesc = {};
				bufferDesc.memType = MT_STATIC;
				bufferDesc.usage = BU_VERTECES;
				bufferDesc.size = dataSize;

				m_tangents = GpuContext::instance().createBuffer(bufferDesc);
				void* data = m_tangents->map(0, dataSize, AccessFlags::AF_WRITE);
				memcpy(data, realData->m_tangets->data(), dataSize);
				m_tangents->unmap();
			}

			if (flags & MSF_TEXCOORDS)
			{
				usize dataSize = realData->m_texcoords->size() * sizeof(Vector2);

				BUFFER_DESC bufferDesc = {};
				bufferDesc.memType = MT_STATIC;
				bufferDesc.usage = BU_VERTECES;
				bufferDesc.size = dataSize;

				m_texcoords = GpuContext::instance().createBuffer(bufferDesc);
				void* data = m_texcoords->map(0, dataSize, AccessFlags::AF_WRITE);
				memcpy(data, realData->m_texcoords->data(), dataSize);
				m_texcoords->unmap();
			}

			if (flags & MSF_INDECES)
			{
				usize dataSize = realData->m_indeces->size() * sizeof(int);

				BUFFER_DESC bufferDesc = {};
				bufferDesc.memType = MT_STATIC;
				bufferDesc.usage = BU_INDECES;
				bufferDesc.size = dataSize;

				m_indeces = GpuContext::instance().createBuffer(bufferDesc);
				void* data = m_indeces->map(0, dataSize, AccessFlags::AF_WRITE);
				memcpy(data, realData->m_indeces->data(), dataSize);
				m_indeces->unmap();

				m_firstSubMesh.indecesOffset = 0;
				m_firstSubMesh.indecesCount = realData->m_indeces->size();
			}

			if (flags & MSF_SUBMESH)
			{
				m_subMeshs = realData->m_subMeshs;
				if (m_subMeshs)
					m_subMeshCount = m_subMeshs->size();
				else
					m_subMeshCount = 0;
			}

			if (flags & MSF_BONE_WEIGHTS || flags & MSF_BONE_INDECES)
			{
				geAssertFalse("Not impl.");
			}
			SyncAllocator::destroy(realData);
		}
	}
	

	Mesh::Mesh() 
	{

	}

	void Mesh::setSubMeshs(Vector<SubMesh>&& verteces)
	{
		if (m_subMeshesh.use_count() == 1)
			* m_subMeshesh = std::move(verteces);
		else
			m_subMeshesh = snew<Vector<SubMesh>>(std::move(verteces));
		if (isCreated())
			markAsDirty(MSF_SUBMESH);
	}

	void Mesh::setSubMeshs(const Vector<SubMesh>& verteces)
	{
		if (m_subMeshesh.use_count() == 1)
			* m_subMeshesh = (verteces);
		else
			m_subMeshesh = snew<Vector<SubMesh>>((verteces));
		if (isCreated())
			markAsDirty(MSF_SUBMESH);
	}

	void Mesh::setVerteces(Vector<Vector3>&& verteces)
	{
		if (m_verteces.use_count() == 1)
			* m_verteces = std::move(verteces);
		else
			m_verteces = snew<Vector<Vector3>>(std::move(verteces));
		if (isCreated())
			markAsDirty(MSF_VERTECES);
	}

	void Mesh::setVerteces(const Vector<Vector3>& verteces)
	{
		if (m_verteces.use_count() == 1)
			* m_verteces = verteces;
		else
			m_verteces = snew<Vector<Vector3>>(verteces);
		if (isCreated())
			markAsDirty(MSF_VERTECES);
	}

	void Mesh::setNormals(Vector<Vector3>&& normals)
	{
		if (m_verteces.use_count() == 1)
			* m_verteces = std::move(normals);
		else
			m_verteces = snew<Vector<Vector3>>(std::move(normals));
		if (isCreated())
			markAsDirty(MSF_NORMALS);
	}

	void Mesh::setNormals(const Vector<Vector3>& normals)
	{
		if (m_normals.use_count() == 1)
			* m_normals = normals;
		else
			m_normals = snew<Vector<Vector3>>(normals);
		if (isCreated())
			markAsDirty(MSF_NORMALS);
	}

	void Mesh::setTangents(Vector<Vector3>&& tangets)
	{
		if (m_tangets.use_count() == 1)
			* m_tangets = std::move(tangets);
		else
			m_tangets = snew<Vector<Vector3>>(std::move(tangets));
		if (isCreated())
			markAsDirty(MSF_TANGENTS);
	}

	void Mesh::setTangents(const Vector<Vector3>& tangets)
	{
		if (m_tangets.use_count() == 1)
			* m_tangets = tangets;
		else
			m_tangets = snew<Vector<Vector3>>(tangets);
		if (isCreated())
			markAsDirty(MSF_TANGENTS);
	}

	void Mesh::setTexcoords(Vector<Vector2>&& texcoords)
	{
		if (m_texcoords.use_count() == 1)
			* m_texcoords = std::move(texcoords);
		else
			m_texcoords = snew<Vector<Vector2>>(std::move(texcoords));
		if (isCreated())
			markAsDirty(MSF_TEXCOORDS);
	}

	void Mesh::setTexcoords(const Vector<Vector2>& texcoords)
	{
		if (m_texcoords.use_count() == 1)
			* m_texcoords = texcoords;
		else
			m_texcoords = snew<Vector<Vector2>>(texcoords);
		if (isCreated())
			markAsDirty(MSF_TEXCOORDS);
	}

	void Mesh::setIndeces(Vector<int>&& indeces)
	{
		if (m_indeces.use_count() == 1)
			* m_indeces = std::move(indeces);
		else
			m_indeces = snew<Vector<int>>(std::move(indeces));
		if (isCreated())
			markAsDirty(MSF_INDECES);
	}

	void Mesh::setIndeces(const Vector<int>& indeces)
	{
		if (m_indeces.use_count() == 1)
			* m_indeces = indeces;
		else
			m_indeces = snew<Vector<int>>(indeces);
		if (isCreated())
			markAsDirty(MSF_INDECES);
	}

	void Mesh::setBoneWeights(Vector<BoneIndeces>&& indeces)
	{
		if (m_bonesIndeces.use_count() == 1)
			* m_bonesIndeces = std::move(indeces);
		else
			m_bonesIndeces = snew<Vector<BoneIndeces>>(std::move(indeces));
		if (isCreated())
			markAsDirty(MSF_BONE_INDECES);
	}

	void Mesh::setBoneWeights(Vector<Vector4>&& texcoords)
	{
		if (m_boneWeights.use_count() == 1)
			* m_boneWeights = std::move(texcoords);
		else
			m_boneWeights = snew<Vector<Vector4>>(std::move(texcoords));
		if (isCreated())
			markAsDirty(MSF_TEXCOORDS);
	}

	void Mesh::setBoneWeights(const Vector<Vector4>& texcoords)
	{
		if (m_boneWeights.use_count() == 1)
			* m_boneWeights = texcoords;
		else
			m_boneWeights = snew<Vector<Vector4>>(texcoords);
		if (isCreated())
			markAsDirty(MSF_TEXCOORDS);
	}

	void Mesh::setBoneIndeces(const Vector<BoneIndeces>& indeces)
	{
		if (m_bonesIndeces.use_count() == 1)
			* m_bonesIndeces = indeces;
		else
			m_bonesIndeces = snew<Vector<BoneIndeces>>(indeces);
		if (isCreated())
			markAsDirty(MSF_BONE_INDECES);
	}

	void* Mesh::sync(SyncAllocator* allocator, uint32 flags) const
	{
		rt::MeshSyncData* s = allocator->alignedAllocate<rt::MeshSyncData>();
		if (flags & MSF_INDECES)
			s->m_indeces = m_indeces;
		if (flags & MSF_TANGENTS)
			s->m_tangets = m_tangets;
		if (flags & MSF_TEXCOORDS)
			s->m_texcoords = m_texcoords;
		if (flags & MSF_VERTECES)
			s->m_verteces = m_verteces;
		if (flags & MSF_NORMALS)
			s->m_normals = m_normals;
		if (flags & MSF_BONE_INDECES)
			s->m_bonesIndeces = m_bonesIndeces;
		if (flags & MSF_BONE_WEIGHTS)
			s->m_boneWeights = m_boneWeights;
		if (flags & MSF_SUBMESH)
			s->m_subMeshs = m_subMeshesh;
		return s;
	}

	void Mesh::serialize(BinaryWriter* writer) const
	{
		writer->writeString(u"__mesh");
		writer->write<uint32>(
			(m_verteces ? MSF_VERTECES : 0) |
			(m_normals ? MSF_NORMALS : 0) |
			(m_indeces ? MSF_INDECES : 0) |
			(m_tangets ? MSF_TANGENTS : 0) |
			(m_texcoords ? MSF_TEXCOORDS : 0) |
			(m_bonesIndeces ? MSF_BONE_INDECES : 0) |
			(m_boneWeights ? MSF_BONE_WEIGHTS : 0) |
			(m_subMeshesh ? MSF_SUBMESH : 0)
		);
		if (m_verteces)
			writer->writeVector(*m_verteces);
		if (m_normals)
			writer->writeVector(*m_normals);
		if (m_indeces)
			writer->writeVector(*m_indeces);
		if (m_tangets)
			writer->writeVector(*m_tangets);
		if (m_texcoords)
			writer->writeVector(*m_texcoords);
		if (m_bonesIndeces)
			writer->writeVector(*m_bonesIndeces);
		if (m_boneWeights)
			writer->writeVector(*m_boneWeights);
		if (m_subMeshesh)
			writer->writeVector(*m_subMeshesh);
	}

	void Mesh::deserialize(BinaryReader* reader)
	{
		geAssert(reader->readString() == u"__mesh");
		uint32 flags = reader->read<uint32>();
		if (flags & MSF_VERTECES)
			setVerteces(std::move(reader->readVector<Vector3>()));
		if (flags & MSF_NORMALS)
			setNormals(std::move(reader->readVector<Vector3>()));
		if (flags & MSF_INDECES)
			setIndeces(std::move(reader->readVector<int>()));
		if (flags & MSF_TANGENTS)
			setTangents(std::move(reader->readVector<Vector3>()));
		if (flags & MSF_TEXCOORDS)
			setTexcoords(std::move(reader->readVector<Vector2>()));
		if (flags & MSF_BONE_INDECES)
			setBoneIndeces(std::move(reader->readVector<BoneIndeces>()));
		if (flags & MSF_BONE_WEIGHTS)
			setBoneWeights(std::move(reader->readVector<Vector4>()));
		if (flags & MSF_SUBMESH)
			setSubMeshs(std::move(reader->readVector<SubMesh>()));
	}
}

