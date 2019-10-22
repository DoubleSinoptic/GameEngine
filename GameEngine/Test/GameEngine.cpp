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
#include "Core/ResourceObject.h"
#include "Game/GameObject.h"
#include "Game/GameObjectManager.h"
#include "Core/Config.h"
#include "Game/EngineApplication.h"
#include "Rendering/Buffer.h"
#include "Physics/BoxCollider.h"
#include "Physics/RigidBody.h"
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



struct SmamplRes : public ResourceObject 
{
	SmamplRes() 
	{
		Debug::log("SmamplRes");
	}

	~SmamplRes() 
	{
		Debug::log("~SmamplRes");
	}
};


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
		Debug::log("Body position: {0}", getGameObject()->position());
		i++;
		if (i == 60 * 5)
			GameObjectManager::instance().exit();
	}
};























































class SampleBuffak : public ResourceObject 
{
public:
	SampleBuffak() {}
};

class SampleRenderable 
{
public:
	RPtr<SampleBuffak> bufferf;
	SampleRenderable()
	{
		bufferf = new SampleBuffak();

	}

};

struct SampleCMd 
{
	int asd[15];
};
class CommandBuffer 
{
public:
	std::vector<SampleCMd> saf;
	std::vector<RPtr<SampleBuffak>> buffaks;

	void draw() 
	{
		saf.push_back({});
	}

	void setBuffer(const RPtr<SampleBuffak>& buff, int set, int slot)
	{
		buffaks.push_back(buff);
	}

	void reset() 
	{
		saf.clear();
		buffaks.clear();
	}
};

int main()
{
	/*std::vector<SampleRenderable*> renderes;
	for (int i = 0; i < 40000; i++)
	{
		renderes.push_back(new SampleRenderable());
	}

	CommandBuffer bs;
	while (true)
	{
		uint64 x = Time::totalTicks();
		for (int i = 0; i < 40000; i++)
		{
			bs.setBuffer(renderes[0]->bufferf, 0, 1);
			bs.draw();
		}

		bs.reset();

		Debug::log("{0}", (Time::totalTicks() - x) * 0.000001);
	}*/

	{
		EngineApplication eng;
		GameObject* gm = new GameObject();
		gm->addComponent<SampleComponent>();
		gm->addComponent<RigidBody>()->setMass(1.0);
		gm->addComponent<BoxCollider>()->setExtents({1.0, 1.0 , 1.0 });
		eng.run();
	}
	Debug::log("Engine finished.");
	return 0;
}
