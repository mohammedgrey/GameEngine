#pragma once

#include "../ecs/world.hpp"
#include "../components/random-movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <stdlib.h>
#include <time.h>

namespace our
{

    // The movement system is responsible for moving every entity which contains a RandomMovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic.
    // For more information, see "common/components/movement.hpp"

    class RandomMovementSystem
    {
        bool changeXvelocity = false;
        bool changeZvelocity = true;

    public:
        RandomMovementSystem()
        {
            srand(time(NULL));
        }
        // This should be called every frame to update all entities containing a RandomMovementComponent.
        void update(World *world, float deltaTime)
        {
            // For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the movement component if it exists
                RandomMovementComponent *movement = entity->getComponent<RandomMovementComponent>();
                // If the movement component exists
                if (movement)
                {
                    entity->localTransform.position += deltaTime * movement->linearVelocity * movement->direction;
                    if (entity->localTransform.position.x >= movement->maxBoundary.x || entity->localTransform.position.x <= movement->minBoundary.x)
                    {
                        movement->direction.x = -movement->direction.x;
                        changeDirection(entity, movement);
                    }
                    if (entity->localTransform.position.z >= movement->maxBoundary.z || entity->localTransform.position.z <= movement->minBoundary.z)
                    {
                        movement->direction.z = -movement->direction.z;
                        changeDirection(entity, movement);
                    }
                }
            }
        }

        void changeDirection(Entity *entity, RandomMovementComponent *movement)
        {
            if (changeXvelocity)
            {
                movement->linearVelocity.x = rand() % (int)movement->maxLinearVelocity.x;
                changeXvelocity = false;
                changeZvelocity = true;
            }
            if (changeZvelocity)
            {
                movement->linearVelocity.z = rand() % (int)movement->maxLinearVelocity.z;
                changeZvelocity = false;
                changeXvelocity = true;
            }

            // if (movement->linearVelocity.x == 0)
            // {
            //     entity->localTransform.rotation.y = movement->direction.z == -1 ? glm::radians(180.0) : 0.0;
            // }
            // else
            // {
            //     float extraAngle = 0;
            //     // if (movement->direction.z == -1 && movement->direction.x == -1)
            //     //     extraAngle = 90;
            //     // if (movement->direction.z == 1 && movement->direction.x == -1)
            //     //     extraAngle = 90;
            //     // if (movement->direction.z == 1 && movement->direction.x == 1)
            //     //     extraAngle = 270;
            //     // if (movement->direction.z == 1 && movement->direction.x == 1)
            //     //     extraAngle = 270;

            //     // std::cout << "extraAngle " << extraAngle << std::endl;
            //     entity->localTransform.rotation.y = glm::radians(extraAngle) + glm::atan((movement->linearVelocity.z) / (movement->linearVelocity.x));
            // }
        }
    };

}
