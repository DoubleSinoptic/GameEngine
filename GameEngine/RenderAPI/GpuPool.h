#pragma once
#ifndef __GPUPOOL_H__
#define __GPUPOOL_H__

#include "Core/Core.h"
#include "Core/ObjectPool.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "Sampler.h"
#include "Framebuffer.h"

namespace ge
{
	class GpuPool;
	class PTexture : public ResourceObject	
	{
		GpuPool* m_pool;
	public:
		PTexture(GpuPool* pool, const RPtr<Texture2D>& t, const RPtr<Sampler>& samp) :
			m_pool(pool),
			texture(t),
			sampler(samp)
		{}

		RPtr<Texture2D> texture;
		RPtr<Sampler>	sampler;

		void release() const noexcept override;
	};

	class PFramebuffer : public ResourceObject
	{
		GpuPool* m_pool;
	public:
		PFramebuffer(GpuPool* pool, const RPtr<Framebuffer>& f) :
			m_pool(pool),
			framebuffer(f)
		{}

		RPtr<Framebuffer> framebuffer;

		void release() const noexcept override;
	};

	class GpuPool 
	{
		friend class PTexture;
		friend class PFramebuffer;
		ObjectPool<Texture2D, RPtr<Texture2D>> m_textures;
		ObjectPool<Sampler, RPtr<Sampler>> m_samplers;
		ObjectPool<Framebuffer, RPtr<Framebuffer>> m_framebuffers;
	public:
		~GpuPool();
		static GpuPool& instance();
		static void setCurrentGpuPool(Ptr<GpuPool> pool);
		RPtr<PTexture> allocate(const TEXTURE2D_DESC& tDesc, const SAMPLER_DESC& sDesc);
		RPtr<PFramebuffer> allocate(const FRAMEBUFFER_DESC& desc);
		void collect();
	};
}

#endif