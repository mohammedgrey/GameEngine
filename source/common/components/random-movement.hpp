#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{

    // This component denotes that the MovementSystem will move the owning entity by a certain linear and angular velocity.
    // This component is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/movement.hpp"
    // For a more complex example of how to use the ECS framework, see "free-camera-controller.hpp"
    class RandomMovementComponent : public Component
    {
    public:
        glm::vec3 linearVelocity = {0, 0, 0}; // Each frame, the entity should move as follows: position += linearVelocity * deltaTime
        glm::vec3 maxLinearVelocity = {7, 0, 7};

        glm::vec3 minBoundary = {-100, -100, -100};
        glm::vec3 maxBoundary = {100, 100, 100};

        glm::vec3 direction = {1, 1, 1};
        // The ID of this component type is "Random Movement"
        static std::string getID() { return "Random Movement"; }

        // Reads linearVelocity & angularVelocity from the given json object
        void deserialize(const nlohmann::json &data) override
        {
            if (!data.is_object())
                return;
            linearVelocity = data.value("linearVelocity", linearVelocity);
            maxLinearVelocity = data.value("maxLinearVelocity", maxLinearVelocity);

            minBoundary = data.value("minBoundary", minBoundary);
            maxBoundary = data.value("maxBoundary", maxBoundary);
        };
    };

}