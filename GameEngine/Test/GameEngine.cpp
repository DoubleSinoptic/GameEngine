#include <iostream>
#include "Math/Quaternion.h"
#include "Core/ObjectPool.h"
#include "Core/CommandQueue.h"
#include "Math/Matrix3.h"
#include "Core/MessageBox.h"
#include "Core/Thread.h"
#include "SyncSystem/SyncManager.h"
#include "SyncSystem/SyncObject.h"
#include "Core/StringFormat.h"
#include "Core/Debug.h"
#include "Core/ThreadPool.h"
#include "Core/TickLocker.h"
#include "Core/Time.h"
#include "Game/GameObject.h"
#include "Game/GameObjectManager.h"
#include "Core/Config.h"
#include "Game/EngineApplication.h"
#include "RenderFramework/IContext.h"
#include "Physics/BoxCollider.h"
#include "Physics/RigidBody.h"
#include "Window/WindowManager.h"
#include "Window/GlfwWindowManager.h"
#include "Rendering/Camera.h"
using namespace ge;










































































































class Sampl2e : public RtSyncObject
{
public:
	virtual void initialize() override
	{
	}

	virtual void sync(void* data, uint32 flags) override
	{
	}
};
class Sample : public SyncObject 
{
public:
	Sample() : SyncObject(SyncTag<Sampl2e>()) {}

	virtual void* sync(SyncAllocator* allocator, uint32 flags) const override
	{
		return nullptr;
	}
};

struct ToS 
{
	String toString() const noexcept
	{
		return u"hello world!";
	}
};



////struct SmamplRes : public ResourceObject 
////{
////	SmamplRes() 
////	{
////		Debug::log("SmamplRes");
////	}
////
////	~SmamplRes() 
////	{
////		Debug::log("~SmamplRes");
////	}
////};


struct SampleComponent : public Component 
{
	int i = 0;
public:
	virtual void initialize() override 
	{
		activate(CA_UPDATE);
	}
	virtual void fixedUpdate() override
	{
		//Debug::log("Body position: {0}", getGameObject()->position());
	
	}
};



class GpuResourcePool {
	typedef std::map<FRAMEBUFFER_DESC, IFramebufferRef, AnyLess<FRAMEBUFFER_DESC>> frm_map;
	typedef std::map<TEXTURE_DESC, Vector<ITextureRef>, AnyLess<TEXTURE_DESC>>     tx_map;
	
	tx_map												m_pool;
	frm_map												m_renderTargets;
	std::vector<typename frm_map::iterator>				m_removeFramebuffers;
	std::vector<typename Vector<ITextureRef>::iterator> m_removeTetures;
public:
	ITextureRef allocate(const TEXTURE_DESC& a) 
	{
		auto it = m_pool.find(a);
		if (it != end(m_pool)) {
			auto& realPool = it->second;
			for (const auto& x : realPool) {
				if ((x->refCount() - x->tmpCount()) == 1) {
					x->resetAge();
					return x;
				}
			}
			ITextureRef image = rt::RenderManager::instance().gpuContext().createTexture(a);
			realPool.push_back(image);
			return image;
		}
		Vector<ITextureRef> realPool;
		ITextureRef image = rt::RenderManager::instance().gpuContext().createTexture(a);
		realPool.push_back(image);
		m_pool.emplace(a, std::move(realPool));
		return image;
	}

	void setRendetTarget(ICommandBuffer& cmd, const FRAMEBUFFER_DESC& a)
	{
		auto it = m_renderTargets.find(a);
		if (it != end(m_renderTargets)) {	
			if (it->second->refCount() != 1) {
				geAssertFalse("Invalid logic");
			}
			cmd.setRenderTarget(it->second);
			it->second->resetAge();
			return;
		}
		IFramebufferRef frm = rt::RenderManager::instance().gpuContext().createFramebuffer(a);
		frm->setTmporary(true);
		cmd.setRenderTarget(frm);
		m_renderTargets.emplace(a, frm);
		return;
	}

	void tick() 
	{
		for (auto it = m_renderTargets.begin(); it != m_renderTargets.end(); it++) {
			auto& cm = it->second;
			if (cm->refCount() == 1) {
				cm->addAge();
				if (cm->age() > 4) {
					m_removeFramebuffers.push_back(it);
				}
			}
		}
		for (const auto& x : m_removeFramebuffers)
			m_renderTargets.erase(x);
		m_removeFramebuffers.clear();

		for (auto& x : m_pool) {
			for (auto it = x.second.begin(); it != x.second.end(); it++) {
				auto cm = (*it).get();
				if (cm->refCount() == 1) {
					cm->addAge();
					if (cm->age() > 4) {
						m_removeTetures.push_back(it);
					}
				}
			}
			for (const auto& i : m_removeTetures)
				x.second.erase(i);
			m_removeTetures.clear();
		}

	}
};

template<typename X>
class AgePool {


};

class MyRendering : public rt::Renderer
{
	GpuResourcePool pool;
public:
	ITextureRef depthTexture;

	MyRendering() 
	{


	}


	~MyRendering()
	{


	}

	virtual void render(ITexture* texture) override
	{
		IContext& ctx = rt::RenderManager::instance().gpuContext();
		ICommandBuffer& mainCmd = *ctx.mainCommandBuffer();
		const TEXTURE_DESC& samplerRenderTarget = texture->getDesc();


		TEXTURE_DESC depthDesc = {};
		depthDesc.format = TF_D24;
		depthDesc.width = samplerRenderTarget.width;
		depthDesc.heigh = samplerRenderTarget.heigh;
		depthDesc.layers = 1;
		depthDesc.mips = 1;
		depthDesc.usage = TU_DEPTHATTACHMENT;
		
		depthTexture = pool.allocate(depthDesc);
		
		FRAMEBUFFER_DESC desc = {};
		desc.colors.push_back({ texture });
		desc.depth = { depthTexture };

		pool.setRendetTarget(mainCmd, desc);
		
		CLEAR_DESC cdesc = {};
		cdesc.clearValues.push_back({0.0f, 0.0f , 0.0f , 1.0f });
		cdesc.depth = 1.0f;
		
		mainCmd.clear(cdesc);

		pool.tick();
	}
};

int main()
{

	{
		Ptr<GlfwWindowManager> manager = snew<GlfwWindowManager>();
		WindowManager::setCurrentWindowManager(std::static_pointer_cast<GlfwWindowManager>(manager));

		

		WINDOW_DESC desc;
		desc.videoMod = {-1, 800, 600};
		desc.title = u"pizdec";
		Ptr<Window> window = WindowManager::instance().create(desc);
		WindowManager::instance().setMainWindow(window);
		{
			EngineApplication eng;
			auto callbackId = eng.disptach.insert([&]() {
				WindowManager::instance().dispatchMessages();
				if (window->isClosed())
					GameObjectManager::instance().exit();
			});

			Ptr<Camera> cm = snew<Camera>();
			cm->setRenderType<MyRendering>();
			cm->setState(CAMERA_STATE_MAIN_BIT | CAMERA_STATE_RENDER_BIT);

			GameObject* gm = new GameObject();
			gm->addComponent<SampleComponent>();
			gm->addComponent<RigidBody>()->setMass(1.0);
			gm->addComponent<BoxCollider>()->setExtents({ 1.0, 1.0 , 1.0 });
			eng.run();

			eng.disptach.remove(callbackId);
		}
		WindowManager::setCurrentWindowManager(nullptr);
	}
	Debug::log("Engine finished.");
	return 0;
}
