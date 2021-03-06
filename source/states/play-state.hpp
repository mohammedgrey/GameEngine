#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <systems/random-movement.hpp>
#include <systems/collider.hpp>
#include <asset-loader.hpp>
#include "../game/Game.hpp"

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::RandomMovementSystem randomMovementSystem;
    our::ColliderSystem colliderSystem;
    our::Game game;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }

        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
    }

    void onDraw(double deltaTime) override
    {
        // In every frame we check if the game is lost or not toc change to lost state
        if (game.state == LOST)
        {

            getApp()->changeState("lost-test");
        }
        // In every frame we check if the game is won or not toc change to won state

        else if (game.state == WON)
        {

            getApp()->changeState("congrats-test");
        }
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        colliderSystem.update(&world, (float)deltaTime, &game);
        randomMovementSystem.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        auto size = getApp()->getFrameBufferSize();
        renderer.render(&world, glm::ivec2(0, 0), size);
    }

    void onDestroy() override
    {
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        world.clear();
        game.resetGame();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};