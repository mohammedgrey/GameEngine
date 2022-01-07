#pragma once
#include <shader/shader.hpp>
#include <mesh/mesh.hpp>
#include <texture/texture2d.hpp>
#include <texture/texture-utils.hpp>
#include <application.hpp>

#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

// This state tests and shows how to use the Texture2D class.
class CongratsState : public our::State
{
    our::World world;
    our::ShaderProgram *shader;
    our::Mesh *mesh;
    our::Texture2D *texture;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["congrats-scene"];
        // Then we load the shader that will be used for this scene
        shader = new our::ShaderProgram();
        shader->attach("assets/shaders/texture-test.vert", GL_VERTEX_SHADER);
        shader->attach("assets/shaders/texture-test.frag", GL_FRAGMENT_SHADER);
        shader->link();

        // We create a simple 2D plane to use for viewing the plane
        std::vector<our::Vertex> vertices = {
            {{-1, -1, 0}, {255, 255, 255, 255}, {0.00, 0.00}, {0, 0, 1}},
            {{1, -1, 0}, {255, 255, 255, 255}, {1.00, 0.00}, {0, 0, 1}},
            {{1, 1, 0}, {255, 255, 255, 255}, {1.00, 1.00}, {0, 0, 1}},
            {{-1, 1, 0}, {255, 255, 255, 255}, {0.00, 1.00}, {0, 0, 1}},
        };
        std::vector<unsigned int> elements = {
            0,
            1,
            2,
            2,
            3,
            0,
        };
        mesh = new our::Mesh(vertices, elements);

        // Then we create a texture and load an image into it
        texture = new our::Texture2D();
        our::texture_utils::loadImage(*texture, config.value("texture", "").c_str());
    }

    void onDraw(double deltaTime) override
    {

        glClear(GL_COLOR_BUFFER_BIT);
        shader->use();
        // Here we set the active texture unit to 0 then bind the texture to it
        glActiveTexture(GL_TEXTURE0);
        texture->bind();
        // Then we send 0 (the index of the texture unit we used above) to the "tex" uniform
        shader->set("tex", 0);
        mesh->draw();
    }
    void onKeyEvent(int key, int scancode, int action, int mods) override
    {
        if (key == GLFW_KEY_ESCAPE)
        {
         
            glfwSetWindowShouldClose(getApp()->getWindow(),1);

          
        }
    }

    void onDestroy() override
    {
        delete shader;
        delete mesh;
        delete texture;
        world.clear();
        our::clearAllAssets();
    }
};