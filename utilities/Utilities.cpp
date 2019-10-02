#include "Utilities.hpp"
using namespace std;

string Utilities::getFileExtension(std::string fileUrl) {
    string extension = fileUrl.substr(fileUrl.find(".") + 1, fileUrl.length());
    return extension;
}