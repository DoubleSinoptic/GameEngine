#pragma once
#ifndef __RENDERBASE_H__
#define __RENDERBASE_H__

#include "Core/Core.h"
#include "Math/Matrix4.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderable.h"
#include <set>

namespace ge
{
	namespace rt 
	{
		class InstacedInstance
		{
			Material* m_material;
			Mesh* m_mesh;
			IBufferRef m_buffer;
		public:
			InstacedInstance(Mesh* mesh, Material* material) :
				m_mesh(mesh),
				m_material(material)
			{}

			constexpr Material* material() const
			{
				return m_material;
			}

			constexpr Mesh* mesh() const
			{
				return m_mesh;
			}

			void  bindBuffers(ICommandBuffer& context);
			usize instanceCount() const;

			void  updateTransform(const Matrix4& transform, usize index);
			usize addInstance(const Matrix4& mat);
			void  removeInstance(usize index);
		};

		struct InstanceElement
		{
			Ptr<InstacedInstance>			instance;
			uint32							materialId;
			uint32							meshId;
		};

		struct InstanceInstanceLess
		{
			constexpr bool operator ()(const InstanceElement& a, const InstanceElement& b) const
			{
				uint32 hl =
					(a.materialId > b.materialId) << 1 |
					(a.meshId > b.meshId) << 0;

				uint32 ll =
					(a.materialId < b.materialId) << 1 |
					(a.meshId < b.meshId) << 0;

				return hl > ll;
			}
		};

		struct RenderChunk
		{
			typedef std::set<rt::RenderElement, rt::RenderElement::RenderablesLess> deffredforward_type;
			typedef std::set<InstanceElement, InstanceInstanceLess> instanced_type;
			deffredforward_type renderables;
			instanced_type		instanced;
		};
	}
}

#endif