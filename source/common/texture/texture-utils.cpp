#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

glm::ivec2 our::texture_utils::loadImage(Texture2D& texture, const char *filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* data = stbi_load(filename, &size.x, &size.y, &channels, 4);
    if(data == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return {0, 0};
    }
    //Bind the texture such that we upload the image data to its storage
    //TODO:PDONE Finish this function
    //HINT: The steps should be as follows: bind the texture, send the pixel data to the GPU, then generate the mipmap (if requested).

    //CHECK__
    // Bind implemented in Texture2D class

    // We use the .bind and bind in the class itself since here we cannot access the variable name which is needed to bind our texture 
    texture.bind();
    // Now that the texture is bound, we can start generating a texture using the previously loaded image data
    // it needs us to pass 1. the texture target 2. minmap level (left as 0) 3. format, we use RGB as usual 
    // 4 and 5 for width and height, border = 0, then formats then the actual image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.x, size.y, 0, GL_RGBA,GL_UNSIGNED_BYTE, data);

    // Generates all the mipmaps for the image, the glTexImage2D only generates for level 0
    // mipmaps is basically a collection of texture images where each subsequent texture is twice as small compared to the previous one
    // OpenGL will use a different mipmap texture that best suits the distance to the object, mipmaps good for performance
    if(generate_mipmap ==  true){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
        
    
    // in case we need to unbind texture
    texture.unbind();

    //CHECK__end

    stbi_image_free(data); //Free image data after uploading to GPU
    return size;
}