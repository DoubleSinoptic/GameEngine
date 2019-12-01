#pragma once
#ifndef __VULKANSWAPCHAIN_H__
#define __VULKANSWAPCHAIN_H__

#include "Core/Core.h"
#include "VulkanGpuContext.h"
#include "../Swapchain.h"
#include "Core/Config.h"
#include "VulkanSync.h"
namespace ge
{
	class VulkanSwapchain : public Swapchain
	{
		std::vector<RPtr<VulkanSemaphore>>    m_semaphores;
		std::vector<VkImage>			m_swapChainImages;
		VkFormat						m_swapChainImageFormat;
		VkExtent2D						m_swapChainExtent;
		VkSwapchainKHR					m_swapChain = VK_NULL_HANDLE;
		VkExtent2D						m_swapChainRequiredExtent;

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : availableFormats) {
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
			if (Config::instance().getValueBool(u"vsync", false)) {
				return VK_PRESENT_MODE_FIFO_KHR;
			}
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& availablePresentMode : availablePresentModes) {
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return availablePresentMode;
				}
				else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
					bestMode = availablePresentMode;
				}
			}
			/*	return VK_PRESENT_MODE_FIFO_KHR;*/
			return bestMode;
		}

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != 0xFFFFFFFF) {
				return capabilities.currentExtent;
			}
			else {
				VkExtent2D actualExtent = m_swapChainRequiredExtent;
				actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));
				return actualExtent;
			}
		}
	public:

		constexpr VkSwapchainKHR vulkanHandle() const noexcept
		{
			return m_swapChain;
		}
		

		VulkanSwapchain(VulkanGpuContext* context);
		~VulkanSwapchain();

		virtual Texture2D* getNextImage();
	};
}

#endif