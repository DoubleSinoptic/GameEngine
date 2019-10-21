#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Core/Core.h"
#include "Core/ResourceObject.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Buffer.h"

namespace ge
{
	struct BoneIndeces
	{
		uint32 a, b, c, d;
	};

	namespace rt 
	{
		struct MeshSyncData 
		{
			Ptr<Vector<Vector3>> m_verteces;
			Ptr<Vector<Vector3>> m_tangets;
			Ptr<Vector<Vector2>> m_texcoords;
			Ptr<Vector<Vector3>> m_normals;
			Ptr<Vector<int>>	 m_indeces;	

			Ptr<Vector<BoneIndeces>> m_bonesIndeces;
			Ptr<Vector<Vector4>> m_boneWeights;
		};

		class Mesh : public SyncObject
		{
			RPtr<Buffer> m_verteces;
			RPtr<Buffer> m_texcoords;
			RPtr<Buffer> m_tangents;
			RPtr<Buffer> m_normals;
			RPtr<Buffer> m_indeces;

			RPtr<Buffer> m_bonesIndeces;
			RPtr<Buffer> m_boneWeights;
		public:
			virtual void initialize() override;
			virtual void sync(void* data, uint32 flags) override;
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
		MSF_BONE_WEIGHTS = 1 << 6
	};

	class Mesh : public SyncObject
	{
		Ptr<Vector<BoneIndeces>> m_bonesIndeces;
		Ptr<Vector<Vector4>> m_boneWeights;

		Ptr<Vector<Vector3>> m_verteces;
		Ptr<Vector<Vector3>> m_tangets;
		Ptr<Vector<Vector2>> m_texcoords;
		Ptr<Vector<Vector3>> m_normals;
		Ptr<Vector<int>>	 m_indeces;
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
	};
}

#endif