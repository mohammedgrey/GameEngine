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
    class RandomMovementSystem
    {
        // Useful for allowing the changes to happen randomly in only one direction
        // to give a smoother movement
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
                // If the random movement component exists
                if (movement)
                {
                    // Then keep changing its location based on its linear velocity in the x and z direction
                    entity->localTransform.position += deltaTime * movement->linearVelocity * movement->direction;

                    // If I reached my min or max boundary in the x direction
                    if (entity->localTransform.position.x >= movement->maxBoundary.x || entity->localTransform.position.x <= movement->minBoundary.x)
                    {
                        // Then change my x direction to the opposite side
                        movement->direction.x = -movement->direction.x;
                        changeDirection(entity, movement);
                    }
                    // If I reached my min or max boundary in the x direction
                    if (entity->localTransform.position.z >= movement->maxBoundary.z || entity->localTransform.position.z <= movement->minBoundary.z)
                    {
                        // Then change my z direction to the opposite side
                        movement->direction.z = -movement->direction.z;
                        changeDirection(entity, movement);
                    }
                }
            }
        }

        void changeDirection(Entity *entity, RandomMovementComponent *movement)
        {
            // Check whether I should change the direction of the x or z
            //  Here we make them alternate each time the character collides
            if (changeXvelocity)
            {
                // cahnge the x velocity randomely from 0 to the max velocity alowed
                movement->linearVelocity.x = rand() % (int)movement->maxLinearVelocity.x;
                // Alternate for next time
                changeXvelocity = false;
                changeZvelocity = true;
            }
            if (changeZvelocity)
            {
                // cahnge the z velocity randomely from 0 to the max velocity alowed
                movement->linearVelocity.z = rand() % (int)movement->maxLinearVelocity.z;
                // Alternate for next time
                changeZvelocity = false;
                changeXvelocity = true;
            }
        }
    };

}
