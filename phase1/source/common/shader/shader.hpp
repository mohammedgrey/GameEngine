#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        //Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        ShaderProgram() { program = 0; }
        //Overwriting copy constructor to disable it.
        ShaderProgram(const ShaderProgram &shader) {}
        //Overwriting assignment operator to disable it.
        void operator=(const ShaderProgram &shader) {}
        ~ShaderProgram() { destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            //TODO:DONE call opengl to use the program identified by this->program
            glUseProgram(this->program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            //TODO:DONE call opengl to get the uniform location for the uniform defined by name from this->program
            return glGetUniformLocation(this->program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            //TODO:DONE call opengl to set the value to the uniform defined by name
            glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            //TODO:DONE call opengl to set the value to the uniform defined by name
            glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            //TODO:DONE call opengl to set the value to the uniform defined by name
            glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            //TODO:DONE call opengl to set the value to the uniform defined by name
            glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        //TODO:DONE Delete the copy constructor and assignment operator
        //Question: Why do we do this? Hint: Look at the deconstructor
        // To prevent the user from creating a new shader object from
        // an old one, since that is not possible because we delete the
        // shder after linking it to the program and the only thing left is the
        // program id which can't be used to create a copy since no two programs can
        // have the same ID
    };

}

#endif