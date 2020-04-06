#include "VulkanTexture2D.h"
#include "VulkanCommandBuffer.h"

namespace ge 
{
	TEXTURE2D_DESC customDesc(const VULKAN_CUSTOM_IMAGE_DESC& desc, VulkanGpuContext* instance)
	{
		TEXTURE2D_DESC d = {};
		d.format = instance->getGeFormat(desc.format);
		d.layerCount = 1;
		d.mipCount = 1;
		d.height = desc.height;
		d.width = desc.width;
		d.usage = TU_COLORATTACHMENT;
		return d;
	}

	VulkanTexture2D::VulkanTexture2D(const VULKAN_CUSTOM_IMAGE_DESC& desc, VulkanGpuContext* instance) :
		Texture2D(customDesc(desc, instance), instance),
		m_format(desc.format),
		m_baseLayout(desc.baseLayout),
		m_isInternal(true),
		m_image(desc.image)
	{
		
	}


	VulkanTexture2D::VulkanTexture2D(const TEXTURE2D_DESC& desc, VulkanGpuContext* instance) :
		Texture2D(desc, instance),
		m_isInternal(false)
	{
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.format = gpuContextT<VulkanGpuContext>().getVkFormat(desc.format);
		m_format = imageCreateInfo.format;
		imageCreateInfo.extent = { desc.width, desc.height, 1 };
		imageCreateInfo.mipLevels = desc.mipCount;
		imageCreateInfo.arrayLayers = desc.layerCount;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage =
			((desc.usage & TextureUsage::TU_SAMPLED_IMAGE) ? VK_IMAGE_USAGE_SAMPLED_BIT : 0) |
			((desc.usage & TextureUsage::TU_COLORATTACHMENT) ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0) |
			((desc.usage & TextureUsage::TU_DEPTHATTACHMENT) ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0) |
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; //TODO: ignore pixels flag
	
		if (desc.usage & TextureUsage::TU_DEPTHATTACHMENT)
		{
			m_aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
			m_baseLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
		}
		else
		{
			m_baseLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			m_aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		}

		imageCreateInfo.flags = (desc.layerCount > 1) ? VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT : 0;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		CHECK_VULKAN(vkCreateImage(gpuContextT<VulkanGpuContext>().device, &imageCreateInfo, nullptr, &m_image));


		VmaAllocationCreateInfo allocCI = {};
		allocCI.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

		VmaAllocationInfo allocInfo;
		CHECK_VULKAN(vmaAllocateMemoryForImage(gpuContextT<VulkanGpuContext>().allocator, m_image, &allocCI, &m_allocation, &allocInfo));
		m_memOff = allocInfo.offset;
		m_mem = allocInfo.deviceMemory;

		CHECK_VULKAN(vkBindImageMemory(gpuContextT<VulkanGpuContext>().device, m_image, m_mem, m_memOff));

		auto toInfo = baseLayoutInfo();
		static_cast<VulkanCommandBuffer&>(gpuContextT<VulkanGpuContext>().transferCb()).switchLayout(m_image, m_aspectFlags, 0, 0, desc.mipCount, desc.layerCount,
			{ VK_IMAGE_LAYOUT_UNDEFINED, 0, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT},
			{ toInfo.layout, toInfo.access, toInfo.stage }
		);

		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.viewType = (desc.layerCount == 1) ? VK_IMAGE_VIEW_TYPE_2D : VK_IMAGE_VIEW_TYPE_CUBE;
		createInfo.format = gpuContextT<VulkanGpuContext>().getVkFormat(desc.format);
		createInfo.image = m_image;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = m_aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = desc.mipCount;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = desc.layerCount;

		CHECK_VULKAN(vkCreateImageView(gpuContextT<VulkanGpuContext>().device, &createInfo, nullptr, &m_samplerView));
	}

	VulkanTexture2D::~VulkanTexture2D()
	{
		vkDestroyImageView(gpuContextT<VulkanGpuContext>().device, m_samplerView, nullptr);
		for (auto& x : m_views) {
			if (x.second) {
				vkDestroyImageView(gpuContextT<VulkanGpuContext>().device, x.second, nullptr);
			}
		}
		if (!m_isInternal) {
			vkDestroyImage(gpuContextT<VulkanGpuContext>().device, m_image, nullptr);
			vmaFreeMemory(gpuContextT<VulkanGpuContext>().allocator, m_allocation);
		}
	}

	VkImageView VulkanTexture2D::baseView()
	{
		return m_samplerView;
	}

	VkImageView VulkanTexture2D::getImageView(uint32 mip, uint32 layer)
	{
		MipLayerView wanted = { uint16(mip), uint16(layer) };
		auto iter = m_views.find(wanted);
		if (iter != m_views.end())
			return iter->second;

		VkImageView			  view;
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = gpuContextT<VulkanGpuContext>().getVkFormat(getDesc().format);
		createInfo.image = m_image;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = m_aspectFlags;
		createInfo.subresourceRange.baseMipLevel = mip;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = layer;
		createInfo.subresourceRange.layerCount = 1;

		CHECK_VULKAN(vkCreateImageView(gpuContextT<VulkanGpuContext>().device, &createInfo, nullptr, &view));
		m_views.emplace(wanted, view);
		return view;
	}

	ImageBarrierTriple VulkanTexture2D::baseLayoutInfo() const
	{
		const auto& desc = getDesc();
		ImageBarrierTriple result = {};
		if (TU_SAMPLED_IMAGE & desc.usage)
		{
			result.access |= VK_ACCESS_SHADER_READ_BIT;
			result.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			result.stage |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
		}
		if (TU_COLORATTACHMENT & desc.usage)
		{
			result.access |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			result.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			result.stage |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		}
		if (TU_DEPTHATTACHMENT & desc.usage)
		{
			result.access |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			result.layout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			result.stage |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		return result;
	}
	
	VulkanSwapchainBackbuffer::VulkanSwapchainBackbuffer(const VULKAN_CUSTOM_IMAGE_DESC& desc, VulkanGpuContext* instance, VulkanSwapchain* swapchain) :
		VulkanTexture2D(desc, instance),
		m_swapchain(swapchain)
	{
	}

	void VulkanSwapchainBackbuffer::callBindBefore(VulkanCommandBuffer* cmdBuffer)
	{
	}

	void VulkanSwapchainBackbuffer::callBindAfter(VulkanCommandBuffer* cmdBuffer)
	{
	}

}
