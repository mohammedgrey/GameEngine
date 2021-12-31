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
        
        //Albedo Texture
        GLuint textureUnit = 0; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        albedo_texture->bind();
        albedo_sampler->bind(textureUnit);
        shader->set("albedo_tex", textureUnit);

        //Specular Texture
        textureUnit = 1; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        specular_texture->bind();
        specular_sampler->bind(textureUnit);
        shader->set("specular_tex", textureUnit);

        //Roughness Texture
        textureUnit = 2; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        roughness_texture->bind();
        roughness_sampler->bind(textureUnit);
        shader->set("roughness_tex", textureUnit);

        //Ambient Occlusion Texture
        textureUnit = 2; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        ao_texture->bind();
        ao_sampler->bind(textureUnit);
        shader->set("ao_tex", textureUnit);

        //Emission Texture
        textureUnit = 3; // this could be any number from 0 up to 31
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        emission_texture->bind();
        emission_sampler->bind(textureUnit);
        shader->set("emission_tex", textureUnit);
        
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
        
        albedo_sampler = AssetLoader<Sampler>::get(data.value("albedo_sampler", ""));;
        specular_sampler = AssetLoader<Sampler>::get(data.value("specular_sampler", ""));;
        roughness_sampler = AssetLoader<Sampler>::get(data.value("roughness_sampler", ""));;
        ao_sampler = AssetLoader<Sampler>::get(data.value("ao_sampler", ""));;
        emission_sampler = AssetLoader<Sampler>::get(data.value("emission_sampler", ""));;

    }



    
}