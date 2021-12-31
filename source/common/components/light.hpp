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

    // This component denotes that any renderer should draw the scene relative to this light.
    // We do not define the eye, center or up here since they can be extracted from the entity local to world matrix
    class LightComponent : public Component {
    public:
        LightType lightType; // The type of the light
        glm::vec3 color;
        glm::vec3 attenuation;
        glm::vec2 cone_angles;
        
        // The ID of this component type is "light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}