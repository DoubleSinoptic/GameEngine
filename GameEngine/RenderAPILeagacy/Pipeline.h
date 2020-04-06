#pragma once
#ifndef __PIPELINE_H__
#define __PIPELINE_H__

#include "Core/Core.h"
#include "GpuResource.h"
#include "Texture2D.h"
#include "Uniform.h"
#include "ShaderModule.h"

namespace ge
{
	enum BlendOp
	{
		BO_ADD,
		BO_MUL,
		BO_DIV
	};

	enum BlendMod
	{
		BM_ZERO,
		BM_ONE,
		BM_SRC_ALPHA,
		BM_DST_ALPHA,
		BM_ONE_MINUS_SRC_ALPHA,
		BM_ONE_MINUS_DST_ALPHA
	};

	struct BLEND_STATE
	{
		bool enabled = false;
		BlendMod src = BM_ONE;
		BlendMod dst = BM_ZERO;
		BlendOp  op = BO_ADD;

		BlendMod srcAlpha = BM_ONE;
		BlendMod dstAlpha = BM_ZERO;
		BlendOp  opAlpha = BO_ADD;
	};


	struct VERTEX_INPUT_ATTRIBUTE
	{
		uint32		binding = 0;
		uint32	    location = 0;
		PixelFormat format = PF_R32G32B32_FLOAT;
		uint32		offset = 0;
	};

	struct VERTEX_INPUT_BINDING
	{
		uint32  binding = 0;
		usize   stride = sizeof(12);
		bool    perInstanceRate = false;
	};

	enum FrontFace
	{
		FF_UNDEFINED,
		FF_CCW,
		FF_CW
	};

	enum CullFace
	{
		CF_BACK,
		CF_FRONT,
		CF_NONE
	};

	enum DepthOp
	{
		DO_L,
		DO_LEQUAL,
		DO_G,
		DO_GEQUAL
	};

	struct PIPELINE_DESC
	{
		VERTEX_INPUT_BINDING    bindings[16];
		VERTEX_INPUT_ATTRIBUTE  attributes[16];
		usize					enabledBindings = 0;
		usize					enabledAttributes = 0;
		UNIFORM_DESC			uniforms[32];
		usize					enabledUniforms = 0;
		BLEND_STATE				blendStates[32];
		usize					enabledBlendStates = 0;
		RPtr<ShaderModule>		vertex;
		RPtr<ShaderModule>		fragment;
		RPtr<ShaderModule>		geometry;
		FrontFace				frontFace = FF_UNDEFINED;
		CullFace				cullFace = CF_NONE;
		bool					depthTest = false;
		DepthOp					depthOp = DO_LEQUAL;
		float					znear = 0.0;
		float					zfar = 1.0;
	};

	class Pipeline : public GpuResource
	{
		PIPELINE_DESC m_desc;
	public:
		virtual ~Pipeline() = default;

		Pipeline(const PIPELINE_DESC& desc, GpuContext* context) noexcept :
			m_desc(desc),
			GpuResource(context)
		{}

		constexpr const PIPELINE_DESC& getDesk() const noexcept
		{
			return m_desc;
		}
	};
}

#endif