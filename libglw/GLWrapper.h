#pragma once
#include "GLClass.h"

namespace gl
{

#pragma region Clear functions
	///Color
	template <typename ...T>
	inline void ClearColor(T... value)
	{
		glm::vec4 v(std::forward<T>(value)...);
		glClearColor(v.r, v.g, v.b, v.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	///Color simplifications
	template <> 
	inline void ClearColor<float, float, float, float>(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	template <> 
	inline void ClearColor<glm::vec4>(glm::vec4 v)
	{
		glClearColor(v.r, v.g, v.b, v.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	///Depth
	inline void ClearDepth(float value)
	{
		glClearDepth(value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	///Accum
	template <typename ...T>
	inline void ClearAccum(T... value)
	{
		glm::vec4 v(std::forward<T...>(rgba)...);
		glClearAccum(v.r, v.g, v.b, v.a);
		glClear(GL_ACCUM_BUFFER_BIT);
	}
	///Accum simplifications
	template <>
	inline void ClearAccum<float, float, float, float>(float r, float g, float b, float a)
	{
		glClearAccum(r, g, b, a);
		glClear(GL_ACCUM_BUFFER_BIT);
	}
	template <>
	inline void ClearAccum<glm::vec4>(glm::vec4 v)
	{
		glClearAccum(v.r, v.g, v.b, v.a);
		glClear(GL_ACCUM_BUFFER_BIT);
	}
	///Stencil
	inline void ClearStencil(int32_t value)
	{
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}
	namespace impl {
		struct T_Clear {};
		template <typename ...T>
		T_Clear& Color(T_Clear& clear, T... rgba)
		{
			ClearColor(std::forward<T...>(rgba)...);
			return clear;
		}
		template <typename T_Float>
		T_Clear& Depth(T_Clear& clear, T_Float value)
		{
			ClearDepth(value);
			return clear;
		}
		template <typename ...T>
		T_Clear& Accum(T_Clear& clear, T... value)
		{
			ClearAccum(std::forward<T...>(rgba)...);
			return clear;
		}
		T_Clear& Stencil(T_Clear& clear, int32_t value);
	}
	extern impl::T_Clear Clear;

#pragma endregion

#pragma region Enable/Disable
	enum Capabilities
	{
		AlphaTest = GL_ALPHA_TEST,
		AutoNormal = GL_AUTO_NORMAL,
		Blend = GL_BLEND,
		CullFace = GL_CULL_FACE,
		DepthTest = GL_DEPTH_TEST,
		Multisample = GL_MULTISAMPLE,
		ScissorTest = GL_SCISSOR_TEST,
		StencilTest = GL_STENCIL_TEST,
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		TextureCubeMap= GL_TEXTURE_CUBE_MAP,
		TextureCubeMapSeamless = GL_TEXTURE_CUBE_MAP_SEAMLESS
	};
	namespace impl
	{
		class T_Enabler
		{
		public:
			T_Enabler& operator <<(Capabilities cap);
			void operator()(Capabilities cap);
			void operator()(Capabilities cap, GLuint index);
			bool is(Capabilities cap);
		};
		class T_Disabler
		{
		public:
			T_Disabler& operator <<(Capabilities cap);
			void operator()(Capabilities cap);
			void operator()(Capabilities cap, GLuint index);
		};
	}
	extern impl::T_Enabler Enable;
	extern impl::T_Disabler Disable;
#pragma endregion

#pragma region n

#pragma endregion 
}