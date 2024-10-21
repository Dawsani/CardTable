#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <stack>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "stb_image.h"
#include "Card.h"
#include "Camera.h"
#include <curl/curl.h>

class Utils {
    public:

    //gameplay helpers
    static glm::vec3 calculateCursorRay(glm::vec2 windowSize, glm::vec2 mousePosition, Camera* pCamera);
    static glm::vec3 calculateRayIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection);
    static Card* findHoveredCard(glm::vec2 windowSize, glm::vec2 mousePosition, Camera* pCamera, std::vector<Card*> cards);

    // Reads the data in file filename, puts the data in output, returns false if data cannot be read.
    static bool readTextFromFile(const char *filename, char* &output);

    static unsigned int LoadTexture(const char* filename);

    static void loadModel(const char* filename, unsigned int& vao, unsigned int& numVAOPoints);

    static std::vector<std::string> splitString(std::string string, char delimiter);
    
    static Card* findHighestCard(std::vector<Card*> cards);

    static std::stack<Card *> readCardsFromFile(std::string filename, ShaderProgram* pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, glm::vec2 hitBoxSize);

    // file downloading
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static bool downloadCardImage(const std::string& setCode, const std::string& collectorNumber, const std::string& outputFilename);
};