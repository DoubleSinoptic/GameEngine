#include "VulkanSync.h"
namespace ge 
{
	VulkanSemaphore::VulkanSemaphore(VulkanGpuContext* context) :
		GpuResource(context)
	{
		VkSemaphoreCreateInfo inf = {};
		inf.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		CHECK_VULKAN(vkCreateSemaphore(gpuContextT<VulkanGpuContext>().device, &inf, nullptr, &m_semaphore));
	}
	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(gpuContextT<VulkanGpuContext>().device, m_semaphore, nullptr);
	}

	VulkanFence::VulkanFence(VulkanGpuContext* context, bool signaled) :
		GpuResource(context)
	{
		VkFenceCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		CHECK_VULKAN(vkCreateFence(gpuContextT<VulkanGpuContext>().device, &info, nullptr, &m_fence));
	}

	VulkanFence::~VulkanFence()
	{
		vkDestroyFence(gpuContextT<VulkanGpuContext>().device, m_fence, nullptr);
	}

	bool VulkanFence::wait(uint64 nan)
	{
		VkResult r = vkWaitForFences(gpuContextT<VulkanGpuContext>().device, 1, &m_fence, VK_TRUE, nan);
		if (r == VK_SUCCESS)
			return true;
		else if (r == VK_TIMEOUT)
			return false;
		CHECK_VULKAN(r);
	}

	void VulkanFence::reset()
	{
		CHECK_VULKAN(vkResetFences(gpuContextT<VulkanGpuContext>().device, 1, &m_fence));
	}

}
