#include "Camera.h"
namespace ge 
{
	rt::Camera::~Camera()
	{
		RenderManager::instance().removeCamera(this);
	}

	rt::Camera::Camera() :
		m_cameraState(0)
	{

	}

	void rt::Camera::initialize()
	{
		RenderManager::instance().addCamera(this);
	}

	void rt::Camera::sync(void* data, uint32 flags)
	{
		CameraSyncFrame* cam = reinterpret_cast<CameraSyncFrame*>(data);
		if (CSF_RENDERTYPE & flags)
		{
			if (cam->funtcion)
			{
				if (m_renderer)
					m_renderer = nullptr;
				m_renderer = cam->funtcion();
			}
		}
		if (CSF_CAMERASTATE & flags) {
			m_cameraState = cam->state;
		}
		SyncAllocator::destroy(cam);
	}

	void rt::Camera::render(ITexture* t)
	{
		if (m_renderer)
			m_renderer->render(t);
	}


	Camera::Camera() :
		SyncObject(SyncTag<rt::Camera>()),
		m_cameraState(0)
	{

	}

	void* Camera::sync(SyncAllocator* allocator, uint32 flags) const
	{
		CameraSyncFrame* cam = allocator->allocate<CameraSyncFrame>();
		if (CSF_RENDERTYPE & flags) {
			/*warning... ok... it's opt... nice!*/
			cam->funtcion = std::move(m_rendererType);
		}
		if (CSF_CAMERASTATE & flags) {
			cam->state = m_cameraState;
		}
		return cam;
	}

	void Camera::setState(uint32 state) {
        m_cameraState = state;
        markAsDirty(CSF_CAMERASTATE);
    }

}

