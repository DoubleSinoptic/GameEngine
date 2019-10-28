#include "VulkanGpuContext.h"
#include "VulkanTexture2D.h"
#include "VulkanBuffer.h"

namespace ge
{
	struct FormatPair
	{
		PixelFormat geFormat;
		VkFormat	vkFormat;
	};

	const FormatPair translatedFormats[] =
	{
		//{PixelFormat::PF_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM},
		//{ PixelFormat::PF_B8G8R8A8_SNORM, VK_FORMAT_B8G8R8A8_SNORM },
		//{ PixelFormat::PF_B8G8R8_UNORM, VK_FORMAT_B8G8R8_UNORM },
		//{ PixelFormat::PF_B8G8R8_SNORM, VK_FORMAT_B8G8R8_SNORM },

		{ PixelFormat::PF_R8G8B8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM },
		{ PixelFormat::PF_R8G8B8A8_SNORM, VK_FORMAT_R8G8B8A8_SNORM },
		{ PixelFormat::PF_R8G8B8_UNORM, VK_FORMAT_R8G8B8_UNORM },
		{ PixelFormat::PF_R8G8B8_SNORM, VK_FORMAT_R8G8B8_SNORM },
		{ PixelFormat::PF_R8G8_UNORM, VK_FORMAT_R8G8_UNORM },
		{ PixelFormat::PF_R8G8_SNORM, VK_FORMAT_R8G8_SNORM },
		{ PixelFormat::PF_R8_UNORM, VK_FORMAT_R8_UNORM },
		{ PixelFormat::PF_R8_SNORM, VK_FORMAT_R8_SNORM },

		{ PixelFormat::PF_R32G32B32A32_UINT, VK_FORMAT_R32G32B32A32_UINT },
		{ PixelFormat::PF_R32G32B32A32_SINT, VK_FORMAT_R32G32B32A32_SINT },
		{ PixelFormat::PF_R32G32B32_UINT, VK_FORMAT_R32G32B32_UINT },
		{ PixelFormat::PF_R32G32B32_SINT, VK_FORMAT_R32G32B32_SINT },

		{ PixelFormat::PF_R32G32B32A32_FLOAT, VK_FORMAT_R32G32B32A32_SFLOAT },
		{ PixelFormat::PF_R16G16B16A16_FLOAT, VK_FORMAT_R16G16B16A16_SFLOAT },

		{ PixelFormat::PF_R32G32B32_FLOAT, VK_FORMAT_R32G32B32_SFLOAT },
		{ PixelFormat::PF_R16G16B16_FLOAT, VK_FORMAT_R16G16B16_SFLOAT },

		{ PixelFormat::PF_R32G32_FLOAT, VK_FORMAT_R32G32_SFLOAT },
		{ PixelFormat::PF_R16G16_FLOAT, VK_FORMAT_R16G16_SFLOAT },

		{ PixelFormat::PF_R32_FLOAT, VK_FORMAT_R32_SFLOAT },
		{ PixelFormat::PF_R16_FLOAT, VK_FORMAT_R16_SFLOAT },

		{ PixelFormat::PF_D16, VK_FORMAT_D16_UNORM },
		{ PixelFormat::PF_D24, VK_FORMAT_D24_UNORM_S8_UINT },
		{ PixelFormat::PF_D32, VK_FORMAT_D32_SFLOAT },
	};


	VulkanGpuContext::VulkanGpuContext()
	{
		for (auto& x : translatedFormats)
		{
			m_toGeFormats.emplace(x.vkFormat, x.geFormat);
			m_toVkFormats.emplace(x.geFormat, x.vkFormat);
		}
	}

	VulkanGpuContext::~VulkanGpuContext()
	{
	}

	VkFormat VulkanGpuContext::getVkFormat(PixelFormat fmt) const
	{
		auto iter = m_toVkFormats.find(fmt);
		geAssert(iter != m_toVkFormats.end());
		return iter->second;
	}

	PixelFormat VulkanGpuContext::getGeFormat(VkFormat fmt) const
	{
		auto iter = m_toGeFormats.find(fmt);
		geAssert(iter != m_toGeFormats.end());
		return iter->second;
	}
}
