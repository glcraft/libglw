#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <type_traits>
#include <stdexcept>
#include <memory>
#include <string>
#include <map>
//#include "GLC_Object.h"
#define DECL_UPTR(T) using uptr = std::unique_ptr<T>;
#define DECL_SPTR(T) using sptr = std::shared_ptr<T>;
#define DECL_PTR(T) DECL_UPTR(T) DECL_SPTR(T)
//! OpenGL Wrapper namespace.
/**
 * @brief OpenGL namespace
 * This namespace contains all classes related to OpenGL (not GLSL)
 * 
 */
namespace gl
{
	namespace sl { class Program; }
	/**
	 * @brief Base of OpenGL Object
	 * 
	 * This is the base for all OpenGL classes. Every OpenGL Object inherite of it, like VAO, VBO, Framebuffer, and so on...
	 * 
	 * It manages ID, creation and destruction of the gl object.
	 * It is of course abstract.
	 * 
	 */
	class Object
	{
	public:
		
		DECL_PTR(Object)
		Object();
		Object(GLuint id);
		virtual ~Object();
		/**
		 * @brief OpenGL ID returns
		 * 
		 * Return The ID got from glGen*
		 * 
		 * @return The gl ID returned by glGen*
		 */
		inline GLuint id() const
		{
			return m_id;
		}
		/**
		 * @brief OpenGL binding
		 * 
		 * Bind the current object.
		 */
		virtual void bind() const = 0;
		void swap(Object& obj);
		/**
		 * @brief OpenGL ID generator
		 * 
		 * Used to create the new OpenGL object by using glGen*. Could also init some default parameters.
		 * Have to be declared in inherited class
		 */
		virtual void instantiate() = 0;
		/**
		 * @brief Enable auto instanciation
		 * 
		 * This enable the auto instantiater option. When enabled, every object created will call instantiate by themselves.
		 * @see GetAutoInstantiate
		 */
		static void SetAutoInstantiate(bool enabled) { m_auto_inst = enabled; }
		/**
		 * @brief Check if auto instanciation is enabled
		 * 
		 * @see SetAutoInstantiate
		 */
		static bool GetAutoInstantiate() { return m_auto_inst; }

	protected:
		/**
		 * @brief OpenGL ID setter
		 * 
		 * Every classes inherited from Object have to write the new ID by this method.
		 * Mostly called by instantiate
		 * 
		 * @param id OpenGL ID got from glGen*
		 * @see instantiate()
		 */
		void setID(GLuint id);
		
		/**
		 * @brief OpenGL ID destroyer
		 * 
		 * Used to destroy the OpenGL object by using glDelete*.
		 * Have to be declared in inherited class
		 */
		virtual void destroy() = 0;
	private:
		static bool m_auto_inst;
		/// OpenGL ID.
		GLuint m_id = 0;
	};
	//* @deprecated Do not use it for now.
	template <class T>
	class Ref
	{
	public:
		Ref() : m_this(nullptr) {  }
		Ref(Ref<T>&& ref) { swap(ref, *this); }
		Ref(const Ref<T>& ref) : m_this(ref.m_this) {  }
		Ref(T& mine) : m_this(&mine) {  }
		Ref<T>& operator=(Ref<T> ref) { swap(ref); return *this; }

		operator const T() const { return *m_this; }
		operator T() { return *m_this; }
		operator bool() const { return m_this!=nullptr; }
		T& operator()() { return *m_this; }
		const T& operator()() const { return *m_this; }

		void swap(Ref<T>& ref)
		{
			using std::swap;
			swap(ref.m_this, m_this);
		}
	private:
		T *m_this;
	};
	/**
	 * @brief Vertex Array Object
	 * 
	 * Wrapper for OpenGL VAO.
	 */
	class VertexArray : public Object
	{
	public:
		DECL_PTR(VertexArray)
		VertexArray();
		VertexArray(GLuint id);
		~VertexArray();
		/// Enable a vertex attrib.
		/// @param index Vertex attribute
		/// @see [glEnableVertexAttribArray](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glEnableVertexAttribArray.xhtml)
		void enable(int );
		/// Disable a vertex attrib.
		/// @param index Vertex attribute
		/// @see [glDisableVertexAttribArray](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDisableVertexAttribArray.xhtml)
		void disable(int );
		/// Return vertex attribute is enabled.
		/// @param index Vertex attribute
		/// @return true if enabled
		/// @see [glGetVertexAttribiv](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetVertexAttribiv.xhtml)(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, myvar)
		bool isEnabled(int ) const;
		void bind() const;
		/// @copydoc Object::instantiate
		/// @see [glGenVertexArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenVertexArrays.xhtml)
		virtual void instantiate();
	protected:
		
		/// @copydoc Object::destroy
		/// @see [glDeleteVertexArrays](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteVertexArrays.xhtml)
		virtual void destroy();
	};
	/**
	 * @brief Buffer object
	 * 
	 * Wrapper for OpenGL Buffer object. 
	 * Work for all kind of OpenGL buffer Object (e.g. VBO, EBO, Texture buffer, ...).
	 * @param target Kind of buffer
	 * @param MyStruct Type to work with
	 */
	template <GLenum target, typename MyStruct>
	class Buffer : public Object
	{
	public:
		Buffer() : m_size(0), m_capacity(0), m_map(nullptr)
		{
			if (Object::GetAutoInstantiate())
				instantiate();
		}
		/**
		 * @brief Append or reduce data.
		 * 
		 * @param size how many *MyStruct* to add/sub
		 * @param usage Usage of the buffer 
		 * @see reserve
		 */
		void reserve_relative(GLsizeiptr size, GLenum usage = GL_STREAM_DRAW)
		{
			m_size = m_size + size;
			auto prevcapa = m_capacity;
			m_capacity = glm::max<GLsizeiptr>(m_capacity, size);
			if (prevcapa < m_capacity)
			{
				m_capacity = m_size;
				m_capacity = m_capacity * 4 / 3;
				bind();
				glBufferData(target, m_capacity * sizeof(MyStruct), nullptr, usage);
			}
		}
		/**
		 * @brief Create and reserve data
		 * 
		 * Note : works like std::vector with a size AND a capacity.
		 * 
		 * @see [glBufferData](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml)
		 */
		void reserve(GLsizeiptr size, GLenum usage = GL_STREAM_DRAW)
		{
			m_size = size;
			auto prevcapa = m_capacity;
			m_capacity = glm::max<GLsizeiptr>(m_capacity, size);
			if (prevcapa < m_capacity)
			{
				bind();
				glBufferData(target, m_capacity * sizeof(MyStruct), nullptr, usage);
			}
		}
		/**
		 * @brief Create and reserve data to exact size
		 * 
		 * Unlike reserve, it will force the capatity to the size, like reserve then shrink_to_fit.
		 * 
		 * Note : works like std::vector with a size AND a capacity.
		 * 
		 * @see [glBufferData](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml)
		 */
		void force_reserve(GLsizeiptr size, GLenum usage = GL_STREAM_DRAW)
		{
			m_size = size;
			m_capacity = size;
			bind();
			glBufferData(target, m_capacity * sizeof(MyStruct), nullptr, usage);
		}
		/**
		 * @brief Shrink the data to correspond to size instead of capacity
		 *
		 * Warning : it doesn't copy old data into new one !
		 *
		 * @see [glBufferData](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBufferData.xhtml)
		 */
		void shrink_to_fit()
		{
			if (m_capacity == m_size)
				return;
			m_capacity = m_size;
			bind();
			glBufferData(target, m_capacity * sizeof(MyStruct), nullptr, GL_STREAM_DRAW);
		}
		/// Return the size to use
		GLuint size() const
		{
			return m_size;
		}
		/// Return the capacity of the buffer
		GLuint capacity() const
		{
			return m_size;
		}
		void bind() const
		{
			glBindBuffer(target, id());
		}
		/**
		 * @brief Get mapped data
		 * 
		 * Once buffer data mapped, you can access it by this pointer.
		 * 
		 * @return mapped data 
		 * @see operator[]
		 */
		MyStruct* data()
		{
			return m_map;
		}
		const MyStruct* const data() const
		{
			return m_map;
		}
		/**
		 * @brief Get mapped data
		 * 
		 * Once buffer data mapped, you can access it by this pointer.
		 * !BE CAREFUL! : use this function with already mapped data !
		 * 
		 * @param index Offset in the array buffer.
		 * @return Value of buffer at index.
		 * @see data
		 */
		MyStruct& operator[](GLuint index)
		{
			if (!m_map)
				map(GL_READ_WRITE);
				//throw std::runtime_error("Buffer map not accessible");
			return m_map[index];
		}
		/// Map buffer.
		/// @see [glMapBuffer](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glMapBuffer.xhtml)
		MyStruct* map(GLenum access)
		{
			bind();
			return m_map = reinterpret_cast<MyStruct*>(glMapBuffer(target, access));
		}
		/// Map buffer in Write Only mode.
		/// @see [glMapBuffer](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glMapBuffer.xhtml)
		MyStruct* map_write()
		{
			bind();
			return m_map = reinterpret_cast<MyStruct*>(glMapBuffer(target, GL_WRITE_ONLY));
		}
		/// Map buffer in Read Only mode.
		/// @see [glMapBuffer](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glMapBuffer.xhtml)
		const MyStruct* map_read() const 
		{
			bind();
			return m_map = reinterpret_cast<MyStruct*>(glMapBuffer(target, GL_READ_ONLY));
		}
		/// Map buffer in Read Write mode.
		/// @see [glMapBuffer](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glMapBuffer.xhtml)
		MyStruct* map_readwrite()
		{
			bind();
			return m_map = reinterpret_cast<MyStruct*>(glMapBuffer(target, GL_READ_WRITE));
		}
		/// Unmap buffer.
		/// @see [glUnmapBuffer](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glUnmapBuffer.xhtml)
		void unmap() const
		{
			if (m_map)
			{
				bind();
				glUnmapBuffer(target);
				m_map = nullptr;
			}
		}
		/// @copydoc Object::instantiate
		/// @see [glGenBuffers](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml)
		virtual void instantiate()
		{
			GLuint myID = id();
			if (!glIsBuffer(myID))
				glGenBuffers(1, &myID);
			setID(myID);
		}
	protected:
		
		/// @copydoc Object::destroy
		/// @see [glDeleteBuffers](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml)
		virtual void destroy()
		{
			GLuint myID = id();
			if (glIsBuffer(myID))
				glDeleteBuffers(1, &myID);
			setID(myID);
		}
		/// Return the target setted in the template
		virtual GLenum getTarget() const
		{
			return target;
		}
		GLsizeiptr m_size=0, m_capacity=0;
		mutable MyStruct* m_map=nullptr;
	};
	template <typename Integer>
	class ElementBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, Integer>
	{
	public:
		DECL_PTR(ElementBuffer)
		ElementBuffer() : Buffer<GL_ELEMENT_ARRAY_BUFFER, Integer>()
		{
			static_assert(std::is_integral<Integer>::value, "ElementBuffer template type is integrale only.");
		}
	private:
	};
	template <typename MyStruct>
	class ArrayBuffer : public Buffer<GL_ARRAY_BUFFER, MyStruct>
	{
	public:
		DECL_PTR(ArrayBuffer)
		template <int _index>
		struct Attrib
		{
			Attrib(GLsizei _offset = 0, GLint _size = 3, GLenum _type = GL_FLOAT, GLboolean _normalized = GL_FALSE) : offset(_offset), size(_size), type(_type), normalized(_normalized)
			{
				
			}
			GLsizei offset;
			GLint size = 3;
			GLenum type = GL_FLOAT;
			GLboolean normalized = GL_FALSE;
			enum { stride = sizeof(MyStruct) };
			enum { index = _index };
		};
		ArrayBuffer() : Buffer<GL_ARRAY_BUFFER, MyStruct>()
		{
			
		}
		~ArrayBuffer()
		{
			this->destroy();
		}
		template <typename ...Args>
		void set_attrib(Args... args)
		{
			bindVAO();
			this->bind();
			set_attrib_priv(args...);
		}
		void attachVertexArray(VertexArray::sptr vao)
		{
			m_VAO = vao;
		}

		GLuint vertex_array() const
		{
			if (m_VAO)
				return m_VAO->id();
			return 0;
		}
		
		void draw(GLenum mode) const
		{
			draw(mode, 0, this->m_size);
		}
		void draw(GLenum mode, GLint first, GLsizei count) const
		{
			///not const...
			/*if (m_map)
				unmap();*/

			bindVAO();
			this->bind();
			glDrawArrays(mode, first, count);
		}
		template <typename Integer>
		void draw(const ElementBuffer<Integer>& ebo, GLenum mode) const
		{
			draw(ebo, mode, 0, this->m_size);
		}
		template <typename Integer>
		void draw(const ElementBuffer<Integer>& ebo, GLenum mode, GLint first, GLsizei count) const
		{
			bindVAO();
			this->bind();
			ebo.bind();
			int type = 0;
			switch (sizeof(Integer))
			{
			case 1:
				type = GL_UNSIGNED_BYTE; break;
			case 2:
				type = GL_UNSIGNED_SHORT; break;
			case 4:
				type = GL_UNSIGNED_INT; break;
			default:
				throw std::runtime_error("Wrong integral type for ElementBuffer. Must be less than 4 bytes");
			}
			glDrawElements(mode, count, type, reinterpret_cast<const GLvoid*>(first*sizeof(Integer)));
		}
	private:
		void bindVAO() const
		{
			if (m_VAO)
				m_VAO->bind();
				//glBindVertexArray(m_VAO);
		}
		template <int id, typename ...Args>
		void set_attrib_priv(Attrib<id> attrib, Args... args)
		{
			set_attrib_priv(attrib);
			set_attrib_priv(args...);
		}
		template <int id>
		void set_attrib_priv(Attrib<id> attrib)
		{
			glVertexAttribPointer(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, reinterpret_cast<GLvoid*>(attrib.offset));
			glEnableVertexAttribArray(attrib.index);
		}
		VertexArray::sptr m_VAO;
		
	};
	//http://www.geeks3d.com/3dfr/20140703/uniform-buffers-objects-opengl-31-tutorial/
    template<typename MyStruct>
    class UniformBuffer : public Buffer<GL_UNIFORM_BUFFER, MyStruct>
    {
    public:
        using BufferBase = Buffer<GL_UNIFORM_BUFFER, MyStruct>;
        UniformBuffer() : BufferBase()
		{
			
		}
        ~UniformBuffer()
		{
			this->destroy();
		}
        void setName(std::string_view block_name)
        {
            m_blockName = block_name;
        }
        
        void bindBase(GLuint bind_point)
        {
            m_bindPoint = bind_point;
            BufferBase::bind();
            glBindBufferBase(GL_UNIFORM_BUFFER, m_bindPoint, this->id());
        }
        template <typename ...Args>
        void bind(Args... programs)
        {
            (bind(programs), ...);
        }
        void bind(gl::sl::Program& program)
        {
            BufferBase::bind();
            // int blockIndx= getBlockIndex(program, m_blockName);
            
            // if (blockIndx!=GL_INVALID_INDEX)
            // {
            //     glUniformBlockBinding(program.id(), blockIndx, m_bindPoint);
            // }
        }
    protected:
        // GLuint getBlockIndex(gl::sl::Program& program, std::string_view block_name)
        // {
        //     return glGetUniformBlockIndex(program.id(), block_name.data());
        // }
    private:
        GLuint m_bindPoint=0;
        std::string m_blockName;
    };
	
	template <class Type>
	class Uniform
	{
	public:
		DECL_PTR(Uniform)
		Uniform(std::string name) : m_name(name)
		{}
		const std::string& name() const
		{
			return m_name;
		}
		void use(const gl::sl::Program& shader) const;
		virtual const Type* instance() const = 0;
	private:
		std::string m_name;
	};
	template <class Type>
	class UniformRef : public Uniform<Type>
	{
	public:
		UniformRef(std::string name, const Type& instance) : Uniform<Type>(name), m_instance(&instance)
		{

		}
		UniformRef(std::string name, Type&& instance) = delete; // Move unable, use UniformStatic<...>

		const Type* instance() const
		{
			return m_instance;
		}
	private:
		const Type* m_instance;
	};
	template <class Type>
	class UniformStatic: public Uniform<Type>
	{
	public:
		UniformStatic(std::string name, const Type& instance) : Uniform<Type>(name), m_instance(instance)
		{

		}
		UniformStatic(std::string name, Type&& instance) : Uniform<Type>(name), m_instance(instance)
		{

		}
		const Type* instance() const
		{
			return &m_instance;
		}
		void set(Type val)
		{
			using std::swap;
			swap(m_instance, val);
		}
		const Type& get() const
		{
			return m_instance;
		}
	private:
		Type m_instance;
	};
	class Sampler : public Object
	{
	public:
		DECL_PTR(Sampler)
		enum Filter
		{
			Linear = GL_LINEAR,
			Nearest = GL_NEAREST
		};
		enum CompareFunction
		{
			None = -1,
			Never = GL_NEVER,
			Always = GL_ALWAYS,
			Less = GL_LESS,
			LessEqual = GL_LEQUAL,
			Equal = GL_EQUAL,
			NotEqual = GL_NOTEQUAL,
			GreaterEqual = GL_GEQUAL,
			Greater = GL_GREATER
		};
		enum Wrap
		{
			Repeat = GL_REPEAT,
			MirroredRepeat = GL_REPEAT,
			ClampToEdge = GL_REPEAT,
			ClampToBorder = GL_REPEAT,
			MirroredClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
		};
		Sampler();
		virtual void instantiate();
		~Sampler();

		void bind() const;
		void bind(GLuint activeTexture) const;

		void unbind() const;
		void unbind(GLuint activeTexture) const;

		void setParameter(GLenum paramName, int value);
		void setParameter(GLenum paramName, float value);
		///Parameters
		void setFiltering(Filter global_filtering);
		void setFiltering(Filter global_filtering, Filter mipmap_filtering);
		void setMinFiltering(Filter global_filtering);
		void setMinFiltering(Filter global_filtering, Filter mipmap_filtering);
		void setMagFiltering(Filter global_filtering);
		void setMagFiltering(Filter global_filtering, Filter mipmap_filtering);
		void setComparisonFunction(CompareFunction);
		void setWrap(Wrap st);
		void setWrap(Wrap s, Wrap t);
		void setWrapS(Wrap wrap);
		void setWrapT(Wrap wrap);
		void setMinLOD(float min_lod);
		void setMaxLOD(float max_lod);
		void setLODBias(float lod_bias);
	protected:
		
		virtual void destroy();
	};

	class Texture : public Object
	{
	public:
		DECL_PTR(Texture)
		
		Texture();
		Texture(GLenum target, int w, int h);
		Texture(GLenum target, GLuint id);
		virtual void instantiate();
		~Texture();

		void bind() const;
		void bind(GLuint activeTexture) const;

		void unbind() const;
		void unbind(GLuint activeTexture) const;

		void set_from(GLuint id);

		void setSize(glm::vec2 size);
		glm::vec2 getSize() const;

		void setFormat(GLenum format);
		GLenum getFormat() const;

		void setTarget(GLenum target);
		GLenum getTarget() const;

		void setSampler(Sampler);
		Sampler getSampler() const;

		void init_null(GLenum format=GL_RGBA, GLenum type=GL_UNSIGNED_BYTE);
		void load(GLenum format, GLenum type, const GLvoid * data, glm::vec2 newsize = glm::vec2(-1));
		void generateMipmap();
	protected:
		
		virtual void destroy();
	private:
		glm::ivec2 m_size;
		GLenum m_format;
		GLenum m_target;
		Sampler m_sampler;
	};
	
	class RenderBuffer : public Object
	{
	public:
		DECL_PTR(RenderBuffer)
		RenderBuffer();
		virtual void instantiate();
		template <int multisample>
		void storage(GLenum internalformat, glm::ivec2 newsize = glm::ivec2(-1))
		{
			instantiate();
			if (newsize.x != -1 && newsize.y != -1)
				setSize(newsize);
			bind();
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample, internalformat, m_size.x, m_size.y);
		}
		void setSize(glm::ivec2 size);
		glm::ivec2 getSize();
		void bind() const;
		
	protected:
		virtual void destroy();
		glm::ivec2 m_size;
	};
	template <>
	void RenderBuffer::storage<1>(GLenum internalformat, glm::ivec2 newsize);
	class Framebuffer : public Object
	{
	public:
		DECL_PTR(Framebuffer)
		enum Target
		{
			TargetRead = GL_READ_FRAMEBUFFER,
			TargetDraw = GL_DRAW_FRAMEBUFFER,
			TargetFrame = GL_FRAMEBUFFER
		};
		enum ClearFlags
		{
			ClearColor = GL_COLOR_BUFFER_BIT,
			ClearDepth = GL_DEPTH_BUFFER_BIT,
			ClearStencil = GL_STENCIL_BUFFER_BIT,
			ClearAccum = GL_ACCUM_BUFFER_BIT
		};
		enum Attachment
		{
			AttachColor0 = GL_COLOR_ATTACHMENT0,
			AttachColor1 = GL_COLOR_ATTACHMENT1,
			AttachColor2 = GL_COLOR_ATTACHMENT2,
			AttachColor3 = GL_COLOR_ATTACHMENT3,
			AttachColor4 = GL_COLOR_ATTACHMENT4,
			AttachColor5 = GL_COLOR_ATTACHMENT5,
			AttachColor6 = GL_COLOR_ATTACHMENT6,
			// And so on...
			AttachDepth = GL_DEPTH_ATTACHMENT,
			AttachStencil = GL_STENCIL_ATTACHMENT,
			AttachDepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
		};
		Framebuffer();
		virtual void instantiate();
		~Framebuffer();

		static void BindScreen();
		static void BindToScreen(Target target);
		void bind() const;
		void bindTo(Target target) const;

		void blit(const Framebuffer& fbo, glm::ivec4 src, glm::ivec4 dst, GLbitfield mask, Sampler::Filter filter);
		void blitToScreen(glm::ivec4 src, glm::ivec4 dst, GLbitfield mask, Sampler::Filter filter);

		void attachTexture(Attachment attachment, const Texture& tex, int level_layer);
		void attachRenderbuffer(Attachment attachment, const RenderBuffer& renderbuffer);

		GLenum getStatus();

		void clear(GLuint flags = ClearColor);
		
	protected:
		virtual void destroy();

		glm::ivec2 m_size;
		
		std::map<GLenum, RenderBuffer> m_attachments;
	};
}