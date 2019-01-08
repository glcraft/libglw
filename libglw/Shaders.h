#pragma once
#include <GL/glew.h>
#ifdef QT_UI
#	include <QtOpenGL>
#endif
#include <string>
#include <list>
#include <memory>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
//#include "GLC_Object.h"
#include "GLClass.h"

namespace gl
{
	namespace sl
	{
		enum TypeShader
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Geometry = GL_GEOMETRY_SHADER
		};
		class CompileException : public std::exception
		{
		public:
			CompileException(TypeShader _type, std::string _name, GLuint _id);

			virtual ~CompileException();
			virtual const char* what() const noexcept;
		protected:
			std::string name;
			TypeShader type;
			GLuint id;
			std::string err;
		};
		template <TypeShader type>
		class Shader : public Object
		{
		public:

			Shader() : Object()
			{}
			Shader(const char* filename, bool isFile = true) : Shader()
			{
				set(std::string(filename), isFile);
			}
			Shader(std::string filename, bool isFile = true) : Shader()
			{
				set(filename, isFile);
			}
			Shader(Shader&& shad) : Object(std::move(shad))
			{
				std::swap(type, shad.type);
			}
			~Shader()
			{
				
			}
			void set(std::string input, bool isFile = true)
			{
				instanciate();
				if (id() == 0)
					throw std::runtime_error("gl::sl::Shader : type inconnu");

				std::string codeSource;
				if (isFile)
				{
					std::ostringstream ostrSource;
					std::string ligneCodeSource;
					std::ifstream fichierSource(input);
					// Test de l'ouverture

					if (!fichierSource)
					{
						destroy();
						throw std::runtime_error("gl::sl::Shader : fichier introuvable (" + input + ")");
					}

					// Lecture du contenu
					while (getline(fichierSource, ligneCodeSource))
						ostrSource << ligneCodeSource << '\n';
					codeSource.swap(ostrSource.str());
				}
				else
					codeSource.swap(input);
				auto myid = id();
				const GLchar *cCode = static_cast<const GLchar*>(codeSource.c_str());
				glShaderSource(myid, 1, &cCode, 0);
				glCompileShader(myid);
				GLint erreurCompilation(0);
				glGetShaderiv(myid, GL_COMPILE_STATUS, &erreurCompilation);

				if (erreurCompilation != GL_TRUE)
					throw CompileException(type, input, myid);
			}
			GLenum getType()
			{
				return type;
			}
			GLboolean exists()
			{
				return glIsShader(id());
			}
			void bind() const
			{
				// There's no bind for Shader, discard...
			}
		protected:
			virtual void instanciate()
			{
				GLuint myid = id();
				if (!glIsShader(myid))
					myid = glCreateShader(type);
				setID(myid);
			}
			virtual void destroy()
			{
				GLuint myid = id();
				if (glIsShader(myid))
					glDeleteShader(myid);
				setID(myid);
			}
		};
		class Collection
		{
		public:
			Collection(std::vector<TypeShader> types, std::string filepath);
			const std::vector<TypeShader>& getTypes();
			const std::string& getFilePath();
		private:
			std::vector<TypeShader> m_types;
			std::string m_fpath;
		};
		/*class Texture
		{
		public:
			Texture();
			Texture(GLenum, GLuint, bool = false);
			Texture(const Texture&);
			Texture(Texture&&);
			~Texture();
			void setTexture(GLenum, GLuint, bool = false);
			friend void swap(gl::sl::Texture& t1, gl::sl::Texture& t2);
			GLenum getTarget();
			GLuint getID();
			void use(int textureActive = 0);
		private:
			bool isRef = true;
			GLenum m_target;
			GLuint m_id;
		};*/
		class Program : public Object
		{
		public:
			class LinkException : public std::exception
			{
			public:
				LinkException(GLuint _id);
				virtual ~LinkException();
				virtual const char* what() const noexcept;
			protected:
				GLuint id;
				std::string err;
			};

			Program();
			Program(GLuint id);
			Program(const Program&);
			Program(Program&&);
			template <typename ...Args>
			Program(Args... args)
			{
				attachShader(args...);
			}
			~Program();
			Program& operator=(Program);

			friend void swap(Program& p1, Program& p2);
			template <typename Type>
			Program& operator<<(gl::Uniform<Type>& uni)
			{
				uni.use(*this);
				return *this;
			}
			template <TypeShader type>
			Program& operator<< (Shader<type>& shad)
			{
				attachShader(shad);
				return *this;
			}
			Program& operator<< (Collection& col);
			Program& operator<< (Program& (*ext)(Program&));
			template <TypeShader type, typename ...Args>
			void attachShader(Shader<type>& shad, Args... shaders)
			{
				attachShader(shad);
				attachShader(shaders...);
			}
			template <TypeShader type>
			void attachShader(Shader<type>& shader)
			{
				if (isRef)
					throw std::runtime_error("Program reference not accessible for attachment");
				if (!exists())
					instanciate();
				glAttachShader(id(), shader.id());
			}
			void link();
			GLint getUniformLocation(const std::string& name) const;
			GLint getUniformLocation(const char* name) const;

			GLint getAttribLocation(const std::string& name) const;
			GLint getAttribLocation(const char* name) const;

			GLboolean exists() const;
			void use() const;
			void load(const std::string & name);
			void bind() const;
			void swap(Program& prog);
		protected:
			void instanciate();
			void destroy();
		private:
			bool isRef;
		};
		inline gl::sl::Program& link(gl::sl::Program& prog)
		{
			prog.link();
			return prog;
		}
		inline gl::sl::Program& use(gl::sl::Program& prog)
		{
			prog.use();
			return prog;
		}

	}
}
#define TRY_GLSL try {
#define CATCH_GLSL }\
catch (gl::sl::CompileException exc) { std::cerr << "GLSL: Erreur de compilation." << exc.what() << std::endl; }\
catch (gl::sl::Program::LinkException exc) { std::cerr << "GLSL: Erreur de link." << exc.what() << std::endl; }
