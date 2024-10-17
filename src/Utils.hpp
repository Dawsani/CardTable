#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "stb_image.h"

class Utils {
    public:
    // Reads the data in file filename, puts the data in output, returns false if data cannot be read.
    static bool readTextFromFile(const char *filename, char* &output);

    static unsigned int LoadTexture(const char* filename);

    static unsigned int loadModel(const char* filename);

    static std::vector<std::string> splitString(std::string string, char delimiter);
};

inline std::vector<std::string> Utils::splitString(std::string string, char delimiter) {
    std::vector<std::string> subStrings;
    std::string subString = "";
    for (char letter : string) {
        if (letter == delimiter) {
            if (subString != "") {
                subStrings.push_back(subString);
                subString = "";
            }
            continue;
        }
        subString += letter;
    }
    if (subString != "") {
        subStrings.push_back(subString);
        subString = "";
    }

    return subStrings;
}

inline unsigned int Utils::loadModel(const char* filename) {
    // get vertex data from file

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> textureCoordinates;
    std::vector<glm::vec3> normals;

    struct VertexAttributes {
        unsigned int vertexIndex;
        unsigned int textureCoordinateIndex;
        unsigned int normalIndex;
    };
    struct Face {
        VertexAttributes vertices[3];
    };
    std::vector<Face> faces;

    char* fileString;
    readTextFromFile(filename, fileString);

    // split the filestring into lines
    std::vector<std::string> lines;
    std::string line = "";
    char* c = fileString;
    while (*c != '\0') {
        if (*c == '\n') {
            if (line != "") {
                lines.push_back(line);
                line = "";
            }
            continue;
        }
        line += *c;
        c++;
    }
    if (line != "") {
        lines.push_back(line);
        line = "";
    }

    for (std::string line : lines) {
        // split the line by spaces
        std::vector<std::string> words = splitString(line, ' ');

        std::string lineType = words[0];
        if (lineType == "v") {
            glm::vec3 newVertex = glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
            vertices.push_back(newVertex);
        }
        else if (lineType == "vt") {
            glm::vec2 newTextureCoordinate = glm::vec2(std::stof(words[1]), std::stof(words[2]));
            textureCoordinates.push_back(newTextureCoordinate);
        }
        else if (lineType == "vn") {
            glm::vec3 newNormal = glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]));
            normals.push_back(newNormal);
        }
        else if (lineType == "f") {
            // split each face by slashes
            
        }
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