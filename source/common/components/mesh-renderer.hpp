#pragma once

#include "../ecs/component.hpp"
#include "../mesh/mesh.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our
{
    enum CollidingType
    {
        BOUNDARY = 0,
        COLLECTABLE = 1,
        IGNORE = 2,
        AVOIDABLE = 3,
    };

    enum RenderKind
    {
        MAIN = 0,
        PRESENT = 1,
        HEART = 2,
    };

    // This component denotes that any renderer should draw the given mesh using the given material at the transformation of the owning entity.
    class MeshRendererComponent : public Component
    {
    public:
        Mesh *mesh;                  // The mesh that should be drawn
        Material *material;          // The material used to draw the mesh
        CollidingType collidingType; // collidig type of the mesh renderer
        RenderKind kind;             // to know if its the main character
        bool hidden = false;

        // colliding variables

        // The ID of this component type is "Mesh Renderer"
        static std::string getID() { return "Mesh Renderer"; }

        // Receives the mesh & material from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json &data) override;
    };

}