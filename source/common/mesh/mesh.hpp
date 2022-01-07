#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

        // add bounding box for the mesh
        glm::vec3 boundingBox[2]; // 0-->min, 1-->max

    public:
        void setBoundingBox(const glm::vec3 &min, const glm::vec3 &max)
        {

            this->boundingBox[0] = min;
            this->boundingBox[1] = max;
        }
        glm::vec3 getBoundingBoxMin()
        {
            return this->boundingBox[0];
        }

        glm::vec3 getBoundingBoxMax()
        {
            return this->boundingBox[1];
        }

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            this->elementCount = elements.size();
            GLsizei verticesCount = vertices.size();

            // Vertex Array
            // The first parameter is the number of vertex array need to be generated
            // The second is passed by reference, to be changed to a pointer on the vertex array
            glGenVertexArrays(1, &this->VAO); // get an id for the vertix array object
            glBindVertexArray(this->VAO);     // bind the vertex array object

            // Element buffer (EBO)
            glGenBuffers(1, &this->EBO);                      // get an id for the elements buffer
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); // bind the elements buffer
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elementCount * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
            //-----------------------------------size of elements array in bytes, reference to the elements array, Static:The data store contents will be modified once and used many times.

            // Vertex Buffer
            glGenBuffers(1, &this->VBO);              // get an id for the vertices buffer
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO); // bind the vertices buffer
            glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            //-----------------------------------size of vertices array in bytes, reference to the vertices array, Static:The data store contents will be modified once and used many times.

            // Attributes (Define the attributes for the vertex array object)
            /*
            glVertexAttribPointer doc:
            (index)
            Specifies the index of the generic vertex attribute to be modified.
            (size)
            postion=3 (x,y,z), color=4 (r,g,b,a)..etc
            (type)
            x=Float,y=Float,z=Float, r=Float,g=Float...etc
            (normalized)
            if values between 0 and 1, then it's already normalized and this is set to false
            if the values have some other we set this to true to tell gl to normalize them for us
            (stride)
            Byte offset between each Vertex and the next Vertex in the vertex array
            (pointer)
            offset of the attribute inside the vertex itself
            */
            // 1) Position Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION); // Specifies the index of the generic vertex attribute to be enabled or disabled.
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);

            // 2) Color Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // 3) Texture Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

            // 4) Normal Attribute
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        }
        // this function should render the mesh
        void draw()
        {

            // TODO: Write this function
            glBindVertexArray(this->VAO); // bind
            // what to draw, how many elements, type of each element, offset of the first index in the array=0
            glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, (void *)0); // Draw
            glBindVertexArray(0);                                                         // unbind
        }
        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: Write this function
            glDeleteVertexArrays(1, &this->VAO);
            glDeleteBuffers(1, &this->VBO);
            glDeleteBuffers(1, &this->EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}