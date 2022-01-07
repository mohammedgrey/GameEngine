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

using namespace our;

// The collider system is responsible for moving every entity which contains a MeshRendererComponent.
// This system is added as a simple example for how use the ECS framework to implement logic.
// For more information, see "common/components/collider.hpp"
class ColliderSystem
{

    Entity *cameraEntity = nullptr;
    Entity *mainCharacterEntity = nullptr;
    time_t currentTime;
    time_t collisionTime;
    std::vector<MeshRendererComponent *> heartComponents;

public:
    // This should be called every frame to update all entities containing a MeshRendererComponent.
    void
    update(World *world, float deltaTime, Game *game)
    {
        currentTime = time(NULL);
        // first get the camera entity
        CameraComponent *camera = nullptr;
        FreeCameraControllerComponent *controller = nullptr;

        if (!cameraEntity)
        {
            for (auto entity : world->getEntities())
            {
                if (entity->parent)
                {
                    camera = entity->parent->getComponent<CameraComponent>();
                    controller = entity->parent->getComponent<FreeCameraControllerComponent>();

                    if (camera && controller)
                    {
                        MeshRendererComponent *hasMeshRenderer = entity->getComponent<MeshRendererComponent>();
                        if (hasMeshRenderer && hasMeshRenderer->isMain)
                        {
                            cameraEntity = entity->parent;
                            mainCharacterEntity = entity;
                        }
                        else if (hasMeshRenderer)
                        {
                            heartComponents.push_back(hasMeshRenderer);
                        }
                    }
                }
            }
        }
        //  For each entity in the world
        for (auto entity : world->getEntities())
        {
            // Get the collider component if it exists
            MeshRendererComponent *collider = entity->getComponent<MeshRendererComponent>();
            // If the collider component exists
            if (collider)
            {
                switch (collider->collidingType)
                {
                case BOUNDARY:
                    // std::cout << "handle boundary " << std::endl;
                    handleBoundaryCollision(collider, entity);
                    break;
                case COLLECTABLE:
                    handleCollectableCollision(collider, entity, game);
                    /* code */
                    break;
                case AVOIDABLE:
                    handleAvoidableCollision(collider, entity, game);
                    /* code */
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
        static int counter = 0;

        if (isCollided(collider, entity))
        {
            counter++;

            std::cout << "overlapped " << counter << " times" << std::endl;
            cameraEntity->localTransform = entity->lastNonCollidedLocalTransform;
        }
        else
        {
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
        if (isCollided(collider, entity) && !collider->hidden)
        {
            collider->hidden = true;
            game->incrementPresents();
        }
    }

    void handleAvoidableCollision(MeshRendererComponent *collider, Entity *entity, Game *game)
    {
        if (isCollided(collider, entity) && currentTime - collisionTime > RECOVERY_SECONDS)
        {
            collisionTime = time(NULL);
            game->decrementHearts();
            if (heartComponents.size())
            { // remove the a heart from the screen
                heartComponents[heartComponents.size() - 1]->hidden = true;
                heartComponents.pop_back();
            }
        }
    }

    glm::vec3 getTransformedBoundingBox(glm::mat4 transformationMat, glm::vec3 boundingBox)
    {
        glm::vec4 pointMin = transformationMat * glm::vec4(boundingBox, 1.0);
        return glm::vec3(pointMin.x, pointMin.y, pointMin.z);
    }
    bool isOverLapped(const glm::vec3 &p1_max, const glm::vec3 &p1_min, const glm::vec3 &p2_max, const glm::vec3 &p2_min)
    {
        return (p1_min.x <= p2_max.x && p1_max.x >= p2_min.x) && (p1_min.y <= p2_max.y && p1_max.y >= p2_min.y) && (p1_min.z <= p2_max.z && p1_max.z >= p2_min.z);
    }
    bool isCollided(MeshRendererComponent *collider, Entity *entity)
    {
        glm::vec3 boundingBoxOfTheMainCharacterMin = mainCharacterEntity->getComponent<MeshRendererComponent>()->mesh->getBoundingBoxMin();
        glm::vec3 boundingBoxOfTheMainCharacterMax = mainCharacterEntity->getComponent<MeshRendererComponent>()->mesh->getBoundingBoxMax();
        glm::vec3 boundingBoxOfTheBoundaryMin = collider->mesh->getBoundingBoxMin();
        glm::vec3 boundingBoxOfTheBoundaryMax = collider->mesh->getBoundingBoxMax();

        glm::vec3 boundingBoxOfTheMainCharacterTransformedMin = getTransformedBoundingBox(mainCharacterEntity->getLocalToWorldMatrix(), boundingBoxOfTheMainCharacterMin);
        glm::vec3 boundingBoxOfTheMainCharacterTransformedMax = getTransformedBoundingBox(mainCharacterEntity->getLocalToWorldMatrix(), boundingBoxOfTheMainCharacterMax);
        glm::vec3 boundingBoxOfTheBoundaryTransformedMin = getTransformedBoundingBox(entity->getLocalToWorldMatrix(), boundingBoxOfTheBoundaryMin);
        glm::vec3 boundingBoxOfTheBoundaryTransformedMax = getTransformedBoundingBox(entity->getLocalToWorldMatrix(), boundingBoxOfTheBoundaryMax);

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

        return isOverLapped(boundingBoxOfTheMainCharacterTransformedMax, boundingBoxOfTheMainCharacterTransformedMin, boundingBoxOfTheBoundaryTransformedMax, boundingBoxOfTheBoundaryTransformedMin);
    }
};