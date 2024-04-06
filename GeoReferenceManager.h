// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <array>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeoReferencingSystem.h"
#include "GeoReferenceManager.generated.h"


struct GridParameters {
    double numColumns;
    double numRows;
    double resolutionX;
    double resolutionY;
    int coordinateSystemIDNative;
    double topEdgeNative;
    double bottomEdgeNative;
    double leftEdgeNative;
    double rightEdgeNative;
    int coordinateSystemIDWGS84;
    double topEdgeWGS84;
    double bottomEdgeWGS84;
    double leftEdgeWGS84;
    double rightEdgeWGS84;

    // Function to access cell based on x, y coordinates (in the specified coordinate system)
    std::pair<int, int> getCellFromCoordinates(float x, float y) {
        // Assuming x, y are in the native coordinate system for this example
        int columnIndex = (x - leftEdgeNative) /resolutionX;
        int rowIndex = (topEdgeNative - y) /std::abs(resolutionY); // Use abs because resolutionY could be negative
        return { rowIndex, columnIndex };
    }
};

struct GridCellInfo {
    int row;
    int col;
    float weight; // Weight percentage
};



UCLASS()
class GEOREFPLUGIN_API AGeoReferenceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeoReferenceManager();
   // float TimeSinceLastFireDataUpdate;
	GridParameters gridParams;
    TArray<TArray<FireData>> FireDataGrid;
    FireData GetFireData();
    FireData BilinearInterpolate(const std::array<GridCellInfo, 4>& gridCells);
    std::array<GridCellInfo, 4> calculateSurroundingCellsAndWeights(float currentLat, float currentLon);



protected:

	AGeoReferencingSystem* MyGeoRefSystem;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReadAndDeserializeBinaryFile(const FString& FilePath);
	void parseCSVToGridParameters(const FString& filePath);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
