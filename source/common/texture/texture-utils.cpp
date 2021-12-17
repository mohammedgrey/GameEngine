#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

glm::ivec2 our::texture_utils::loadImage(Texture2D &texture, const char *filename, bool generate_mipmap)
{
    glm::ivec2 size;
    int channels;
    // Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    // We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    // Load image data and retrieve width, height and number of channels in the image
    // The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    // Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char *data = stbi_load(filename, &size.x, &size.y, &channels, 4);
    if (data == nullptr)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return {0, 0};
    }
    // Bind the texture such that we upload the image data to its storage
    // TODO: Finish this function
    // HINT: The steps should be as follows: bind the texture, send the pixel data to the GPU, then generate the mipmap (if requested).
    texture.bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void *)data);
    /*
    (GL_TEXTURE_2D)--> talks to the texture bound to the GL_TEXTURE_2D
    (0)--> level (if 0 takes the image as it is, 1 creates small size of image of 1/2 W and 1/2 H)
    every time we increase the level the image size reduces to quarter its area.
    (GL_RGBA8) --> the 8 represents how many bits stored in each color channel, since each element
    in data is unsigned char therefore occupies 1 byte (8 bits)
    (size.x) --> width of the image
    (size.y) --> height of the image
    (0) --> order, must be 0, useless parameter.
    (GL_RGBA) --> the order of the channels is R then G then B then A
    (GL_UNSIGNED_BYTE) --> type of each channel
    ((void*)data) --> an array of unsigned char of size width*height*4
    where 4 is the number of channels
    */
    if (generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D); // required if the filter is linear for example
    // a mipmap is just a smaller version of the image
    // generates dfferent images for different levels

    stbi_image_free(data); // Free image data after uploading to GPU
    return size;
}