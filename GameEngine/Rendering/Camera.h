#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Core/Core.h"
#include "RenderFramework/IContext.h"
#include "Mesh.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"
#include "RenderManager.h"

namespace ge
{
	constexpr const uint32 CAMERA_STATE_MAIN_BIT = 1;
	constexpr const uint32 CAMERA_STATE_RENDER_BIT = 2;
	namespace rt
	{
		class Renderer {
		public:
			virtual ~Renderer() = default;
			virtual void render(ITexture* texture) = 0;
		};

		class Camera : public SyncObject
		{
			Ptr<Renderer> m_renderer;
			TEXTURE_DESC  m_lastDesck;
			uint32        m_cameraState;
		public:
			const uint32& cameraState = m_cameraState;
			const TEXTURE_DESC& lastDesck = m_lastDesck;
			~Camera();
			Camera();

			virtual void initialize() override;
			virtual void sync(void* data, uint32 flags) override;

			void render(ITexture* t);
		};

	}

	struct CameraSyncFrame {
		std::function<Ptr<rt::Renderer>(void)> funtcion;
		uint32	state;
	};

	constexpr const uint32 CSF_RENDERTYPE = 1;
	constexpr const uint32 CSF_CAMERASTATE = 2;
	class Camera : public SyncObject
	{
		std::function<Ptr<rt::Renderer>(void)> m_rendererType;
		uint32        m_cameraState;
	public:
		Camera();

		virtual void* sync(SyncAllocator* allocator, uint32 flags) const override;

		void setState(uint32 state);

		template<typename T>
		void setRenderType() {
			m_rendererType = []() -> Ptr<rt::Renderer> {
				return std::dynamic_pointer_cast<rt::Renderer>(snew<T>());
			};
			markAsDirty(CSF_RENDERTYPE);
		}
	};
}

#endif