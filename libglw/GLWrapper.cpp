#include "GLWrapper.h"
namespace gl
{
	impl::T_Clear Clear;
	namespace impl
	{
		T_Clear& Stencil(T_Clear& clear, int32_t value)
		{
			ClearStencil(value);
			return clear;
		}
	}
	impl::T_Enabler Enable;
	impl::T_Disabler Disable;
	namespace impl
	{
		T_Enabler & T_Enabler::operator<<(Capabilities cap)
		{
			operator()(cap);
			return *this;
		}
		void T_Enabler::operator()(Capabilities cap)
		{
			glEnable(cap);
		}
		void T_Enabler::operator()(Capabilities cap, GLuint index)
		{
			glEnablei(cap, index);
		}
		bool T_Enabler::is(Capabilities cap)
		{
			return glIsEnabled(cap);
		}
		T_Disabler & T_Disabler::operator<<(Capabilities cap)
		{
			operator()(cap);
			return *this;
		}
		void T_Disabler::operator()(Capabilities cap)
		{
			glDisable(cap);
		}
		void T_Disabler::operator()(Capabilities cap, GLuint index)
		{
			glDisablei(cap, index);
		}
	}

}