#pragma once
#ifndef __GPUPOOL_H__
#define __GPUPOOL_H__

#include "Core/Core.h"
#include "Core/ObjectPool.h"
#include "Texture.h"
#include "Buffer.h"
#include "Sampler.h"

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

	class GpuPool 
	{
		friend class PTexture;
		ObjectPool<Texture2D, RPtr<Texture2D>> m_textures;
		ObjectPool<Sampler, RPtr<Sampler>> m_samplers;
	public:
		static GpuPool& instance();
		static void setCurrentGpuPool(Ptr<GpuPool> pool);
		RPtr<PTexture> allocate(const TEXTURE2D_DESC& tDesc, const SAMPLER_DESC& sDesc);
		void collect();
	};
}

#endif