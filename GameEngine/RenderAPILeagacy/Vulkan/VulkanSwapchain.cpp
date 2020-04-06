#include "VulkanSwapchain.h"
#include "VulkanGpuContext.h"

namespace ge 
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	VulkanSwapchain::VulkanSwapchain(VulkanGpuContext* context) :
		Swapchain(context),
		m_nextImageIndex(0)
	{
		VulkanGpuContext* m_instance = context;
		for (auto& c : m_semaphores) {
			c = instance->CreateSemaphore();
		}

		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_instance->physicalDevice, m_instance->surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_instance->physicalDevice, m_instance->surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_instance->physicalDevice, m_instance->surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_instance->physicalDevice, m_instance->surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_instance->physicalDevice, m_instance->surface, &presentModeCount, details.presentModes.data());
		}

		SwapChainSupportDetails& swapChainSupport = details;

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_instance->surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		std::vector<uint32_t>	results;
		std::set<uint32_t>		fams = { context->m_graphicsFamily, context->m_transportFamily, context->m_presentFamily ,context->m_computeFamily };
		for (auto& x : fams)
			results.push_back(x);

		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = results.size();
		createInfo.pQueueFamilyIndices = results.data();
		createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		CHECK_VULKAN(vkCreateSwapchainKHR(context->device, &createInfo, nullptr, &m_swapChain));

		vkGetSwapchainImagesKHR(context->device, m_swapChain, &imageCount, nullptr);
		m_swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(context->device, m_swapChain, &imageCount, m_swapChainImages.data());

		m_swapChainImageFormat = surfaceFormat.format;
		m_swapChainExtent = extent;


		for (USize i = 0; i < m_swapChainImages.size(); i++) {
			Texture* tex = new VulkanTexture(m_instance, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, m_swapChainImages[i], w, h, m_swapChainImageFormat);
			IdTextures.emplace(i, tex);
			texturesId.emplace(tex, i);
			VkSemaphore sema;
			VkSemaphoreCreateInfo inf = {};
			inf.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			CHECK_VULKAN(vkCreateSemaphore(m_instance->device, &inf, nullptr, &sema));
			semaki.emplace(tex, sema);
		}
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
	}

	Texture2D* VulkanSwapchain::getNextImage()
	{

		vkAcquireNextImageKHR(gpuContextT<VulkanGpuContext>().device, m_swapchain, UINT64_MAX, );

		return nullptr;
	}

}
