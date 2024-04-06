// Fill out your copyright notice in the Description page of Project Settings.


#include "GeoReferenceManager.h"
#include "FireDataStruct.h"
#include "GeoReferencingSystem.h"
#include "Kismet/GameplayStatics.h"
#include <corecrt_math_defines.h>


// Sets default values
AGeoReferenceManager::AGeoReferenceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AGeoReferenceManager::BeginPlay()


{
	
	FString SectionName = TEXT("/Script/ThisIsTheOne.MyPluginSettings");
	FString KeyName = TEXT("lossless");
	FString ConfigFileName = GGameIni; // For game-specific settings
	FString FilePathValue;

	// Read the file path from the configuration file
	if (GConfig->GetString(*SectionName, *KeyName, FilePathValue, ConfigFileName))
	{



		// FilePathValue now contains the file path as a string, and you can use it here
		UE_LOG(LogTemp, Log, TEXT("File Path: %s"), *FilePathValue);

		ReadAndDeserializeBinaryFile(FilePathValue);
	}
	else
	{
		// Handle the case where the key is not found
		UE_LOG(LogTemp, Warning, TEXT("Failed to read the file path from the configuration."));
	}
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGeoReferencingSystem::StaticClass(), FoundActors);

	// Ensure that at least one instance was found
	if (FoundActors.Num() > 0)
	{

		MyGeoRefSystem = Cast<AGeoReferencingSystem>(FoundActors[0]);



	}

	parseCSVToGridParameters("C:/Users/hanna/OneDrive/Desktop/Data_to_use/test_fold/georef_info.csv");




}



void AGeoReferenceManager::ReadAndDeserializeBinaryFile(const FString& FilePath)
{
	TArray<uint8> BinaryData;
	if (FFileHelper::LoadFileToArray(BinaryData, *FilePath))
	{
		// Read the grid dimensions (first 16 bytes: two 64-bit integers)
		int64 NumRows, NumCols;
		FMemory::Memcpy(&NumRows, BinaryData.GetData(), sizeof(NumRows));
		FMemory::Memcpy(&NumCols, BinaryData.GetData() + sizeof(NumRows), sizeof(NumCols));

		// Offset to start reading grid data
		int32 ByteOffset = sizeof(NumRows) + sizeof(NumCols);

		int32 NumBytesPerInstance = sizeof(double) * 8; // 8 doubles per instance

		// Create a 2D array for FireData
		
		FireDataGrid.SetNum(NumRows);

		for (int32 i = 0; i < NumRows; ++i)
		{
			FireDataGrid[i].SetNum(NumCols);
			for (int32 j = 0; j < NumCols; ++j)
			{
				FireData DataInstance;

				// Deserialize each double into the struct
				FMemory::Memcpy(&DataInstance.elevation, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fuelload1, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fuelload10, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fuelload100, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fuelload1000, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fueldepth, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.slope, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				FMemory::Memcpy(&DataInstance.fuelsav, &BinaryData[ByteOffset], sizeof(double));
				ByteOffset += sizeof(double);

				// Add the deserialized instance to the grid
				FireDataGrid[i][j] = DataInstance;
				// Log the elevation data for each grid cell
			}
		}
		
	
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load binary file: %s"), *FilePath);
	}
}



void AGeoReferenceManager::parseCSVToGridParameters(const FString& FilePath) {


	TArray<FString> Lines;

	// Load the file into an array of FString
	FFileHelper::LoadFileToStringArray(Lines, *FilePath);

	for (FString& Line : Lines) {

		TArray<FString> KeyValue;
		Line.ParseIntoArray(KeyValue, TEXT(","), true); // Split line based on colon


		for (int32 Index = 0; Index < KeyValue.Num(); ++Index) {
			// Log each part of the KeyValue
		}

		if (KeyValue.Num() == 2) {

			FString Key = KeyValue[0].TrimStartAndEnd();
			FString Value = KeyValue[1].TrimStartAndEnd();

			// Strip unit annotations from the value if present
			int32 SpaceIndex;
			if (Value.FindChar(' ', SpaceIndex)) {
				Value = Value.Left(SpaceIndex);
			}

			// Assign values based on the key
			if (Key.Equals("Number of Columns")) {
				UE_LOG(LogTemp, Log, TEXT("THE STRING: %s"), *Value);

				gridParams.numColumns = FCString::Atod(*Value);
				UE_LOG(LogTemp, Log, TEXT("Number of Columns: %f"), gridParams.numColumns);
			}
			else if (Key.Equals("Number of Rows")) {
				gridParams.numRows = FCString::Atoi(*Value);
				UE_LOG(LogTemp, Log, TEXT("Number of Rows: %f"), gridParams.numRows);
			}
			else if (Key.Equals("Resolution in x direction")) {
				gridParams.resolutionX = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Resolution in x direction: %f"), gridParams.resolutionX);
			}
			else if (Key.Equals("Resolution in y direction")) {
				gridParams.resolutionY = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Resolution in y direction: %f"), gridParams.resolutionY);
			}
			else if (Key.Equals("Coordinate system ID Native")) {
				gridParams.coordinateSystemIDNative = FCString::Atoi(*Value);
				UE_LOG(LogTemp, Log, TEXT("Coordinate system ID Native: %d"), gridParams.coordinateSystemIDNative);
			}
			else if (Key.Equals("Top edge Native")) {
				gridParams.topEdgeNative = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Top edge Native: %f"), gridParams.topEdgeNative);
			} // Continue for other keys...
			else if (Key.Equals("Bottom edge Native")) {
				gridParams.bottomEdgeNative = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("bottom edge Native: %f"), gridParams.bottomEdgeNative);
			} // Continue for other keys...
			else if (Key.Equals("Left edge Native")) {
				gridParams.leftEdgeNative = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("left edge Native: %f"), gridParams.leftEdgeNative);
			} // Continue for other keys...
			else if (Key.Equals("Right edge Native")) {
				gridParams.rightEdgeNative = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Right edge Native: %f"), gridParams.rightEdgeNative);
			}
			else if (Key.Equals("Top edge WGS84")) {
				gridParams.topEdgeWGS84 = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Top edge WGS84: %f"), gridParams.topEdgeWGS84);
			}
			else if (Key.Equals("Bottom edge WGS84")) {
				gridParams.bottomEdgeWGS84 = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Bottom edge WGS84: %f"), gridParams.bottomEdgeWGS84);
			}
			else if (Key.Equals("Left edge WGS84")) {
				gridParams.leftEdgeWGS84 = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("left edge WGS84: %f"), gridParams.leftEdgeWGS84);
			}
			else if (Key.Equals("Right edge WGS84")) {
				gridParams.rightEdgeWGS84 = FCString::Atof(*Value);
				UE_LOG(LogTemp, Log, TEXT("Right edge WGS84: %f"), gridParams.rightEdgeWGS84);
			}
		}
	}
}
	




// Assuming gridParams, topLeftLat, topLeftLon, deltaLat, and deltaLon are accessible
std::array<GridCellInfo, 4> AGeoReferenceManager::calculateSurroundingCellsAndWeights(float currentLat, float currentLon) {

	float totalWidth = abs(gridParams.rightEdgeWGS84 - gridParams.leftEdgeWGS84);
	float totalHeight = abs(gridParams.bottomEdgeWGS84 - gridParams.topEdgeWGS84);

	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("totalWidth %f, totalheight: %f"), totalWidth, totalHeight));


	float deltaLon = totalWidth/gridParams.numColumns;
	float deltaLat = totalHeight/gridParams.numRows;

	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("deltaLon %f, deltaLat: %f"), deltaLon, deltaLat));


	std::array<GridCellInfo, 4> surroundingCells;
	float topLeftLon = gridParams.leftEdgeWGS84;
	float topLeftLat = gridParams.topEdgeWGS84;


	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("Current Lon %f, Current Lat: %f"), currentLon, currentLat));
	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("to[ left Lon %f, top left lat: %f"), topLeftLon, topLeftLat));


	// Calculate relative position
	float latDiff = abs(topLeftLat - currentLat);
	float lonDiff = abs(currentLon - topLeftLon);

	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("lonDiff %f, LatDiff: %f"), lonDiff, latDiff));


	// Calculate grid positions
	float gridRow = latDiff / deltaLat;
	float gridCol = lonDiff / deltaLon;

	//GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Magenta, FString::Printf(TEXT("gridRow: %f, gridCol: %f"), gridRow, gridCol));


	// Determine indices of the bottom-right cell that contains the point
	int rowBR = std::round(gridRow);
	int colBR = std::round(gridCol);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Grid Position - gridRow: %f, gridCol: %f"), gridRow, gridCol));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Bottom-Right Cell Indices - rowBR: %d, colBR: %d"), rowBR, colBR));
	
	// Calculate top-left cell indices
	int rowTL = rowBR-1;
	int colTL = colBR-1;

	// Ensure indices are within grid bounds
	rowTL = std::max(0, rowTL);
	colTL = std::max(0, colTL);
	rowBR = std::min(int(gridParams.numRows - 1), rowBR);
	colBR = std::min(int(gridParams.numColumns - 1), colBR);

	// Cells around the point: TL, TR, BL, BR
  // Cells around the point: TL, TR, BL, BR
	int rows[4] = { rowTL, rowTL, rowBR, rowBR };
	int cols[4] = { colTL, colBR, colTL, colBR };	

	// Calculate distances as weight percentages
	for (int i = 0; i < 4; ++i) {
		float cellLat = topLeftLat - rows[i] * deltaLat - deltaLat / 2;
		float cellLon = topLeftLon + cols[i] * deltaLon + deltaLon / 2;

		// Distance calculation (simplified, assuming flat Earth and small distances)
		float latDist = (cellLat - currentLat) / deltaLat; // as a fraction of cell height
		float lonDist = (cellLon - currentLon) / deltaLon; // as a fraction of cell width
		float weight = 1.0f - (std::sqrt(latDist * latDist + lonDist * lonDist) / std::sqrt(2.0f)); // Normalize by sqrt(2) for corner-to-center distance

		surroundingCells[i] = GridCellInfo{ rows[i], cols[i], weight };
	}

	return surroundingCells;
}


float getValue(float val1, float weight1, float val2, float weight2, float val3, float weight3, float val4, float weight4) {
	// Calculate the weighted sum of the values
	float weightedSum = val1 * weight1 + val2 * weight2 + val3 * weight3 + val4 * weight4;

	// Calculate the sum of the weights
	float totalWeight = weight1 + weight2 + weight3 + weight4;

	// Avoid division by zero
	if (totalWeight == 0) {
		// Handle this case as you see fit. For example, you might return 0, or the average of the values, etc.
		return 0;
	}

	// Calculate the weighted average
	float weightedAverage = weightedSum / totalWeight;

	return weightedAverage;
}



FireData AGeoReferenceManager::BilinearInterpolate(const std::array<GridCellInfo, 4>& gridCells) {
	// Assuming the array order is [topLeft, topRight, bottomLeft, bottomRight]

	// Initialize interpolated data struct
	FireData interpolatedData;
	
	
	// Safely retrieve FireData for the four nearest cells
	FireData topLeftData = FireDataGrid[gridCells[0].row][gridCells[0].col];
	FireData topRightData = FireDataGrid[gridCells[1].row][gridCells[1].col];
	FireData bottomLeftData = FireDataGrid[gridCells[2].row][gridCells[2].col];
	FireData bottomRightData = FireDataGrid[gridCells[3].row][gridCells[3].col];


	interpolatedData.elevation = getValue(topLeftData.elevation, gridCells[0].weight, topRightData.elevation, gridCells[1].weight, bottomLeftData.elevation, gridCells[2].weight, bottomRightData.elevation, gridCells[3].weight);
	interpolatedData.fueldepth = getValue(topLeftData.fueldepth, gridCells[0].weight, topRightData.fueldepth, gridCells[1].weight, bottomLeftData.fueldepth, gridCells[2].weight, bottomRightData.fueldepth, gridCells[3].weight);
	interpolatedData.fuelload1 = getValue(topLeftData.fuelload1, gridCells[0].weight, topRightData.fuelload1, gridCells[1].weight, bottomLeftData.fuelload1, gridCells[2].weight, bottomRightData.fuelload1, gridCells[3].weight);
	interpolatedData.fuelload10 = getValue(topLeftData.fuelload10, gridCells[0].weight, topRightData.fuelload10, gridCells[1].weight, bottomLeftData.fuelload10, gridCells[2].weight, bottomRightData.fuelload10, gridCells[3].weight);
	interpolatedData.fuelload100 = getValue(topLeftData.fuelload100, gridCells[0].weight, topRightData.fuelload100, gridCells[1].weight, bottomLeftData.fuelload100, gridCells[2].weight, bottomRightData.fuelload100, gridCells[3].weight);
	interpolatedData.fuelload1000 = getValue(topLeftData.fuelload1000, gridCells[0].weight, topRightData.fuelload1000, gridCells[1].weight, bottomLeftData.fuelload1000, gridCells[2].weight, bottomRightData.fuelload1000, gridCells[3].weight);
	interpolatedData.fuelsav = getValue(topLeftData.fuelsav, gridCells[0].weight, topRightData.fuelsav, gridCells[1].weight, bottomLeftData.fuelsav, gridCells[2].weight, bottomRightData.fuelsav, gridCells[3].weight);
	interpolatedData.slope = getValue(topLeftData.slope, gridCells[0].weight, topRightData.slope, gridCells[1].weight, bottomLeftData.slope, gridCells[2].weight, bottomRightData.slope, gridCells[3].weight);





	return interpolatedData;

}




FireData AGeoReferenceManager::GetFireData() {

	
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FGeographicCoordinates GeoLocation;
	MyGeoRefSystem->EngineToGeographic(PlayerLocation, GeoLocation);
	std::array<GridCellInfo, 4> surroundingcells = calculateSurroundingCellsAndWeights(GeoLocation.Latitude, GeoLocation.Longitude);
	// Assume surroundingcells is populated correctly as per your snippet
	for (int i = 0; i < surroundingcells.size(); ++i) {
		const GridCellInfo& cell = surroundingcells[i];
		FString message = FString::Printf(TEXT("Cell %d - Row: %d, Col: %d, Weight: %f"), i, cell.row, cell.col, cell.weight);
		GEngine->AddOnScreenDebugMessage(-1, 28.f, FColor::Yellow, message);
	}

	FireData data = BilinearInterpolate(surroundingcells);

	return data;

	
	
}



// Called every frame
void AGeoReferenceManager::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);


	// Increment the timer by the amount of time since the last frame
	//TimeSinceLastFireDataUpdate += DeltaTime;
	FireData data = GetFireData();

	UE_LOG(LogTemp, Warning, TEXT("dont worry."));

	// Use rowIndex and columnIndex as needed
	FString ElevationMessage = FString::Printf(TEXT("Elevation: %f"), data.elevation);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, ElevationMessage);

	// Check if 5 seconds have passed
	//if (TimeSinceLastFireDataUpdate >= 0.0) {
		// Reset the timer
	//	TimeSinceLastFireDataUpdate = 0.0;

		// Place the code you want to execute every 5 seconds here
		//FireData data2 = GetFireData();

		// Use rowIndex and columnIndex as needed
	//	FString ElevationMessage2 = FString::Printf(TEXT("Elevation: %f"), data2.elevation);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, ElevationMessage2);
	//}


}

