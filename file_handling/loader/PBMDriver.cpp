#include "PBMImageLoader.hpp"
#include <iostream>
using namespace std;

int main() {
    string filename;
    cin >> filename;
    PBMImageLoader loader = PBMImageLoader();
    loader.load(filename);
}