#include "GpuPool.h"
#include "GpuContext.h"
#include "Core/PoolAllocator.h"
#include "../Rendering/RenderManager.h"

namespace ge
{
	PTexture::~PTexture()
	{
		m_pool->m_samplers.free(sampler);
		m_pool->m_textures.free(texture);
	}

	void PTexture::release() const noexcept
	{
		if (deincrement())
			PoolAllocator::free(this);
	}

	PFramebuffer::~PFramebuffer()
	{
		m_pool->m_framebuffers.free(framebuffer);
	}

	void PFramebuffer::release() const noexcept
	{
		if (deincrement())
			PoolAllocator::free(this);
	}

	GpuPool::~GpuPool()
	{
	}

	GpuPool& GpuPool::instance()
	{
		return rt::RenderManager::instance().gpuPool();
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

		return PoolAllocator::allocate<PTexture>(this, textureObject, samplerObject);
	}

	RPtr<PFramebuffer> GpuPool::allocate(const FRAMEBUFFER_DESC& desc)
	{
		auto& context = GpuContext::instance();

		auto framebufferObject = m_framebuffers.allocate_great([&](const RPtr<Framebuffer>& obj) {
			return compareBinary(obj->getDesc(), desc) == 0;
			}, [&]() { return context.createFramebuffer(desc); });

		return PoolAllocator::allocate<PFramebuffer>(this, framebufferObject);
	}

	void GpuPool::collect()
	{
		m_samplers.collect(3);
		m_textures.collect(3);
		m_framebuffers.collect(3);
	}
	
}
