#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

struct GridParameters {
    int numColumns;
    int numRows;
    float resolutionX;
    float resolutionY;
    int coordinateSystemIDNative;
    float topEdgeNative;
    float bottomEdgeNative;
    float leftEdgeNative;
    float rightEdgeNative;
    int coordinateSystemIDWGS84;
    float topEdgeWGS84;
    float bottomEdgeWGS84;
    float leftEdgeWGS84;
    float rightEdgeWGS84;

    // Function to access cell based on x, y coordinates (in the specified coordinate system)
    std::pair<int, int> getCellFromCoordinates(float x, float y) {
        // Assuming x, y are in the native coordinate system for this example
        int columnIndex = (x - leftEdgeNative) / resolutionX;
        int rowIndex = (topEdgeNative - y) / std::abs(resolutionY); // Use abs because resolutionY could be negative
        return { rowIndex, columnIndex };
    }
};
