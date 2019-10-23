#include "GpuPool.h"
#include "GpuContext.h"

namespace ge
{
	void PTexture::release() const noexcept
	{
		if (deincrement())
		{
			m_pool->m_samplers.free(sampler);
			m_pool->m_textures.free(texture);
			delete this;
		}
	}

	Ptr<GpuPool> currentGpuPool;

	GpuPool& GpuPool::instance()
	{
		return *currentGpuPool;
	}

	void GpuPool::setCurrentGpuPool(Ptr<GpuPool> pool)
	{
		currentGpuPool = pool;
	}

	RPtr<PTexture> GpuPool::allocate(const TEXTURE2D_DESC& tDesc, const SAMPLER_DESC& sDesc)
	{
		auto& context = GpuContext::instance();
		
		auto textureObject = m_textures.allocate_great([&](const RPtr<Texture2D>& obj) {
			return compareBinary(obj->getDesc(), tDesc) == 0;
		}, [&]() { return context.createTexture2D(tDesc); });

		auto samplerObject = m_samplers.allocate_great([&](const RPtr<Sampler>& obj) {
			return compareBinary(obj->getDesc(), sDesc) == 0;
			}, [&]() { return context.createSampler(sDesc); });

		return new PTexture(this, textureObject, samplerObject);
	}

	void GpuPool::collect()
	{
		m_samplers.collect(4);
		m_textures.collect(4);
	}
}
