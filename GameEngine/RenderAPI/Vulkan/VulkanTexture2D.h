#pragma once
#ifndef __VULKANTEXTURE2D_H__
#define __VULKANTEXTURE2D_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "../Texture2D.h"
#include <unordered_map>

namespace ge
{
	struct MipLayerView
	{
		uint16 mip;
		uint16 level;

		constexpr usize hash_value() const
		{
			return (usize(mip) << 16) | usize(level);
		}
	};

	constexpr bool operator ==(const MipLayerView& cd, const MipLayerView& bs)
	{
		return cd.hash_value() == bs.hash_value();
	}

	struct MipLayerViewHash
	{
		constexpr usize operator()(const MipLayerView& k) const
		{
			return k.hash_value();
		}
	};

	class VulkanTexture2D : public Texture2D
	{
		VulkanGpuContext*		m_instance;
		VkDeviceSize			m_memOff;
		VkDeviceMemory			m_mem;
		VmaAllocation			m_allocation;
		VkImageView				m_samplerView;
		VkImage					m_image;
		VkImageView				m_imageView;
		VkImageAspectFlags		m_aspectFlags;
		VkImageLayout			m_baseLayout;
		bool					m_isInternal;
		std::unordered_map<MipLayerView, VkImageView, MipLayerViewHash> m_views;
	public:
		VulkanTexture2D(TEXTURE2D_DESC& desc, VulkanGpuContext* instance);
		~VulkanTexture2D();

		VkImageView getImageView(uint32 mip, uint32 layer);

		struct LayoutAccessStageState
		{
			VkImageLayout		 layout;
			VkAccessFlags		 access;
			VkPipelineStageFlags stage;
		};

		LayoutAccessStageState switchToRelease();
		LayoutAccessStageState switchToAcquire();
	};
}

#endif