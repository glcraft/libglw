#include <libglw/GLClass.h>
#include <libglw/Shaders.h>
namespace gl
{
	template <>
	void Uniform<glm::mat4>::use(const gl::sl::Program& shader) const
	{glUniformMatrix4fv(shader.getUniformLocation(name()), 1, GL_FALSE, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<glm::mat3>::use(const gl::sl::Program& shader) const
	{glUniformMatrix3fv(shader.getUniformLocation(name()), 1, GL_FALSE, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<glm::mat2>::use(const gl::sl::Program& shader) const
	{glUniformMatrix2fv(shader.getUniformLocation(name()), 1, GL_FALSE, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<glm::vec4>::use(const gl::sl::Program& shader) const
	{glUniform4fv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<glm::vec3>::use(const gl::sl::Program& shader) const
	{glUniform3fv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<glm::vec2>::use(const gl::sl::Program& shader) const
	{glUniform2fv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLfloat*>(instance()));}

	template <>
	void Uniform<float>::use(const gl::sl::Program& shader) const
	{glUniform1fv(shader.getUniformLocation(name()), 1, instance());}

	template <>
	void Uniform<glm::ivec4>::use(const gl::sl::Program& shader) const
	{glUniform4iv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLint*>(instance()));}

	template <>
	void Uniform<glm::ivec3>::use(const gl::sl::Program& shader) const
	{glUniform3iv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLint*>(instance()));}

	template <>
	void Uniform<glm::ivec2>::use(const gl::sl::Program& shader) const
	{glUniform2iv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLint*>(instance()));}

	template <>
	void Uniform<int>::use(const gl::sl::Program& shader) const
	{glUniform1iv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLint*>(instance()));}

	template <>
	void Uniform<glm::uvec4>::use(const gl::sl::Program& shader) const
	{glUniform4uiv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLuint*>(instance()));}

	template <>
	void Uniform<glm::uvec3>::use(const gl::sl::Program& shader) const
	{glUniform3uiv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLuint*>(instance()));}

	template <>
	void Uniform<glm::uvec2>::use(const gl::sl::Program& shader) const
	{glUniform2uiv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLuint*>(instance()));}

	template <>
	void Uniform<uint32_t>::use(const gl::sl::Program& shader) const
	{glUniform1uiv(shader.getUniformLocation(name()), 1, reinterpret_cast<const GLuint*>(instance()));}
}