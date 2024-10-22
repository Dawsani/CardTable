#include "Utils.h"
#include <filesystem>
#include "Engine.h"

// Callback function to write data to a file
size_t Utils::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::ofstream* ofs = static_cast<std::ofstream*>(userp);
    size_t totalSize = size * nmemb;
    ofs->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to download a card image given a set code and collector number
bool Utils::downloadCardImage(const std::string& setCode, const std::string& collectorNumber, const std::string& outputFilename) {
    CURL* curl;
    CURLcode res;
    std::ofstream ofs(outputFilename, std::ios::binary);
    
    if (!ofs.is_open()) {
        std::cerr << "Failed to open file for writing: " << outputFilename << std::endl;
        return false;
    }

    curl = curl_easy_init();
    if(curl) {
        // Construct the URL
        std::string url = "https://api.scryfall.com/cards/" + setCode + "/" + collectorNumber + "?format=image";

        // Set curl options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // Follow redirects
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // Set write callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);  // Pass the file stream to the callback

        // Add required headers
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "User-Agent: CardTable/0.1");
        headers = curl_slist_append(headers, "Accept: */*");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl);
            return false;
        }

        // Clean up
        curl_slist_free_all(headers);  // Free the header list
        curl_easy_cleanup(curl);
        ofs.close();
        return true;
    }
    
    std::cerr << "Failed to initialize CURL." << std::endl;
    return false;
}

Card* Utils::findHighestCard(std::vector<Card*> cards) {
    if (cards.size() == 0) {
        return nullptr;
    }

    Card* highestCard = cards[0];
    for (int i = 0; i < cards.size(); i++) {
        Card* card = cards[i];
        if (card->getPosition().y > highestCard->getPosition().y) {
            highestCard = card;
        }
    }

    return highestCard;
}

bool fileExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}

std::deque<Card *> Utils::readCardsFromFile(Engine* pEngine, std::string filename, ShaderProgram* pShaderProgram, ShaderProgram* pScreenSpaceShaderProgram, unsigned int vaoHandle, unsigned int numVAOPoints, glm::vec2 hitBoxSize)
{
    std::deque<Card*> cards;

    std::ifstream file(filename);

    std::string line;

    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return cards;
    }

    while (getline(file, line)) {
        // assume the txt format for now
        // split it into it's parts
        std::vector<std::string> words = splitString(line, ' ');
        unsigned int numCopies = std::stoi(words[0].substr(words[0].size() - 2));

        // combine words until you hit a word starting with (
        std::string cardName = words[1];
        char firstLetter = words[2][0];
        int wordIndex = 2;
        bool validFormat = true;
        while (firstLetter != '(') {
            cardName += words[wordIndex];
            wordIndex++;
            if (wordIndex >= words.size()) {
                std::cout << "Improper formatting in file " << filename << std::endl;
                validFormat = false;
                break;
            }
            firstLetter = words[wordIndex][0];
        }
        if (!validFormat) {
            continue;
        }
        std::string setCode = words[wordIndex];
        setCode = setCode.substr(1, setCode.size() - 2);
        std::string collectorNumber = words[wordIndex + 1];
        std::string tags = words[wordIndex + 2];

        std::string outputFileName = "assets/cards/" + setCode + '_' + collectorNumber + ".jpg";

        // check if the file is already downloaded
        if (!fileExists(outputFileName)) {
            // download the file
            std::cout << "Downloading " << outputFileName << "...";
            Utils::downloadCardImage(setCode, collectorNumber, outputFileName);
            std::cout << "Done!" << std::endl;
        }
        
        // create a texture from the file
        unsigned int textureId = LoadTexture(outputFileName.c_str());
        for (unsigned int i = 0; i < numCopies; i++) {
            Card* newCard = new Card(pEngine, pShaderProgram, pScreenSpaceShaderProgram, vaoHandle, numVAOPoints, textureId, hitBoxSize);
            newCard->setName(cardName);
            cards.push_back(newCard);
        }
    }

    file.close();

    return cards;
}

std::vector<std::string> Utils::splitString(std::string string, char delimiter) {
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

void Utils::loadModel(const char* filename, unsigned int& vao, unsigned int& numVAOPoints) {

    std::cout << "Loading model " << filename << std::endl;

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
        }
        else {
            line += *c;
        }
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
            Face newFace;
            for (int i = 0; i < 3; i++) {
                std::vector<std::string> attributes = splitString(words[i + 1], '/');
                VertexAttributes newVertex;
                newVertex.vertexIndex = std::stof(attributes[0]);
                newVertex.textureCoordinateIndex = std::stof(attributes[1]);
                newVertex.normalIndex = std::stof(attributes[2]);
                newFace.vertices[i] = newVertex;
            }
            faces.push_back(newFace);
        }
    }
    
    // x vertices and 5 floats ver vertex
    const int finalVerticesSize = 5 * 3 * faces.size();
    float finalVertices[finalVerticesSize];
    for (unsigned int i = 0; i < faces.size(); i++) {
        Face f = faces[i];
        for (unsigned int j = 0; j < 3; j++) {
            unsigned int vertexIndex = f.vertices[j].vertexIndex - 1;
            unsigned int textureCoordinateIndex = f.vertices[j].textureCoordinateIndex - 1;
            glm::vec3 v = vertices[vertexIndex];
            glm::vec2 vt = textureCoordinates[textureCoordinateIndex];
            finalVertices[5 * j + 15 * i + 0] = v.x;
            finalVertices[5 * j + 15 * i + 1] = v.y;
            finalVertices[5 * j + 15 * i + 2] = v.z;
            finalVertices[5 * j + 15 * i + 3] = vt.x;
            finalVertices[5 * j + 15 * i + 4] = vt.y;
        }
    }

    // set up the indices just in case
    unsigned int indices[finalVerticesSize / 5];
    for (int i = 0; i < finalVerticesSize / 5; i++) {
        indices[i] = i;
    }

    // Generate vertex array and buffers
    unsigned int VAO, VBO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(finalVertices), finalVertices, GL_STATIC_DRAW);

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

    vao = VAO;
    numVAOPoints = finalVerticesSize;
}

unsigned int Utils::LoadTexture(const char* filename) {
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

glm::vec3 Utils::calculateCursorRay(glm::vec2 windowSize, glm::vec2 mousePosition, Camera* pCamera)
{
    // get mouse coordinates in normalized device coords, but really clip space
    glm::vec2 cursorPositionNDC = glm::vec2(2 * mousePosition.x / windowSize.x - 1, 1 - 2 * mousePosition.y / windowSize.y);
    glm::vec4 cursorPositionClipSpace = glm::vec4(cursorPositionNDC.x, cursorPositionNDC.y, -1.0f, 1.0f);
    glm::mat4 inverseProjectionMatrix = glm::inverse(pCamera->getProjectionMatrix());
    glm::vec4 cursorPositionViewSpace = inverseProjectionMatrix * cursorPositionClipSpace;
    glm::vec4 cursorRayViewSpace = glm::vec4(cursorPositionViewSpace.x, cursorPositionViewSpace.y, -1.0f, 0.0f);

    glm::mat4 inverseViewMatrix = glm::inverse(pCamera->getViewMatrix());
    glm::vec3 cursorRay = glm::normalize(inverseViewMatrix * cursorRayViewSpace);

    // flip some directions for some reasson
    return cursorRay;
}

glm::vec3 Utils::calculateRayIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection)
{
    rayDirection = glm::normalize(rayDirection);
    float t = - rayOrigin.z / rayDirection.z;
    glm::vec3 intersectionPoint = rayOrigin + rayDirection * t;

    // TODO: FIGURE OUT WHY I NEED TO MULTIPLY BY 2 RN.
    return intersectionPoint;
}

Card *Utils::findHoveredCard(glm::vec2 windowSize, glm::vec2 mousePosition, Camera *pCamera, std::vector<Card *> cards)
{
    glm::vec3 cursorRay = calculateCursorRay(windowSize, mousePosition, pCamera);
    glm::vec3 intersectionPoint = calculateRayIntersection(pCamera->getPosition(), cursorRay);

    std::vector<Card*> cardsUnderCursor;
    for (int i = 0; i < cards.size(); i++) {
        Card* card = cards[i];
        // check to see if the click selected a card
        glm::vec2 groundIntersection = glm::vec2(intersectionPoint.x, intersectionPoint.z);
        if (groundIntersection.x < card->getPosition().x + card->getHitBox()->getSize().x && groundIntersection.x > card->getPosition().x && 
            intersectionPoint.z > card->getPosition().z - card->getHitBox()->getSize().y && intersectionPoint.z < card->getPosition().z) {
                cardsUnderCursor.push_back(card);
        }
    }
    Card* highestCard = Utils::findHighestCard(cardsUnderCursor);

    if (highestCard != nullptr) {
        return highestCard;
    }

    return nullptr;
}

GameObject *Utils::findHoveredGameObject(glm::vec2 windowSize, glm::vec2 mousePosition, Camera *pCamera, std::vector<GameObject *> gameObjects)
{
    // there are different ways of seeing what is being hovered dependant on the game object.
    // every game object needs a way to check if it was collided with by a ray
    // cards in your hand are always in front of everythin, the deck should, but is not guranteed to be closer than a card
    // game objects should return how close to the camera they are somehow

    glm::vec3 rayDirection = calculateCursorRay(windowSize, mousePosition, pCamera);

    GameObject* closestHoveredObject = nullptr;
    float closestDistance = -1;
    for (GameObject* g : gameObjects) {
        float distanceFromCamera = g->checkRayCollision(pCamera->getPosition(), rayDirection);
        if (distanceFromCamera >= 0 && 
            (distanceFromCamera < closestDistance || closestDistance == -1)) {
                closestDistance = distanceFromCamera;
                closestHoveredObject = g;
        }
    }

    if (closestHoveredObject) {
        std::cout << closestHoveredObject->getName() << std::endl;
    }

    return closestHoveredObject;
}

bool Utils::readTextFromFile(const char *filename, char *&output)
{
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