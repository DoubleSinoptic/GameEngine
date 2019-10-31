#include "VulkanPipeline.h"
namespace ge 
{
	VulkanPipeline::VulkanPipeline(const PIPELINE_DESC& desc, VulkanGpuContext* context) :
		m_instance(context),
		Pipeline(desc)
	{
		VkDescriptorSetLayout descirptorSetsLayouts[16];
		for (usize i = 0; i < desc.enabledUniforms; i++) {
			descirptorSetsLayouts[i] = m_instance->getLayoutFromDesc(desc.uniforms[i]);
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = desc.enabledUniforms;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pSetLayouts = descirptorSetsLayouts;

		CHECK_VULKAN(vkCreatePipelineLayout(m_instance->device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));
	}

	VulkanPipeline::~VulkanPipeline()
	{
		std::unordered_map<VkSizedRenderPass, VkPipeline, VkSizedRenderPassHash, VkSizedRenderPassEqual> pipelines = std::move(m_pipelines);
		VulkanGpuContext* instance = m_instance;
		VkPipelineLayout layout = m_pipelineLayout;

		instance->registerRelease([=]() {
			for (auto& x : pipelines) {
				if (x.second)
					vkDestroyPipeline(instance->device, x.second, nullptr);
			}
			vkDestroyPipelineLayout(instance->device, layout, nullptr);
		});
	}


	VkBlendFactor getVkBlendFactor(BlendMod fac)
	{
		switch (fac)
		{
		case BM_ZERO:
			return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
			break;
		case BM_ONE:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE;
			break;
		case BM_SRC_ALPHA:
			return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case BM_DST_ALPHA:
			return VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case BM_ONE_MINUS_SRC_ALPHA:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case BM_ONE_MINUS_DST_ALPHA:
			return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
		default:
			geAssertFalse("error");
			break;
		}
		geAssertFalse("error");
	}

	VkBlendOp getVkBlendOp(BlendOp op)
	{
		switch (op)
		{
		case BO_ADD:
			return VkBlendOp::VK_BLEND_OP_ADD;
			break;
		case BO_MUL:
			geAssertFalse("error");
			break;
		case BO_DIV:
			geAssertFalse("error");
			break;
		default:
			geAssertFalse("error");
			break;
		}
		geAssertFalse("error");
	}

	VkPipeline VulkanPipeline::get(VkRenderPass renerPass, uint32 w, uint32 h)
	{
		auto iter = m_pipelines.find({ renerPass, w, h });
		if (iter != m_pipelines.end())
			return iter->second;

		const auto& desc = getDesk();

		usize sindex = 0;
		VkPipelineShaderStageCreateInfo shaderStages[3] = {};
	/*	if (desc.vertex)
			shaderStages[sindex++] = static_cast<VulkanShaderModule*>(desc.vertex.Get())->vertShaderStageInfo;
		if (desc.geometry)
			shaderStages[sindex++] = static_cast<VulkanShaderModule*>(desc.geometry.Get())->vertShaderStageInfo;
		if (desc.fragment)
			shaderStages[sindex++] = static_cast<VulkanShaderModule*>(desc.fragment.Get())->vertShaderStageInfo;*/

		VkVertexInputAttributeDescription	m_vertexInputAttributeDescs[32];
		VkVertexInputBindingDescription		m_vertexBindingDescs[32];
		
		usize m_vertexBindingDescsCount = desc.enabledBindings;
		for (usize i = 0; i < m_vertexBindingDescsCount; i++) {
			auto& x = desc.bindings[i];
			auto& bindingDescription = m_vertexBindingDescs[i];
			bindingDescription.binding = x.binding;
			bindingDescription.stride = x.stride;
			bindingDescription.inputRate = x.perInstanceRate ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
		}
		usize m_vertexInputAttributeDescsCount = desc.enabledAttributes;
		for (usize i = 0; i < m_vertexInputAttributeDescsCount; i++) {
			auto& x = desc.attributes[i];
			auto& bindingDescription = m_vertexInputAttributeDescs[i];
			bindingDescription.binding = x.binding;
			bindingDescription.format = m_instance->getVkFormat(x.format);
			bindingDescription.location = x.location;
			bindingDescription.offset = x.offset;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = m_vertexBindingDescsCount;
		vertexInputInfo.vertexAttributeDescriptionCount = m_vertexInputAttributeDescsCount;
		vertexInputInfo.pVertexAttributeDescriptions = m_vertexInputAttributeDescs;
		vertexInputInfo.pVertexBindingDescriptions = m_vertexBindingDescs;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)w;
		viewport.height = (float)h;
		viewport.minDepth = desc.znear;
		viewport.maxDepth = desc.zfar;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { w, h };

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		switch (desc.cullFace)
		{
		case CF_BACK:
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			break;
		case CF_FRONT:
			rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
			break;
		case CF_NONE:
			rasterizer.cullMode = VK_CULL_MODE_NONE;
			break;
		default:
			geAssertFalse("Invalid logic.");
		}

		rasterizer.frontFace = (desc.frontFace == FF_CW) ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachments[32] = {};
		for (usize i = 0; i < desc.enabledBlendStates; i++) 
		{
			VkPipelineColorBlendAttachmentState& colorBlendAttachment = colorBlendAttachments[i];
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = desc.blendStates[i].enabled;
			if (!desc.blendStates[i].enabled)
				continue;
			colorBlendAttachment.srcColorBlendFactor = getVkBlendFactor(desc.blendStates[i].src);
			colorBlendAttachment.dstColorBlendFactor = getVkBlendFactor(desc.blendStates[i].dst);
			colorBlendAttachment.colorBlendOp = getVkBlendOp(desc.blendStates[i].op);
			colorBlendAttachment.srcAlphaBlendFactor = getVkBlendFactor(desc.blendStates[i].srcAlpha);
			colorBlendAttachment.dstAlphaBlendFactor = getVkBlendFactor(desc.blendStates[i].dstAlpha);
			colorBlendAttachment.alphaBlendOp = getVkBlendOp(desc.blendStates[i].opAlpha);
		}

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = desc.enabledBlendStates;
		colorBlending.pAttachments = colorBlendAttachments;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		if (desc.depthTest) 
		{
			depthStencil.depthTestEnable = VK_TRUE;
			depthStencil.depthWriteEnable = VK_TRUE;
			switch (desc.depthOp)
			{
			case DO_L:
				depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
				break;
			case DO_LEQUAL:
				depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
				break;
			case DO_G:
				depthStencil.depthCompareOp = VK_COMPARE_OP_GREATER;
				break;
			case DO_GEQUAL:
				depthStencil.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
				break;
			default:
				geAssertFalse("Invalid code logic.");
				break;
			}
			depthStencil.depthBoundsTestEnable = VK_FALSE;
			depthStencil.minDepthBounds = desc.znear;
			depthStencil.maxDepthBounds = desc.zfar;
			depthStencil.stencilTestEnable = VK_FALSE;
			depthStencil.front = {};
			depthStencil.back = {};
		}
		else {
			depthStencil.stencilTestEnable = VK_FALSE;
			depthStencil.front = {};
			depthStencil.back = {};
			depthStencil.depthTestEnable = VK_FALSE;
			depthStencil.depthWriteEnable = VK_FALSE;
		}


		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = sindex;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = m_pipelineLayout;
		pipelineInfo.renderPass = renerPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.pDepthStencilState = &depthStencil;
		VkPipeline pipe;
		CHECK_VULKAN(vkCreateGraphicsPipelines(m_instance->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipe));

		m_pipelines[{ renerPass, w, h}] =  pipe;
		return pipe;
	}
}
