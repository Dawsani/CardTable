#include <iostream>
#include <fstream>
#include <cstring>
#include "stb_image.h"

class Utils {
    public:
    // Reads the data in file filename, puts the data in output, returns false if data cannot be read.
    static bool readTextFromFile(const char *filename, char* &output);

    static unsigned int LoadTexture(const char* filename);

    static unsigned int loadModel(const char* filename);
};

inline unsigned int Utils::loadModel(const char* filename) {
    // get vertex data from file
    char* fileString;
    readTextFromFile(filename, fileString);
    char* c = fileString;
    while (*c != '\0') {
        std::cout << *c;
        c++;
    }

    float vertices[] = {
        -10.0f, 0.0f, -10.0f, 0.0f, 0.0f,
        10.0f, 0.0f, -10.0f, 20.0f, 0.0f,
        -10.0f,  0.0f, 10.0f, 0.0f, 20.0f,
        10.0f,  0.0f, 10.0f, 20.0f, 20.0f
    };

    unsigned int indices[] = {
        0, 1, 2, 3
    };

    // Generate vertex array and buffers
    unsigned int VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);   // vertex postion
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // texture coordinate
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}

inline unsigned int Utils::LoadTexture(const char* filename) {
    unsigned int textureHandle = 0;

    stbi_set_flip_vertically_on_load(true);
    
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);

    // Load the texture image
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        // Upload the image to the GPU
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate Mipmaps
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return textureHandle;
}

inline bool Utils::readTextFromFile(const char *filename, char* &output) {
    std::string buf = std::string("");
    std::string line;

    std::ifstream in(filename);
    if( !in.is_open() ) {
    	fprintf( stderr, "[ERROR]: Could not open file %s\n", filename );
    	return false;
    }
    while( std::getline(in, line) ) {
        buf += line + "\n";
    }
	
	output = new char[buf.length()+1];
	strncpy(output, buf.c_str(), buf.length());
	output[buf.length()] = '\0';
	
    in.close();
	
	return true;
}