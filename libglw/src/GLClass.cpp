#include <libglw/GLClass.h>
#include <libglw/GLImpl_Helper>
namespace gl
{
	bool Object::m_auto_inst=false;
	Object::Object()
	{
		m_id = 0;
	}
	Object::Object(GLuint id)
	{
		m_id = id;
	}
	Object::~Object()
	{

	}
	
	void Object::swap(Object & obj)
	{
		using std::swap;
		swap(obj.m_id, m_id);
	}
	void Object::setID(GLuint id)
	{
		m_id = id;
	}
	Sampler::Sampler() : Object()
	{
		if (Object::GetAutoInstantiate())
			instantiate();
	}
	Sampler::~Sampler()
	{
		destroy();
	}
	void Sampler::bind() const
	{
		glBindSampler(0, id());
	}
	void Sampler::bindTo(GLuint activeTexture)
	{
		glBindSampler(activeTexture, id());
	}
	void Sampler::unbind() const
	{
		glBindSampler(0, 0);
	}
	void Sampler::setParameter(GLenum paramName, int value)
	{
		glSamplerParameteri(id(), paramName, value);
	}
	void Sampler::setParameter(GLenum paramName, float value)
	{
		glSamplerParameterf(id(), paramName, value);
	}
	void Sampler::setFiltering(Filter global_filtering)
	{
		setMinFiltering(global_filtering);
		setMagFiltering(global_filtering);
	}
	void Sampler::setFiltering(Filter global_filtering, Filter mipmap_filtering)
	{
		setMinFiltering(global_filtering, mipmap_filtering);
		setMagFiltering(global_filtering, mipmap_filtering);
	}
	void Sampler::setMinFiltering(Filter normal_filtering)
	{
		setParameter(GL_TEXTURE_MIN_FILTER, normal_filtering);
	}
	void Sampler::setMinFiltering(Filter normal_filtering, Filter mipmap_filtering)
	{
		setParameter(GL_TEXTURE_MIN_FILTER, 0x2700 + (normal_filtering == Linear) + (mipmap_filtering == Linear));
	}
	void Sampler::setMagFiltering(Filter normal_filtering)
	{
		setParameter(GL_TEXTURE_MAG_FILTER, normal_filtering);
	}
	void Sampler::setMagFiltering(Filter normal_filtering, Filter mipmap_filtering)
	{
		setParameter(GL_TEXTURE_MAG_FILTER, 0x2700 + (normal_filtering == Linear) + (mipmap_filtering == Linear));
	}
	void Sampler::setComparisonFunction(CompareFunction func)
	{
		if (func == None)
			setParameter(GL_COMPARE_REF_TO_TEXTURE, GL_NONE);
		else
		{
			setParameter(GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			setParameter(GL_TEXTURE_COMPARE_FUNC, func);
		}
	}
	void Sampler::setWrap(Wrap st)
	{
		setWrapS(st);
		setWrapT(st);
	}
	void Sampler::setWrap(Wrap s, Wrap t)
	{
		setWrapS(s);
		setWrapT(t);
	}
	void Sampler::setWrapS(Wrap wrap)
	{
		setParameter(GL_TEXTURE_WRAP_S, wrap);
	}
	void Sampler::setWrapT(Wrap wrap)
	{
		setParameter(GL_TEXTURE_WRAP_T, wrap);
	}
	void Sampler::setMinLOD(float min_lod)
	{
		setParameter(GL_TEXTURE_MIN_LOD, min_lod);
	}
	void Sampler::setMaxLOD(float max_lod)
	{
		setParameter(GL_TEXTURE_MAX_LOD, max_lod);
	}
	void Sampler::setLODBias(float lod_bias)
	{
		setParameter(GL_TEXTURE_LOD_BIAS, lod_bias);
	}
	void Sampler::instantiate()
	{
		GLuint myID = id();
		if (!glIsSampler(myID))
			glGenSamplers(1, &myID);
		setID(myID);
	}
	void Sampler::destroy()
	{
		GLuint myID = id();
		if (glIsSampler(myID))
			glGenSamplers(1, &myID);
		setID(myID);
	}
	Texture::Texture() : Object(), m_target(GL_TEXTURE_2D)
	{
		if (Object::GetAutoInstantiate())
			instantiate();
	}
	Texture::Texture(GLenum target, int w, int h) : Object(), m_target(target)
	{
		m_size.x = w, m_size.y = h, m_target = target;
	}
	Texture::Texture(GLenum target, GLuint id) : Object(id), m_target(target)
	{
		set_from(id);
	}
	Texture::~Texture()
	{
		destroy();
	}
	void Texture::set_from(GLuint id)
	{
		bind();
		GLint* format = reinterpret_cast<GLint*>(&m_format);
		glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_INTERNAL_FORMAT, format);
		glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_WIDTH, &m_size.x);
		glGetTexLevelParameteriv(m_target, 0, GL_TEXTURE_HEIGHT, &m_size.y);

	}
	void Texture::bind() const
	{
		m_sampler.bind();
		glBindTexture(m_target, id());
	}
	void Texture::bindTo(GLuint activeTexture)
	{
		m_sampler.unbind();
	}
	void Texture::unbind()
	{
		m_sampler.unbind(activeTexture);
		glBindTexture(m_target, 0);
	}
	void Texture::setSize(glm::vec2 size)
	{
		m_size = size;
	}
	glm::vec2 Texture::getSize() const
	{
		return m_size;
	}
	void Texture::setFormat(GLenum format)
	{
		m_format = format;
	}
	GLenum Texture::getFormat() const
	{
		return m_format;
	}
	void Texture::setTarget(GLenum target)
	{
		m_target = target;
	}
	GLenum Texture::getTarget() const
	{
		return m_target;
	}

	void Texture::setSampler(Sampler::sptr samp)
	{
		m_sampler = samp;
	}

	Sampler::sptr Texture::getSampler() const
	{
		return m_sampler;
	}
	void Texture::init_null(GLenum format, GLenum type)
	{
		bind();
		glTexImage2D(m_target, 0, m_format, m_size.x, m_size.y, 0, format, type, nullptr);
	}
	void Texture::load(GLenum format, GLenum type, const GLvoid * data, glm::vec2 newsize)
	{
		if (!glIsTexture(id()))
			if (Object::GetAutoInstantiate())
				instantiate();
		if (newsize.x != -1 && newsize.y != -1)
			setSize(newsize);
		bind();
		glTexImage2D(m_target, 0, m_format, m_size.x, m_size.y, 0, format, type, data);
	}
	void Texture::generateMipmap()
	{
		bind();
		glGenerateMipmap(m_target);
	}
	void Texture::instantiate()
	{
		GLuint myid=0;
		glGenTextures(1, &myid);
		glBindTexture(m_target, myid);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(m_target, 0);
		setID(myid);
		m_sampler = std::make_unique<Sampler>();
	}

	void Texture::destroy()
	{
		GLuint myid = id();
		if (glIsTexture(myid))
			glDeleteTextures(1, &myid);
		setID(myid);
	}
	
	Framebuffer::Framebuffer() : Object()
	{
		if (Object::GetAutoInstantiate())
			instantiate();
	}

	Framebuffer::~Framebuffer()
	{
		destroy();
	}
	void Framebuffer::BindScreen()
	{
		BindToScreen(TargetFrame);
	}
	void Framebuffer::BindToScreen(Target target)
	{
		glBindFramebuffer(target, 0);
	}
	void Framebuffer::bind() const
	{
		bindTo(TargetFrame);
	}
	void Framebuffer::bindTo(Target target) const
	{
		glBindFramebuffer(target, id());
	}

	void Framebuffer::blit(const Framebuffer & fbo, glm::ivec4 src, glm::ivec4 dst, GLbitfield mask, Sampler::Filter filter)
	{
		fbo.bindTo(TargetRead);
		bindTo(TargetDraw);
		glBlitFramebuffer(src.x, src.y, src.z, src.w, dst.x, dst.y, dst.z, dst.w, mask, filter);
	}

	void Framebuffer::blitToScreen(glm::ivec4 src, glm::ivec4 dst, GLbitfield mask, Sampler::Filter filter)
	{
		bindTo(TargetRead);
		//glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(TargetDraw, 0);
		glDrawBuffer(GL_FRONT);
		glBlitFramebuffer(src.x, src.y, src.z, src.w, dst.x, dst.y, dst.z, dst.w, mask, filter);
	}

	void Framebuffer::attachTexture(Attachment attachment, const Texture & tex, int level_layer)
	{
		bind();
		switch(tex.getTarget())
		{
		case GL_TEXTURE_1D:
			glFramebufferTexture1D(GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.id(), level_layer);
			break;
		default:
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, tex.getTarget(), tex.id(), level_layer);
			break;
		}
	}

	void Framebuffer::attachRenderbuffer(Attachment attachment, const RenderBuffer& renderbuffer)
	{
		/*auto found = m_attachments.find(attachment);
		if (found != m_attachments.end())
			m_attachments.erase(found);

		m_attachments.emplace(std::make_pair(attachment, RenderBuffer()));
		m_attachments[attachment].storage<1>(format, m_size);*/
		bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.id());
	}

	GLenum Framebuffer::getStatus()
	{
		bind();
		return glCheckFramebufferStatus(GL_FRAMEBUFFER);
	}

	void Framebuffer::clear(GLuint flags)
	{
		bind();
		glClear(flags);
	}

	void Framebuffer::instantiate()
	{
		GLuint myid = id();
		if (!glIsFramebuffer(myid))
			glGenFramebuffers(1, &myid);
		setID(myid);
	}

	void Framebuffer::destroy()
	{
		GLuint myID = id();
		if (glIsFramebuffer(myID))
			glDeleteFramebuffers(1, &myID);
		setID(myID);
	}

	RenderBuffer::RenderBuffer() : Object()
	{
		if (Object::GetAutoInstantiate())
			instantiate();
	}

	void RenderBuffer::setSize(glm::ivec2 size)
	{
		m_size = size;
	}

	glm::ivec2 RenderBuffer::getSize()
	{
		return m_size;
	}

	void RenderBuffer::bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, id());
	}

	void RenderBuffer::instantiate()
	{
		GLuint myID = id();
		if (!glIsRenderbuffer(myID))
			glGenRenderbuffers(1, &myID);
		setID(myID);
	}

	void RenderBuffer::destroy()
	{
		GLuint myID = id();
		if (glIsRenderbuffer(myID))
			glDeleteRenderbuffers(1, &myID);
		setID(myID);
	}
	template <>
	void RenderBuffer::storage<1>(GLenum internalformat, glm::ivec2 newsize)
	{
		if (Object::GetAutoInstantiate())
			instantiate();
		if (newsize.x != -1 && newsize.y != -1)
			setSize(newsize);
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, internalformat, m_size.x, m_size.y);
	}

	VertexArray::VertexArray() : Object()
	{
		if (Object::GetAutoInstantiate())
			instantiate();
	}

	VertexArray::VertexArray(GLuint id) : Object(id)
	{
	}
	VertexArray::~VertexArray()
	{
		destroy();
	}
	void VertexArray::enable(int index)
	{
		bind();
		glEnableVertexAttribArray(index);
	}

	void VertexArray::disable(int index)
	{
		bind();
		glDisableVertexAttribArray(index);
	}

	bool VertexArray::isEnabled(int index) const
	{
		int res;
		glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &res);
		return res;
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(id());
	}

	void VertexArray::instantiate()
	{
		GLuint myID = id();
		if (!glIsVertexArray(myID))
			glGenVertexArrays(1, &myID);
		setID(myID);
	}

	void VertexArray::destroy()
	{
		GLuint myID = id();
		if (glIsVertexArray(myID))
			glDeleteVertexArrays(1, &myID);
		setID(myID);
	}

}