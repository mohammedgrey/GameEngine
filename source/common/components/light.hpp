#pragma once

#include "../ecs/component.hpp"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>


namespace our {

    // An enum that defines the type of the light (SPOT or DIRECTIONAL or POINT) 
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    // This component denotes that any renderer should draw the scene affected by this light
    class LightComponent : public Component {
    public:
        LightType lightType;    // The type of the light
        glm::vec3 color;        //The color of the light
        glm::vec3 attenuation;  //Attenuation for both point and spot, decrease the intensity as the distance increase
        glm::vec2 cone_angles;  //Cone angles for the spot light, both the inner and outer 
        
        // The ID of this component type is "light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}