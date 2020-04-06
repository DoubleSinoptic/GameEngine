#include "VulkanGpuContext.h"
#include "VulkanTexture2D.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeline.h"
#include "VulkanUniform.h"
#include "VulkanFrameBuffer.h"
#include "VulkanShaderModule.h"
#include "VulkanSampler.h"
#include "Core/Debug.h"

namespace ge
{
	const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	#ifdef _DEBUG
		VK_EXT_DEBUG_MARKER_EXTENSION_NAME
	#endif	
	};

	const std::vector<const char*> extensions = {
	#ifdef _DEBUG
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	#endif	
			VK_KHR_SURFACE_EXTENSION_NAME,
			"VK_KHR_win32_surface"
	};
	const std::vector<const char*> validationLayers = {
	#ifdef _DEBUG
		   "VK_LAYER_LUNARG_standard_validation"
	#endif
	};

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		Debug::log("Vulkan: {0}", pCallbackData->pMessage);
	
		/*if (
			(String(pCallbackData->pMessage).Token("Type mismatch on descriptor slot") != -1) ||
			(String(pCallbackData->pMessage).Token("vkWaitForFences called for fence") != -1) ||
			(String(pCallbackData->pMessage).Token("OBJ ERROR :") != -1)
			) {

			throw std::runtime_error("ebah");
		}
*/
		return VK_FALSE;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}


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



	VulkanCommandPool* VulkanGpuContext::getCommandPool(const COMMAND_BUFFER_DESC& desc)
	{
		auto iter = m_commandPools.find(desc);
		if (iter == m_commandPools.end())
		{
			VulkanCommandPool* cmdPool = new VulkanCommandPool(this, desc);
			m_commandPools.emplace(desc, cmdPool);
			return cmdPool;
		}
		return iter->second;
	}

	VulkanGpuContext::VulkanGpuContext(const VulkanSurfaceCreator& surfaceCreator)
	{
		Debug::log("Vulkan: VulkanGpuContext::VulkanGpuContext");
		for (auto& x : translatedFormats)
		{
			m_toGeFormats.emplace(x.vkFormat, x.geFormat);
			m_toVkFormats.emplace(x.geFormat, x.vkFormat);
		}

		{
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "(null)";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "(null)";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledLayerCount = validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.ppEnabledExtensionNames = extensions.data();
			createInfo.enabledExtensionCount = extensions.size();
			CHECK_VULKAN(vkCreateInstance(&createInfo, nullptr, &instance));
	
			CHECK_VULKAN(surfaceCreator(surface, this));

			if (!surface)
				throw std::runtime_error("error of acquire surface");
		}

#ifdef _DEBUG
		{
			VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = debugCallback;
			createInfo.pUserData = nullptr; // Optional

			CHECK_VULKAN(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger));
		}
#endif
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		devices.resize(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (auto& x : devices)
		{
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(x, &deviceProperties);
			physicalDevice = x;
			Debug::log("Vulkan: device detected api: {0}, drv-ver: {1}, {2}", deviceProperties.apiVersion, deviceProperties.driverVersion, deviceProperties.deviceName);
		}

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
				m_computeFamily = i;
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				m_graphicsFamily = i;
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
				m_transportFamily = i;
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
			if (queueFamily.queueCount > 0 && presentSupport)
				m_presentFamily = i;
			i++;
		}


		if (m_graphicsFamily == UINT32_MAX || m_presentFamily == UINT32_MAX || m_transportFamily == UINT32_MAX || m_computeFamily == UINT32_MAX)
			geAssertFalse("failed to find graphics famaly");

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { m_graphicsFamily, m_presentFamily, m_transportFamily, m_computeFamily };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
		vkGetPhysicalDeviceProperties(physicalDevice, &props);

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = queueCreateInfos.size();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = deviceExtensions.size();
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		CHECK_VULKAN(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

		O_vkDebugMarkerSetObjectNameEXT = (decltype(O_vkDebugMarkerSetObjectNameEXT))vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT");

		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = physicalDevice;
		allocatorInfo.device = device;
		CHECK_VULKAN(vmaCreateAllocator(&allocatorInfo, &allocator));

		Debug::log("Vulkan: legacy interface inited.");
	}

	VulkanGpuContext::~VulkanGpuContext()
	{
		Debug::log("Vulkan: VulkanGpuContext::~VulkanGpuContext");
		vkDeviceWaitIdle(device);

		for (auto& x : m_commandPools) {
			if (x.second)
				delete x.second;
		}

		if (allocator) 
		{
			vmaDestroyAllocator(allocator);
			allocator = nullptr;
		}

		for (auto& x : m_descriptorSetLayouts)
			vkDestroyDescriptorSetLayout(device, x.second, nullptr);

		for (auto& x : m_pools) 
			vkDestroyDescriptorPool(device, x, nullptr);

		if (device) 
		{
			vkDestroyDevice(device, nullptr);
			device = VK_NULL_HANDLE;
		}
#ifdef _DEBUG
		if (debugMessenger) 
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
			debugMessenger = VK_NULL_HANDLE;
		}
#endif
		if (instance)
		{
			vkDestroyInstance(instance, nullptr);
			instance = nullptr;
		}
	}

	VulkanGpuContext::DesckAlloc VulkanGpuContext::createSet(const UNIFORM_DESC& desc)
	{
		VkDescriptorSet		  set;
		VkDescriptorSetLayout lay = getLayoutFromDesc(desc);

		auto alloc = [&]()
		{
			VkDescriptorPoolSize poolSize = {};
			poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSize.descriptorCount = 1 << 11;

			VkDescriptorPoolSize poolSize1 = {};
			poolSize1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSize1.descriptorCount = 1 << 5;

			VkDescriptorPoolSize kea[] = { poolSize, poolSize1 };
			VkDescriptorPool pool;
			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 2;
			poolInfo.pPoolSizes = kea;
			poolInfo.maxSets = 1 << 15;
			poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			CHECK_VULKAN(vkCreateDescriptorPool(device, &poolInfo, nullptr, &pool));
			Debug::log("Vulkan: allocated new descriptor pool.");
			m_pools.push_back(pool);
		};

		if (!m_pools.size()) {
			alloc();
		}
		bool reallocSigned = false;

		for (auto x = m_pools.rbegin(); x != m_pools.rend(); x++) {
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = *x;
			allocInfo.descriptorSetCount = 1;
			allocInfo.pSetLayouts = &lay;
			if (vkAllocateDescriptorSets(device, &allocInfo, &set) == VK_SUCCESS)
				return  { set, *x };
		}
		alloc();
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_pools.back();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &lay;
		CHECK_VULKAN(vkAllocateDescriptorSets(device, &allocInfo, &set));
		return { set, m_pools.back() };
	}

	uint32 VulkanGpuContext::getQueueFamailyIndex(QueueType type) const
	{
		switch (type)
		{
		case ge::QT_TRANSFER:
			return m_transportFamily;
			break;
		case ge::QT_GRAPHICS:
			return m_graphicsFamily;
			break;
		case ge::QT_COMPUTE:
			return m_computeFamily;
			break;
		case ge::QT_PRESENT:
			return m_presentFamily;
		default:
			break;
		}
		return uint32();
	}

	VkQueue VulkanGpuContext::getQueue(QueueType type) const
	{
		VkQueue queue;
		vkGetDeviceQueue(device, getQueueFamailyIndex(type), 0, &queue);
		return queue;
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

	VkDescriptorSetLayout VulkanGpuContext::getLayoutFromDesc(const UNIFORM_DESC& a)
	{
		auto iter = m_descriptorSetLayouts.find(a);
		if (iter != m_descriptorSetLayouts.end())
			return iter->second;

		VkDescriptorSetLayoutCreateInfo fon = {};
		fon.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		usize index = 0;
		VkDescriptorSetLayoutBinding bindigs[16];
		memset((void*)bindigs, 0, sizeof(bindigs));
		while (true)
		{
			const DESCRIPTOR_DESC& b = a.resources[index];
			if (b.type == UT_NONE)
				break;
			VkDescriptorSetLayoutBinding& binding = bindigs[index];
			binding.binding = index;
			binding.stageFlags =
				((b.stages & PS_VERTEX) ? VK_SHADER_STAGE_VERTEX_BIT : 0) |
				((b.stages & PS_PIXEL) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0);
			binding.descriptorCount = 1;
			switch (b.type) {
			case UniformType::UT_SAMPLER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
				break;
			case UniformType::UT_BUFFER:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				break;
			case UniformType::UT_TEXTURE:
				binding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
				break;
			default:
				geAssertFalse("Invalid logic.");
			}
			binding.descriptorCount = 1;
			index++;
		}
		VkDescriptorSetLayout newObject;
		fon.bindingCount = index;
		fon.pBindings = bindigs;
		CHECK_VULKAN(vkCreateDescriptorSetLayout(device, &fon, nullptr, &newObject));
		m_descriptorSetLayouts.emplace(a, newObject);
		return newObject;
	}
	void VulkanGpuContext::submit(CommandBuffer* cmdBuffer)
	{
	}
	Uniform* VulkanGpuContext::createUniform(const UNIFORM_DESC& desc)
	{
		return new VulkanUniform(desc, this);
	}
	Texture2D* VulkanGpuContext::createTexture2D(const TEXTURE2D_DESC& desc)
	{
		return new VulkanTexture2D(desc, this);
	}
	Buffer* VulkanGpuContext::createBuffer(const BUFFER_DESC& desc)
	{
		return new VulkanBuffer(desc, false, this);
	}
	Sampler* VulkanGpuContext::createSampler(const SAMPLER_DESC& desc)
	{
		return new VulkanSampler(desc, this);
	}
	CommandBuffer* VulkanGpuContext::createCommandBuffer(const COMMAND_BUFFER_DESC& desc)
	{
		VulkanCommandPool* m_pool = getCommandPool(desc);
		return m_pool->allocate();
	}
	Pipeline* VulkanGpuContext::createPipeline(const PIPELINE_DESC& desc)
	{
		return new VulkanPipeline(desc, this);
	}
	Framebuffer* VulkanGpuContext::createFramebuffer(const FRAMEBUFFER_DESC& desc)
	{
		return new VulkanFramebuffer(desc, this);
	}
	usize VulkanGpuContext::allignUniform(usize length) const
	{
		return usize();
	}
}
