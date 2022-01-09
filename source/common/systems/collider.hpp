#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"
#include "../game/Game.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <time.h>
#include <vector>

#define RECOVERY_SECONDS 5

// The collider system is responsible for detacting any type of collision between the main character and
//  any other MeshRederer component that has a colliding type anything other than ignore
// the possible colliding types are:
//  BOUNDARY = 0
//  COLLECTABLE = 1
//  IGNORE = 2
//  AVOIDABLE = 3
namespace our
{
    class ColliderSystem
    {

        Entity *cameraEntity = nullptr;
        Entity *mainCharacterEntity = nullptr;
        time_t currentTime;
        time_t collisionTime;
        std::vector<MeshRendererComponent *> heartComponents;
        std::vector<MeshRendererComponent *> presentComponents;

    public:
        // this is called each frame to detect any collision happening between the main character
        // and other mesh renderer components in the scene
        void
        update(World *world, float deltaTime, Game *game)
        {
            currentTime = time(NULL);

            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;

            // fisrt I need to get
            // 1) Camera
            // 2) Main character
            // 3) presents
            // 4) hearts
            if (!cameraEntity)
            {
                for (auto entity : world->getEntities())
                {
                    // If I have a parent
                    if (entity->parent)
                    {
                        camera = entity->parent->getComponent<CameraComponent>();
                        controller = entity->parent->getComponent<FreeCameraControllerComponent>();

                        // And my parent is the camera, then I am either the main character, a heart, or a present
                        if (camera && controller)
                        {
                            MeshRendererComponent *hasMeshRenderer = entity->getComponent<MeshRendererComponent>();
                            // If I am the main character
                            if (hasMeshRenderer && hasMeshRenderer->kind == MAIN)
                            {
                                // Set the main charcter and camera
                                cameraEntity = entity->parent;
                                mainCharacterEntity = entity;
                            }
                            // If I am a heart
                            else if (hasMeshRenderer && hasMeshRenderer->kind == HEART)
                            {
                                // append to the list of hearts
                                heartComponents.push_back(hasMeshRenderer);
                            }
                            // If I am a present
                            else if (hasMeshRenderer && hasMeshRenderer->kind == PRESENT)
                            {
                                // append to the list of presents
                                presentComponents.push_back(hasMeshRenderer);
                            }
                        }
                    }
                }
            }
            //  For each entity in the world
            for (auto entity : world->getEntities())
            {
                // Get the MeshRenderer component if it exists
                MeshRendererComponent *collider = entity->getComponent<MeshRendererComponent>();

                // if there is a mesh renderer then chack its colliding type
                if (collider)
                {
                    switch (collider->collidingType)
                    {
                    case BOUNDARY:
                        handleBoundaryCollision(collider, entity);
                        break;
                    case COLLECTABLE:
                        handleCollectableCollision(collider, entity, game);
                        break;
                    case AVOIDABLE:
                        handleAvoidableCollision(collider, entity, game);
                        break;
                    case IGNORE:
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        void handleBoundaryCollision(MeshRendererComponent *collider, Entity *entity)
        {

            if (isCollided(collider, entity))
                cameraEntity->localTransform = entity->lastNonCollidedLocalTransform;

            else
            { // As long as I am moving and I haven't collided
                // then set the last non collided place of the entity with the place of the camera
                // but shifted a little based on the direction
                // here the shift is done to create a ripple effect when the character collides
                // to make the character step back a little based on the ammount of shift
                Transform shiftedNonCollided = cameraEntity->localTransform;
                if (!entity->localTransform.position.x == 0)
                    shiftedNonCollided.position.x += shiftedNonCollided.position.x < 0 ? 1 : -1;
                if (!entity->localTransform.position.z == 0)
                    shiftedNonCollided.position.z += shiftedNonCollided.position.z < 0 ? 1 : -1;
                entity->lastNonCollidedLocalTransform = shiftedNonCollided;
            }
        }

        void handleCollectableCollision(MeshRendererComponent *collider, Entity *entity, Game *game)
        {
            // If I have collided with a collectable item and its not hidden yet
            if (isCollided(collider, entity) && !collider->hidden)
            {
                // then hide that item
                collider->hidden = true;
                // and increase the amount of collected presents
                // this function also changes the game state if the presents collected equal to the presents goal
                game->incrementPresents();
                if (presentComponents.size())
                { // to add the collected presents at the top of the screen
                    presentComponents[presentComponents.size() - 1]->hidden = false;
                    presentComponents.pop_back();
                }
            }
        }

        void handleAvoidableCollision(MeshRendererComponent *collider, Entity *entity, Game *game)
        {
            // If I have collided with an avoidable item and the recovery time has passed from the last time I collided
            if (isCollided(collider, entity) && currentTime - collisionTime > RECOVERY_SECONDS) //  Recovery time is defined as 5 seconds here
            {
                // Set the collision time, this is useful for the next chack to allow for recovery time from the colllision
                collisionTime = time(NULL);
                // and decrease the hearts the character has
                // this function also changes the game state if player has lost all its hearts
                game->decrementHearts();
                if (heartComponents.size())
                { // remove a heart from the top screen
                    heartComponents[heartComponents.size() - 1]->hidden = true;
                    heartComponents.pop_back();
                }
            }
        }

        // This function transforms the bounding box from the object space to the world space
        glm::vec3 getTransformedBoundingBox(glm::mat4 transformationMat, glm::vec3 boundingBox)
        {
            glm::vec4 pointMin = transformationMat * glm::vec4(boundingBox, 1.0);
            return glm::vec3(pointMin.x, pointMin.y, pointMin.z);
        }

        // This checks if two bounding boxes are overlapped with normal logical checks
        bool isOverLapped(const glm::vec3 &p1_max, const glm::vec3 &p1_min, const glm::vec3 &p2_max, const glm::vec3 &p2_min)
        {
            return (p1_min.x <= p2_max.x && p1_max.x >= p2_min.x) && (p1_min.y <= p2_max.y && p1_max.y >= p2_min.y) && (p1_min.z <= p2_max.z && p1_max.z >= p2_min.z);
        }

        // The aim of this function is to get the bounding boxes of the moving entities
        // as their bounding boxes change as long as they are moving
        bool isCollided(MeshRendererComponent *collider, Entity *entity)
        {
            // First get the non transformed bounding box of the main character and the entity I want to chack my collison with in the object space
            // 1) Non transformed main character bounding box
            glm::vec3 boundingBoxOfTheMainCharacterMin = mainCharacterEntity->getComponent<MeshRendererComponent>()->mesh->getBoundingBoxMin();
            glm::vec3 boundingBoxOfTheMainCharacterMax = mainCharacterEntity->getComponent<MeshRendererComponent>()->mesh->getBoundingBoxMax();
            // 2) Non transformed other entity bounding box
            glm::vec3 boundingBoxOfTheBoundaryMin = collider->mesh->getBoundingBoxMin();
            glm::vec3 boundingBoxOfTheBoundaryMax = collider->mesh->getBoundingBoxMax();

            // Then get the transformed bounding boxes in the world space
            // 1) Transformed main character bounding box
            glm::vec3 boundingBoxOfTheMainCharacterTransformedMin = getTransformedBoundingBox(mainCharacterEntity->getLocalToWorldMatrix(), boundingBoxOfTheMainCharacterMin);
            glm::vec3 boundingBoxOfTheMainCharacterTransformedMax = getTransformedBoundingBox(mainCharacterEntity->getLocalToWorldMatrix(), boundingBoxOfTheMainCharacterMax);
            // 2) Transformed other entity bounding box
            glm::vec3 boundingBoxOfTheBoundaryTransformedMin = getTransformedBoundingBox(entity->getLocalToWorldMatrix(), boundingBoxOfTheBoundaryMin);
            glm::vec3 boundingBoxOfTheBoundaryTransformedMax = getTransformedBoundingBox(entity->getLocalToWorldMatrix(), boundingBoxOfTheBoundaryMax);

            // Swap the min and max bounding boxes if the min is greater than the max
            if (boundingBoxOfTheMainCharacterTransformedMax.x < boundingBoxOfTheMainCharacterTransformedMin.x)
                std::swap(boundingBoxOfTheMainCharacterTransformedMax.x, boundingBoxOfTheMainCharacterTransformedMin.x);
            if (boundingBoxOfTheMainCharacterTransformedMax.y < boundingBoxOfTheMainCharacterTransformedMin.y)
                std::swap(boundingBoxOfTheMainCharacterTransformedMax.y, boundingBoxOfTheMainCharacterTransformedMin.y);
            if (boundingBoxOfTheMainCharacterTransformedMax.z < boundingBoxOfTheMainCharacterTransformedMin.z)
                std::swap(boundingBoxOfTheMainCharacterTransformedMax.z, boundingBoxOfTheMainCharacterTransformedMin.z);
            if (boundingBoxOfTheBoundaryTransformedMax.x < boundingBoxOfTheBoundaryTransformedMin.x)
                std::swap(boundingBoxOfTheBoundaryTransformedMax.x, boundingBoxOfTheBoundaryTransformedMin.x);
            if (boundingBoxOfTheBoundaryTransformedMax.y < boundingBoxOfTheBoundaryTransformedMin.y)
                std::swap(boundingBoxOfTheBoundaryTransformedMax.y, boundingBoxOfTheBoundaryTransformedMin.y);
            if (boundingBoxOfTheBoundaryTransformedMax.z < boundingBoxOfTheBoundaryTransformedMin.z)
                std::swap(boundingBoxOfTheBoundaryTransformedMax.z, boundingBoxOfTheBoundaryTransformedMin.z);

            // Check is the two bounding boxes are overlapped or not
            // overlapped--> true
            // not overlapped-->false
            return isOverLapped(boundingBoxOfTheMainCharacterTransformedMax, boundingBoxOfTheMainCharacterTransformedMin, boundingBoxOfTheBoundaryTransformedMax, boundingBoxOfTheBoundaryTransformedMin);
        }
    };
}