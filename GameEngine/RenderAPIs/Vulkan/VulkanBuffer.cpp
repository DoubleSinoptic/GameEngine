#include "VulkanBuffer.h"

namespace ge 
{
	VulkanBuffer::VulkanBuffer(const BUFFER_DESC& desc, bool transferSrc, VulkanGpuContext* instance) :
		m_instance(instance),
		Buffer(desc),
		m_hasMapped(false),
		m_mappedBuffer(nullptr),
		m_mappedOffset(0),
		m_mappedSize(0),
		m_allocation(nullptr),
		m_directMod(false)
	{
		if (desc.memType == MT_DYNAMIC)
			m_directMod = true;
		m_size = desc.size;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage =
			((desc.usage & BU_UNIFORM) ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : 0) |
			((desc.usage & BU_VERTECES) ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : 0) |
			((desc.usage & BU_INDECES) ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : 0) |
			(!m_directMod ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0) |
			((transferSrc || (desc.usage == 0)) ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0);

		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.size = m_size;

		CHECK_VULKAN(vkCreateBuffer(m_instance->device, &bufferInfo, nullptr, &m_buffer));

		VmaAllocationCreateInfo allocCI = {};
		allocCI.requiredFlags = (m_directMod ? (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) :
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VmaAllocationInfo allocInfo;
		CHECK_VULKAN(vmaAllocateMemoryForBuffer(m_instance->allocator, m_buffer, &allocCI, &m_allocation, &allocInfo));
		m_memoryOffset = allocInfo.offset;
		m_memory = allocInfo.deviceMemory;

		CHECK_VULKAN(vkBindBufferMemory(m_instance->device, m_buffer, allocInfo.deviceMemory, allocInfo.offset))
	}

	VulkanBuffer::~VulkanBuffer()
	{
		if (m_mappedBuffer)
		{
			delete m_mappedBuffer;
			m_mappedBuffer = nullptr;
		}

		VmaAllocation all = m_allocation;
		VkBuffer buffer = m_buffer;
		VulkanGpuContext* ctx = m_instance;

		ctx->registerRelease([=]() 
		{
			vmaFreeMemory(ctx->allocator, all);
			vkDestroyBuffer(ctx->device, buffer, nullptr);
		});
	}

	void* VulkanBuffer::map(usize offset, usize size, AccessFlags access)
	{
		if (m_directMod) 
		{
			unmap();
			void* r = nullptr;
			CHECK_VULKAN(vkMapMemory(m_instance->device, m_memory, m_memoryOffset + offset, size, 0, &r));
			m_hasMapped = true;
			return (void*)r;
		}
		else 
		{
			if (access == AF_READ || access == AF_WRITE)
				throw std::runtime_error("error of map static buffer to write");

			BUFFER_DESC dsc = {};
			dsc.size = m_size;
			dsc.memType = MT_DYNAMIC;

			m_mappedOffset = offset;
			m_mappedSize = size;

			if (!m_mappedBuffer)
				m_mappedBuffer = new VulkanBuffer(dsc, true, m_instance);

			m_hasMapped = true;
			return m_mappedBuffer->map(m_mappedOffset, m_mappedSize, access);
		}
	}

	void VulkanBuffer::unmap()
	{
		if (m_directMod) 
		{
			if (m_hasMapped) 
			{
				vkUnmapMemory(m_instance->device, m_memory);
				m_hasMapped = false;
			}
		}
		else 
		{
			if (m_hasMapped) 
			{
				m_mappedBuffer->unmap();
				m_instance->copyBuffer(const_cast<ge::VulkanBuffer*>(this), m_mappedBuffer, m_mappedSize, m_mappedSize, m_mappedSize);
				//m_instance->BufferBarirer(this, m_copyBufferSize, m_copyBufferOffset);
				m_hasMapped = false;
			}
		}
	}
}

