#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: Write this function
        Material::setup(); // set up of the parent
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {

        // TODO: Write this function
        TintedMaterial::setup(); // set up of the parent
        shader->set("alphaThreshold", alphaThreshold);
        GLuint textureUnit = 0; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        texture->bind();
        sampler->bind(textureUnit);
        shader->set("tex", textureUnit);
        
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }


    //Phase3
    void LitMaterial::setup() const
    {
        // TODO: Write this function
        Material::setup(); // set up of the parent
        
        // //Albedo Texture
        GLuint textureUnit = 0;
        glActiveTexture(GL_TEXTURE0 + 0);
        albedo_texture->bind();
        sampler->bind(0);
        shader->set("albedo_tex", 0);

        
        // //Specular Texture
        textureUnit = 1;
        glActiveTexture(GL_TEXTURE0 + 1);
        specular_texture->bind();
        sampler->bind(1);
        shader->set("specular_tex", 1);
        
        
        // //Roughness Texture
        textureUnit = 2;
        glActiveTexture(GL_TEXTURE0 + 2);
        roughness_texture->bind();
        sampler->bind(2);
        shader->set("roughness_tex", 2);
        
        
        //AO Texture
        textureUnit = 3;
        glActiveTexture(GL_TEXTURE0 + 3);
        ao_texture->bind();
        sampler->bind(3);
        shader->set("ao_tex", 3);
        
        //Emission Texture
        textureUnit = 4;
        glActiveTexture(GL_TEXTURE0 + 4);
        emission_texture->bind();
        sampler->bind(4);
        shader->set("emission_tex", 4); 
        
    }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        

        albedo_texture = AssetLoader<Texture2D>::get(data.value("albedo_texture", ""));
        specular_texture = AssetLoader<Texture2D>::get(data.value("specular_texture", ""));
        roughness_texture  = AssetLoader<Texture2D>::get(data.value("roughness_texture", ""));
        ao_texture = AssetLoader<Texture2D>::get(data.value("ao_texture", ""));
        emission_texture = AssetLoader<Texture2D>::get(data.value("emission_texture", ""));
        
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));;

    }



    
}