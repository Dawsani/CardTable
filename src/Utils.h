#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "stb_image.h"
#include "Card.h"

class Utils {
    public:

    // Reads the data in file filename, puts the data in output, returns false if data cannot be read.
    static bool readTextFromFile(const char *filename, char* &output);

    static unsigned int LoadTexture(const char* filename);

    static void loadModel(const char* filename, unsigned int& vao, unsigned int& numVAOPoints);

    static std::vector<std::string> splitString(std::string string, char delimiter);
};