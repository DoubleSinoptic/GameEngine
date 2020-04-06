#include "VulkanFrameBuffer.h"
#include "VulkanTexture2D.h"
namespace ge 
{
	VulkanFramebuffer::VulkanFramebuffer(const FRAMEBUFFER_DESC& d, VulkanGpuContext* context) :
		Framebuffer(d, context)
	{

		usize					 m_colorAttachments = 0;
		usize					 m_attachments = 0;
		VkAttachmentDescription	 m_descriptions[32];
		VkAttachmentReference	 m_references[32];
		VkImageView				 m_views[32];

		VkImageView				 m_detphView;
		VkAttachmentDescription	 m_depthDesc = {};
		VkAttachmentReference	 m_depthRef = {};

		VkSubpassDescription	 m_subpassDesc = {};
		VkSubpassDependency		 m_dependencies[2];

		VkRenderPassCreateInfo   m_renderPassCreateionInfo = {};

		uint32 m_h = d.heigth;
		uint32 m_w = d.width;
		for (uint32 i = 0; i < d.colorAttachmentsNum; i++)
		{
			VkAttachmentDescription& attachmentDesc = m_descriptions[i];
			attachmentDesc.flags = 0;
			attachmentDesc.format = static_cast<VulkanTexture2D*>(d.colorAttachments[i].texture)->vulkanFormat();
			attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDesc.initialLayout = static_cast<VulkanTexture2D*>(d.colorAttachments[i].texture)->baseLayout();
			attachmentDesc.finalLayout = static_cast<VulkanTexture2D*>(d.colorAttachments[i].texture)->baseLayout();

			VkAttachmentReference& ref = m_references[i];
			ref.attachment = i;
			ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			m_views[i] = static_cast<VulkanTexture2D*>(d.colorAttachments[i].texture)->getImageView(
				d.colorAttachments[i].mip,
				d.colorAttachments[i].layer
			);
			m_colorAttachments++;
			m_attachments++;
		}

		if (d.stencil.texture)
			throw std::runtime_error("stencill test not supported");

		if (d.depth.texture) {
			VkAttachmentDescription& attachmentDesc = m_descriptions[m_attachments];
			attachmentDesc.flags = 0;
			attachmentDesc.format = static_cast<VulkanTexture2D*>(d.depth.texture)->vulkanFormat();
			attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
			attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDesc.initialLayout = static_cast<VulkanTexture2D*>(d.depth.texture)->baseLayout();
			attachmentDesc.finalLayout = static_cast<VulkanTexture2D*>(d.depth.texture)->baseLayout();

			VkAttachmentReference& ref = m_depthRef;
			ref.attachment = m_attachments;
			ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			m_views[m_attachments] = static_cast<VulkanTexture2D*>(d.depth.texture)->getImageView(
				d.depth.mip,
				d.depth.layer
			);
			m_attachments++;
		}

		m_subpassDesc.flags = 0;
		m_subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		m_subpassDesc.colorAttachmentCount = m_colorAttachments;
		m_subpassDesc.inputAttachmentCount = 0;
		m_subpassDesc.pInputAttachments = nullptr;
		m_subpassDesc.preserveAttachmentCount = 0;
		m_subpassDesc.pPreserveAttachments = nullptr;
		m_subpassDesc.pResolveAttachments = nullptr;

		if (m_colorAttachments > 0)
			m_subpassDesc.pColorAttachments = m_references;

		if (d.depth.texture)
			m_subpassDesc.pDepthStencilAttachment = &m_depthRef;

		m_dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		m_dependencies[0].dstSubpass = 0;
		m_dependencies[0].srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		m_dependencies[0].dstStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		m_dependencies[0].srcAccessMask = 0;
		m_dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
		m_dependencies[0].dependencyFlags = 0;

		m_dependencies[1].srcSubpass = 0;
		m_dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		m_dependencies[1].srcStageMask = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		m_dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		m_dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
			VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT;
		m_dependencies[1].dstAccessMask = 0;
		m_dependencies[1].dependencyFlags = 0;

		m_renderPassCreateionInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		m_renderPassCreateionInfo.pNext = nullptr;
		m_renderPassCreateionInfo.flags = 0;
		m_renderPassCreateionInfo.attachmentCount = m_attachments;
		m_renderPassCreateionInfo.pAttachments = m_descriptions;
		m_renderPassCreateionInfo.subpassCount = 1;
		m_renderPassCreateionInfo.pSubpasses = &m_subpassDesc;
		m_renderPassCreateionInfo.dependencyCount = 2;
		m_renderPassCreateionInfo.pDependencies = m_dependencies;

		CHECK_VULKAN(vkCreateRenderPass(gpuContextT<VulkanGpuContext>().device, &m_renderPassCreateionInfo, nullptr, &m_renderPass));

		VkFramebufferCreateInfo	info = {};
		info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		info.renderPass = m_renderPass;
		info.attachmentCount = m_attachments;
		info.pAttachments = m_views;
		info.width = d.width;
		info.height = d.heigth;
		info.layers = 1;

		CHECK_VULKAN(vkCreateFramebuffer(gpuContextT<VulkanGpuContext>().device, &info, nullptr, &m_framebuffer));
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		if (m_framebuffer) {
			vkDestroyFramebuffer(gpuContextT<VulkanGpuContext>().device, m_framebuffer, nullptr);
		}
		if (m_renderPass) {
			vkDestroyRenderPass(gpuContextT<VulkanGpuContext>().device, m_renderPass, nullptr);
		}
	}


}
