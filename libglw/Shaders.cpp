#include "Shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/type_ptr.hpp>
namespace gl
{
	namespace sl
	{
		CompileException::CompileException(TypeShader _type, std::string _name, GLuint _id) : name(_name), type(_type), id(_id)
		{
			// Récupération de la taille de l'erreur
			GLint tailleErreur(0);
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &tailleErreur);

			// Allocation de mémoire pour l'erreur
			char *erreur = new char[tailleErreur + 1];
			erreur[tailleErreur] = '\0';

			// Récupération de l'erreur
			glGetShaderInfoLog(id, tailleErreur, &tailleErreur, erreur);

			// Detection du type
			std::ostringstream ostr;
			std::string txttype;
			switch (type)
			{
			case GL_VERTEX_SHADER:
				txttype = "Vertex Shader";
				break;
			case GL_FRAGMENT_SHADER:
				txttype = "Fragment Shader";
				break;
			case GL_GEOMETRY_SHADER:
				txttype = "Geometry Shader";
				break;
				/*case GL_TESS_CONTROL_SHADER:
				txttype = "Tesselation Control Shader";
				break;
				case GL_TESS_EVALUATION_SHADER:
				txttype = "Tesselation Evaluation Shader";
				break;*/
			}

			// Affichage de l'erreur
			ostr << "\nErreur de compilation du shader (" << type << ")\nNom du Shader : " + name + "\nType de shader : " << txttype << "\nErreur :\n" << std::string(erreur) << std::endl;
			delete[] erreur;
			err = ostr.str();
		}
		CompileException::~CompileException()
		{
			if (glIsShader(id))
				glDeleteShader(id);
		}
		const char* CompileException::what() const noexcept
		{
			return err.c_str();
		}
		Program::Program() : Object()
		{
			isRef = false;
		}

		Program::Program(GLuint id) : Object(id)
		{
			isRef = false;
		}

		Program::Program(const Program & program) : Object(program.id())
		{
			isRef = true;
		}

		Program::Program(Program && program)
		{
			swap(program);
		}

		Program::~Program()
		{

		}

		Program & Program::operator=(Program prog)
		{
			swap(prog);
			return *this;
		}
		Program & Program::operator<<(Collection & col)
		{
			auto& lsTypes = col.getTypes();
			for (auto type : lsTypes)
			{
				switch (type)
				{
				case gl::sl::Vertex: attachShader(gl::sl::Shader<gl::sl::Vertex>(col.getFilePath() + ".vert")); break;
				case gl::sl::Fragment: attachShader(gl::sl::Shader<gl::sl::Fragment>(col.getFilePath() + ".frag")); break;
				case gl::sl::Geometry: attachShader(gl::sl::Shader<gl::sl::Geometry>(col.getFilePath() + ".geom")); break;
				}

			}
			return *this;
		}
		Program & Program::operator<<(Program &(*ext)(Program &))
		{
			return ((*ext)(*this));
		}
		const Program& Program::operator<<(const Program& (*ext)(const Program&)) const
		{
			return ((*ext)(*this));
		}
		void Program::link()
		{
			glGetError();
			glLinkProgram(id());
			GLint link(0);
			glGetProgramiv(id(), GL_LINK_STATUS, &link);
			if (link != GL_TRUE)
				throw LinkException(id());
		}

		GLint Program::getAttribLocation(const std::string & name) const
		{
			return getAttribLocation(name.c_str());
		}

		GLint Program::getAttribLocation(const char * name) const
		{
			return glGetAttribLocation(id(), name);
		}

		GLboolean Program::exists() const
		{
			return glIsProgram(id());
		}

		GLint Program::getUniformLocation(const std::string& name) const
		{
			return getUniformLocation(name.c_str());
		}

		GLint Program::getUniformLocation(const char * name) const
		{
			return glGetUniformLocation(id(), name);
		}
		void Program::use()  const
		{
			glUseProgram(id());
		}
		void Program::load(const std::string& name)
		{
			TRY_GLSL
				destroy();
			attachShader(gl::sl::Shader<gl::sl::Vertex>(name + ".vert"));
			attachShader(gl::sl::Shader<gl::sl::Fragment>(name + ".frag"));
			link();
			CATCH_GLSL
		}
		void Program::bind() const
		{
			use();
		}
		void Program::swap(Program & prog)
		{
			using std::swap;
			Object::swap(prog);
			swap(prog.isRef, isRef);
		}
		void Program::instanciate()
		{
			GLuint myid = id();
			if (!exists())
				myid = glCreateProgram();
			setID(myid);
		}
		void Program::destroy()
		{
			GLuint myid = id();
			if (!isRef && exists())
				glDeleteProgram(myid);
			setID(0);
			isRef = false;
		}


		Program::LinkException::LinkException(GLuint _id) : id(_id)
		{
			// Récupération de la taille de l'erreur
			GLint tailleErreur(0);
			char *erreur(0);

			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &tailleErreur);

			// Allocation de mémoire pour l'erreur
			erreur = new char[tailleErreur + 1];

			// Copie de l'erreur dans la chaine de caractères
			glGetProgramInfoLog(id, tailleErreur, &tailleErreur, erreur);
			erreur[tailleErreur] = '\0';

			
			// Affichage de l'erreur
			std::ostringstream ostr;
			ostr << "\nErreur lors du link du program\nErreur :\n" << erreur << std::endl; 
			int errorGL = glGetError();
			if (errorGL>0)
				ostr << "Erreur OpenGL (" << errorGL << ") : " << gluErrorString(errorGL) << std::endl;
			// On libère la mémoire puis on retourne false
			delete[] erreur;
			err = ostr.str();
		}
		Program::LinkException::~LinkException() 
		{ 
			glDeleteProgram(id); 
		}
		const char * Program::LinkException::what() const noexcept
		{
			return err.c_str();
		}
		void swap(Program & p1, Program & p2)
		{
			using std::swap;
			p1.swap(p2);
		}

		Collection::Collection(std::vector<TypeShader> types, std::string filepath)
		{
			m_types.swap(types);
			m_fpath.swap(filepath);
		}
		const std::vector<TypeShader>& Collection::getTypes()
		{
			return m_types;
		}
		const std::string & Collection::getFilePath()
		{
			return m_fpath;
		}
	}
}