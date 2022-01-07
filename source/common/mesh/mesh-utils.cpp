#include "mesh-utils.hpp"

// We will use "Tiny OBJ Loader" to read and process '.obj" files
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include <iostream>
#include <vector>
#include <unordered_map>

our::Mesh *our::mesh_utils::loadOBJ(const char *filename)
{

    // The data that we will use to initialize our mesh
    std::vector<our::Vertex> vertices;
    std::vector<GLuint> elements;

    // Since the OBJ can have duplicated vertices, we make them unique using this map
    // The key is the vertex, the value is its index in the vector "vertices".
    // That index will be used to populate the "elements" vector.
    std::unordered_map<our::Vertex, GLuint> vertex_map;

    // The data loaded by Tiny OBJ Loader
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename))
    {
        std::cerr << "Failed to load obj file \"" << filename << "\" due to error: " << err << std::endl;
        return nullptr; // was false and now nullptr
    }
    if (!warn.empty())
    {
        std::cout << "WARN while loading obj file \"" << filename << "\": " << warn << std::endl;
    }

    // An obj file can have multiple shapes where each shape can have its own material
    // Ideally, we would load each shape into a separate mesh or store the start and end of it in the element buffer to be able to draw each shape separately
    // But we ignored this fact since we don't plan to use multiple materials in the examples

    // for getting the bounding box
    glm::vec3 min = {INT16_MAX, INT16_MAX, INT16_MAX};
    glm::vec3 max = {INT16_MIN, INT16_MIN, INT16_MIN};

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {

            Vertex vertex = {};

            // Read the data for a vertex from the "attrib" object
            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};

            // for getting the bounding box (min,max)
            if (vertex.position.x > max.x)
                max.x = vertex.position.x;
            if (vertex.position.y > max.y)
                max.y = vertex.position.y;
            if (vertex.position.z > max.z)
                max.z = vertex.position.z;
            if (vertex.position.x < min.x)
                min.x = vertex.position.x;
            if (vertex.position.y < min.y)
                min.y = vertex.position.y;
            if (vertex.position.z < min.z)
                min.z = vertex.position.z;

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]};

            vertex.tex_coord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {
                attrib.colors[3 * index.vertex_index + 0] * 255,
                attrib.colors[3 * index.vertex_index + 1] * 255,
                attrib.colors[3 * index.vertex_index + 2] * 255,
                255};

            // See if we already stored a similar vertex
            auto it = vertex_map.find(vertex);
            if (it == vertex_map.end())
            {
                // if no, add it to the vertices and record its index
                auto new_vertex_index = static_cast<GLuint>(vertices.size());
                vertex_map[vertex] = new_vertex_index;
                elements.push_back(new_vertex_index);
                vertices.push_back(vertex);
            }
            else
            {
                // if yes, just add its index in the elements vector
                elements.push_back(it->second);
            }
        }
    }

    Mesh *mesh = new our::Mesh(vertices, elements);
    mesh->setBoundingBox(min, max);

    return mesh;
}