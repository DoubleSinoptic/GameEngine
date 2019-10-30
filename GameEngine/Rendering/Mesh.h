#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "Core/Serializer.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "RenderAPI/Buffer.h"
#include "RenderAPI/GpuContext.h"
namespace ge
{
	struct BoneIndeces
	{
		uint32 a, b, c, d;
	};

	struct SubMesh
	{
		uint32 vertecesOffset;
		uint32 vertecesCount;
		uint32 indecesOffset;
		uint32 indecesCount;
	};

	namespace rt 
	{
		struct MeshSyncData 
		{
			Ptr<Vector<Vector3>>		m_verteces;
			Ptr<Vector<Vector3>>		m_tangets;
			Ptr<Vector<Vector2>>		m_texcoords;
			Ptr<Vector<Vector3>>		m_normals;
			Ptr<Vector<int>>			m_indeces;	
			Ptr<Vector<SubMesh>>		m_subMeshs;
			Ptr<Vector<BoneIndeces>>    m_bonesIndeces;
			Ptr<Vector<Vector4>>		m_boneWeights;
		};

		class Mesh : public SyncObject
		{
			Ptr<Vector<SubMesh>>	 m_subMeshs;
			RPtr<Buffer> m_verteces;
			RPtr<Buffer> m_texcoords;
			RPtr<Buffer> m_tangents;
			RPtr<Buffer> m_normals;
			RPtr<Buffer> m_indeces;
			usize		 m_subMeshCount;
			RPtr<Buffer> m_bonesIndeces;
			RPtr<Buffer> m_boneWeights;
			SubMesh		 m_firstSubMesh;
			uint32		 m_meshId;
		public:
			virtual void initialize() override;
			virtual void sync(void* data, uint32 flags) override;
			virtual ~Mesh();

			constexpr uint32 meshId() const 
			{
				return m_meshId;
			}

			constexpr usize subMeshCount() const 
			{
				return m_subMeshCount == 0 ? 1 : m_subMeshCount;
			}

			const SubMesh& subMeshAt(usize index) const 
			{
				if (m_subMeshCount)
					return (*m_subMeshs)[index];
				else
					return m_firstSubMesh;
			}

			void setMeshCall(GpuContext& context);
			void setMeshInstancedCall(GpuContext& context);
		};
	}

	enum MeshSyncFlags 
	{
		MSF_VERTECES = 1 << 0,
		MSF_NORMALS = 1 << 1,
		MSF_TEXCOORDS = 1 << 2,
		MSF_TANGENTS = 1 << 3,
		MSF_INDECES = 1 << 4,
		MSF_BONE_INDECES = 1 << 5,
		MSF_BONE_WEIGHTS = 1 << 6,
		MSF_SUBMESH = 1 << 7
	};

	class Mesh : public SyncObject, public ISerializable
	{
		Ptr<Vector<BoneIndeces>> m_bonesIndeces;
		Ptr<Vector<Vector4>> m_boneWeights;

		Ptr<Vector<Vector3>> m_verteces;
		Ptr<Vector<Vector3>> m_tangets;
		Ptr<Vector<Vector2>> m_texcoords;
		Ptr<Vector<Vector3>> m_normals;
		Ptr<Vector<int>>	 m_indeces;
		Ptr<Vector<SubMesh>> m_subMeshesh;
	public:
		Mesh();

		const Vector<Vector3>& getVertexces() const noexcept 
		{
			return *m_verteces;
		}

		const Vector<Vector3>& getNormals() const noexcept
		{
			return *m_normals;
		}

		const Vector<int>& getIndeces() const noexcept
		{
			return *m_indeces;
		}

		const Vector<Vector3>& getTangets() const noexcept
		{
			return *m_tangets;
		}

		const Vector<Vector2>& getTexcoords() const noexcept
		{
			return *m_texcoords;
		}

		const Vector<SubMesh>& getSubMesh() const noexcept
		{
			return *m_subMeshesh;
		}

		void setSubMeshs(Vector<SubMesh>&& verteces);

		void setSubMeshs(const Vector<SubMesh>& verteces);

		void setVerteces(Vector<Vector3>&& verteces);

		void setVerteces(const Vector<Vector3>& verteces);

		void setNormals(Vector<Vector3>&& normals);

		void setNormals(const Vector<Vector3>& normals);

		void setTangents(Vector<Vector3>&& tangets);

		void setTangents(const Vector<Vector3>& tangets);

		void setTexcoords(Vector<Vector2>&& texcoords);

		void setTexcoords(const Vector<Vector2>& texcoords);

		void setIndeces(Vector<int>&& indeces);

		void setIndeces(const Vector<int>& indeces);

		void setBoneWeights(Vector<Vector4>&& texcoords);

		void setBoneWeights(const Vector<Vector4>& texcoords);

		void setBoneWeights(Vector<BoneIndeces>&& indeces);

		void setBoneIndeces(const Vector<BoneIndeces>& indeces);

		virtual void* sync(SyncAllocator* allocator, uint32 flags) const override;

		virtual void serialize(BinaryWriter* writer) const override;
		virtual void deserialize(BinaryReader* reader) override;
	};
}

#endif