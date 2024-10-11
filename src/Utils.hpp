#include <iostream>
#include <fstream>
#include <cstring>

class Utils {
    public:
    // Reads the data in file filename, puts the data in output, returns false if data cannot be read.
    static bool readTextFromFile(const char *filename, char* &output);
};

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