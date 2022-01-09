#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "deserialize-utils.hpp"

namespace our
{
    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        std::string lightTypeStr = data.value("lightType", "directional");
        if (lightTypeStr == "directional")
        {
            lightType = LightType::DIRECTIONAL;
        }
        else if(lightTypeStr == "spot")
        {
            lightType = LightType::SPOT;
        }
        else if(lightTypeStr == "point")
        {
            lightType = LightType::POINT;
        }

        color = data.value("color", glm::vec3(1, 0.9,0.7));
        attenuation = data.value("attenuation", glm::vec3(0, 0, 1));
        cone_angles = glm::radians(data.value("cone_angles", glm::vec2(15.0f,30.0f)));
    }
}