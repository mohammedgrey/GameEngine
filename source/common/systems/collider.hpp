#pragma once

#include "../ecs/world.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
using namespace our;

// The collider system is responsible for moving every entity which contains a MeshRendererComponent.
// This system is added as a simple example for how use the ECS framework to implement logic.
// For more information, see "common/components/collider.hpp"
class ColliderSystem
{

    Entity *cameraEntity = nullptr;
    Entity *mainCharacterEntity = nullptr;

public:
    // This should be called every frame to update all entities containing a MeshRendererComponent.
    void update(World *world, float deltaTime)
    {
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
                        if (hasMeshRenderer)
                        {
                            cameraEntity = entity->parent;
                            mainCharacterEntity = entity;
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
                    handleCollectableCollision(collider, entity);
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

        // std::cout
        //     << "x of main player min" << boundingBoxOfTheMainCharacterTransformedMin.x << std::endl;
        // std::cout << "y of main player min" << boundingBoxOfTheMainCharacterTransformedMin.y << std::endl;
        // std::cout << "z of main player min" << boundingBoxOfTheMainCharacterTransformedMin.z << std::endl;

        // std::cout << "x of main player max" << boundingBoxOfTheMainCharacterTransformedMax.x << std::endl;
        // std::cout << "y of main player max" << boundingBoxOfTheMainCharacterTransformedMax.y << std::endl;
        // std::cout << "z of main player max" << boundingBoxOfTheMainCharacterTransformedMax.z << std::endl;

        // std::cout << "x of wall min" << boundingBoxOfTheBoundaryTransformedMin.x << std::endl;
        // std::cout << "y of wall min" << boundingBoxOfTheBoundaryTransformedMin.y << std::endl;
        // std::cout << "z of wall min" << boundingBoxOfTheBoundaryTransformedMin.z << std::endl;

        // std::cout << "x of wall max" << boundingBoxOfTheBoundaryTransformedMax.x << std::endl;
        // std::cout << "y of wall max" << boundingBoxOfTheBoundaryTransformedMax.y << std::endl;
        // std::cout << "z of wall max" << boundingBoxOfTheBoundaryTransformedMax.z << std::endl;

        if (isOverLapped(boundingBoxOfTheMainCharacterTransformedMax, boundingBoxOfTheMainCharacterTransformedMin, boundingBoxOfTheBoundaryTransformedMax, boundingBoxOfTheBoundaryTransformedMin))
        {
            // std::cout << "overlapped " << std::endl;
            // mainCharacterEntity->localTransform = mainCharacterEntity->lastNonCollidedLocalTransform;
            cameraEntity->localTransform = cameraEntity->lastNonCollidedLocalTransform;
        }
        else
        {
            // std::cout << "not overlapped " << std::endl;
            // mainCharacterEntity->lastNonCollidedLocalTransform = mainCharacterEntity->getLocalToWorldMatrix();
            cameraEntity->lastNonCollidedLocalTransform = cameraEntity->localTransform;
        }
    }

    void handleCollectableCollision(MeshRendererComponent *collider, Entity *entity)
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

        if (isOverLapped(boundingBoxOfTheMainCharacterTransformedMax, boundingBoxOfTheMainCharacterTransformedMin, boundingBoxOfTheBoundaryTransformedMax, boundingBoxOfTheBoundaryTransformedMin))
        {
            collider->hidden = true;
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
};