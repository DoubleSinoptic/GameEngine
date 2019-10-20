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
		i++;
		if (i == 60 * 5)
			GameObjectManager::instance().exit();
	}
};


int main()
{

	{
		EngineApplication eng;
		GameObject* gm = new GameObject();
		gm->addComponent<SampleComponent>();
		eng.run();
	}

	Debug::log("Engine finished.");
	return 0;
}
