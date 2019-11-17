#include "VulkanUniform.h"
#include "VulkanTexture2D.h"
#include "VulkanBuffer.h"
#include "VulkanSampler.h"

namespace ge 
{
	VulkanUniform::VulkanUniform(const UNIFORM_DESC& desc, VulkanGpuContext* context) :
		Uniform(desc, context)
	{
		auto data = context->createSet(desc);
		m_set = data.set;
		m_pool = data.pool;
	}

	VulkanUniform::~VulkanUniform()
	{
		vkFreeDescriptorSets(gpuContextT<VulkanGpuContext>().device, m_pool, 1, &m_set);
	}
	void VulkanUniform::setSampler(Sampler* sampler, int32 slot)
	{
		VkDescriptorImageInfo bf = {};
		bf.sampler = static_cast<VulkanSampler*>(sampler)->vulkanHandle();

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_set;
		descriptorWrite.dstBinding = slot;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = nullptr;
		descriptorWrite.pImageInfo = &bf;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(gpuContextT<VulkanGpuContext>().device, 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanUniform::setTexture(Texture2D* texture, int32 slot)
	{
		VkDescriptorImageInfo bf = {};
		bf.imageLayout = static_cast<VulkanTexture2D*>(texture)->baseLayout();
		bf.imageView = static_cast<VulkanTexture2D*>(texture)->baseView();
	
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_set;
		descriptorWrite.dstBinding = slot;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = nullptr;
		descriptorWrite.pImageInfo = &bf;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(gpuContextT<VulkanGpuContext>().device, 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanUniform::setBuffer(Buffer* buffer, int32 slot)
	{
		VkDescriptorBufferInfo bf = {};
		bf.buffer = static_cast<VulkanBuffer*>(buffer)->vulkanHandle();
		bf.range = static_cast<VulkanBuffer*>(buffer)->getDesk().size;
		bf.offset = 0;
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_set;
		descriptorWrite.dstBinding = slot;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bf;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(gpuContextT<VulkanGpuContext>().device, 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanUniform::setBuffer(Buffer* buffer, usize offset, usize length, int32 slot)
	{
		VkDescriptorBufferInfo bf = {};
		bf.buffer = static_cast<VulkanBuffer*>(buffer)->vulkanHandle();
		bf.range = length;
		bf.offset = offset;
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_set;
		descriptorWrite.dstBinding = slot;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bf;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(gpuContextT<VulkanGpuContext>().device, 1, &descriptorWrite, 0, nullptr);
	}

}
