// Copyright Epic Games, Inc. All Rights Reserved.

#include "RecoverableGeoPlugin.h"
#include "RecoverableGeoPluginStyle.h"
#include "RecoverableGeoPluginCommands.h"
#include "Misc/MessageDialog.h"
#include "LandscapeSubsystem.h"
#include "LandscapeInfo.h"
#include "Materials/MaterialInstanceConstant.h"
#include "FireDataStruct.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "AssetSelection.h"
#include "Blueprint/UserWidget.h"
#include "UMG.h"
#include "ToolMenus.h"
#include <Misc/FileHelper.h>
#include "Widgets/SWindow.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "GeoReferencingSystem.h"




static const FName RecoverableGeoPluginTabName("RecoverableGeoPlugin");

#define LOCTEXT_NAMESPACE "FRecoverableGeoPluginModule"
FString OutSelectedFolder;
FString OutSelectedFile;

void FRecoverableGeoPluginModule::CreateEditorWindow()
{
	
	TSharedRef<SWindow> EditorWindow = SNew(SWindow)
		.Title(FText::FromString("My Editor Window"))
		.ClientSize(FVector2D(800, 400))
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("Select Project Directory"))
						.OnClicked_Lambda([this]() -> FReply {
						return OnMyButtonClicked();
							})
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("Select Landscape File (elevation)"))
						.OnClicked_Lambda([this]() -> FReply {
						return OnMyButtonClicked2();
							})
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("Generate Landscape"))  // Add the third button with your desired text
						.OnClicked_Lambda([this]() -> FReply {
						return OnMyButtonClicked3();  // You can define the behavior for the third button here
							})
				]
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View Elevation"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D( (OutSelectedFolder +"/elevation.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View Fuel Depth"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_depth.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View 1 Hour Fuel Load"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_load_1hr.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View 10 Hour Fuel Load"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_load_10hr.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View 100 Hour Fuel Load"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_load_100hr.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View 1000 Hour Fuel Load"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_load_1000hr.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
						.Text(FText::FromString("View Fuel SAV"))
						.OnClicked_Lambda([this]() -> FReply {
						UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/fuel_sav.png"));

						return OnChangeLandscapeTextureClicked(MyTexture);
							})
				]

					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SButton)
							.Text(FText::FromString("View Slope"))
							.OnClicked_Lambda([this]() -> FReply {
							UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/slope.png"));

							return OnChangeLandscapeTextureClicked(MyTexture);
								})
					]


		];

	// Add the window to the screen
	FSlateApplication::Get().AddWindow(EditorWindow);
}



FReply  FRecoverableGeoPluginModule::OnMyButtonClicked()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform != nullptr)
	{
		// Prompt for a directory
		bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			nullptr, // ParentWindowHandle
			TEXT("Select a Directory"), // DialogTitle
			TEXT(""), // DefaultPath
			OutSelectedFolder // OUT Selected Folder
		);

	}
	// Handle button click
	return FReply::Handled();
}


FReply  FRecoverableGeoPluginModule::OnMyButtonClicked2()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform != nullptr)
	{
		//Prompt for a landscape file
		TArray<FString> SelectedFiles;
		const bool bFileSelected = DesktopPlatform->OpenFileDialog(
			nullptr, // ParentWindowHandle
			TEXT("Select Landscape File"), // DialogTitle
			TEXT(""), // DefaultFile
			TEXT(""), // DefaultFile
			TEXT("Landscape Files (*.png, *.jpg)|*.png;*.jpg"), // FileTypes
			EFileDialogFlags::None, // Flags
			SelectedFiles // OUT Selected Files
		);

		if (SelectedFiles.Num() > 0)
		{
			FString OutSelectedFile1 = SelectedFiles[0];
			OutSelectedFile = OutSelectedFile1;

			// Log the selected file path to the Unreal Engine log
			UE_LOG(LogTemp, Warning, TEXT("Selected File: %s"), *OutSelectedFile1);


		}
		// Handle button click
	}
	return FReply::Handled();

}


FReply  FRecoverableGeoPluginModule::OnMyButtonClicked3()
{

	//Here begins the texture impot 
	FString SourceFilePath = OutSelectedFile;

	UTexture2D* MyTexture = FImageUtils::ImportFileAsTexture2D(SourceFilePath);

	// Generate Landscape form selected base file
	GenerateLandscapeFromTexture(MyTexture);
	ConfigureGeoReferencingFromCSV();
	FString BinaryFilePath = OutSelectedFolder + TEXT("/lossless.bin");


	// Function to read and deserialize the binary file
	ReadAndDeserializeBinaryFile(BinaryFilePath);	
	return FReply::Handled();

}


// Define the new function that will be called when the button is clicked
FReply FRecoverableGeoPluginModule::OnChangeLandscapeTextureClicked(UTexture2D* MyTexture)
{

	UE_LOG(LogTemp, Warning, TEXT("button do be pressed."))


	if (!MyTexture)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load texture."));
		return FReply::Handled();
	}
	NewMIC->SetTextureParameterValueEditorOnly(ParameterInfo1, MyTexture);
	NewMIC->MarkPackageDirty();

	// Then apply the MID to the landscape if needed
	// Note that this is not the usual practice for landscape materials due to performance considerations
	Landscape->LandscapeMaterial = NewMIC;
	Landscape->MarkPackageDirty();

	return FReply::Handled();


}


// Example function to configure the GeoReferencing plugin based on CSV data
void FRecoverableGeoPluginModule::ConfigureGeoReferencingFromCSV()
{

	FString FilePath = OutSelectedFolder + "/georef_info.csv";
	TArray<FString> Lines;
	TMap<FString, FString> ParsedCSV;

	// Load the CSV file into an array of strings, each representing a line
	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		for (FString Line : Lines)
		{
			TArray<FString> LineParts;
			if (Line.ParseIntoArray(LineParts, TEXT(","), true) == 2) // Assuming each line has exactly two parts
			{
				FString Key = LineParts[0].TrimStartAndEnd();
				FString Value = LineParts[1].TrimStartAndEnd();
				ParsedCSV.Add(Key, Value);
			}
		}

		// Use the parsed CSV data
		FString CoordinateSystem = ParsedCSV.Contains(TEXT("Coordinate system ID WGS84")) ? ParsedCSV[TEXT("Coordinate system ID WGS84")] : TEXT("");
		FString CoordinateSystem2 = ParsedCSV.Contains(TEXT("Coordinate system ID Native")) ? ParsedCSV[TEXT("Coordinate system ID Native")] : TEXT("");
		// Extracting origin projected coordinates easting and northing
		double OriginProjectedCoordinatesEasting = ParsedCSV.Contains(TEXT("Left edge Native")) ? FCString::Atof(*ParsedCSV[TEXT("Left edge Native")]) : 0.0;
		double OriginProjectedCoordinatesNorthing = ParsedCSV.Contains(TEXT("Top edge Native")) ? FCString::Atof(*ParsedCSV[TEXT("Top edge Native")]) : 0.0;


		double OriginLatitude = ParsedCSV.Contains(TEXT("Top edge WGS84")) ? FCString::Atof(*ParsedCSV[TEXT("Top edge WGS84")]) : 0.0;
		// Continue extracting other parameters similarly...

		// Example output
		UE_LOG(LogTemp, Log, TEXT("Coordinate System: %s, Latitude: %f"), *CoordinateSystem, OriginLatitude);


		UWorld* World = GEditor->GetEditorWorldContext().World();

		AGeoReferencingSystem* GeoRefSystem = World->SpawnActor<AGeoReferencingSystem>(AGeoReferencingSystem::StaticClass());
		GeoRefSystem->GeographicCRS = FString("EPSG:" + CoordinateSystem);
		GeoRefSystem->ProjectedCRS = FString("EPSG:" + CoordinateSystem2);
		GeoRefSystem->OriginProjectedCoordinatesEasting = OriginProjectedCoordinatesEasting;
		GeoRefSystem->OriginProjectedCoordinatesNorthing = OriginProjectedCoordinatesNorthing;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file at path: %s"), *FilePath);
	}



	// Placeholder for reading and parsing the CSV file
	// For demonstration purposes, let's assume we've parsed these values
	double OriginLatitude = 40.7128;
	double OriginLongitude = -74.0060;
	double OriginAltitude = 0.0;



}




void FRecoverableGeoPluginModule::GenerateLandscapeFromTexture(UTexture2D* MyTexture) {

	if (MyTexture)
	{
		UTexture2D* MyTexture3 = FImageUtils::ImportFileAsTexture2D((OutSelectedFolder + "/elevation.png"));



		MyTexture->SRGB = false; // Turn off sRGB if it's a heightmap
		MyTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale; // This is just an example. Set based on your needs.
		MyTexture->UpdateResource();
		FAssetRegistryModule::AssetCreated(MyTexture);

		MyTexture->SetFlags(RF_Public | RF_Standalone);


		// Conversion of Texture to HeightData
		FTexture2DMipMap& Mip = MyTexture->GetPlatformData()->Mips[0];
		uint16* Data = (uint16*)Mip.BulkData.Lock(LOCK_READ_ONLY);

		uint16* HeightData = new uint16[Mip.SizeX * Mip.SizeY];

		for (int32 y = 0; y < Mip.SizeY; y++)
		{
			for (int32 x = 0; x < Mip.SizeX; x++)
			{
				uint16 grayscaleValue16Bit = Data[y * Mip.SizeX + x];

				HeightData[y * Mip.SizeX + x] = grayscaleValue16Bit;
			}
		}

		Mip.BulkData.Unlock();
		UE_LOG(LogTemp, Warning, TEXT("Conversion complete."))



			//HERE BEGINS LANDSCXAPE CREASTION
		UWorld* World = GEditor->GetEditorWorldContext().World();
		if (World) {
			UE_LOG(LogTemp, Warning, TEXT("world"))

		}


		UClass* LandscapeClass = ALandscape::StaticClass();
		FVector SpawnLocation(0, 0, 0); // example location
		FRotator SpawnRotation(0, 0, 0); // example rotation
		UMaterialInterface* Material = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/StarterContent/Materials/NewHeightLerp.NewHeightLerp"));
		if (!Material)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load landscape material."));
		}

		int32 subsections;
		int32 subsectionsize;
		int32 subsectionsizequads;
		FString elefile = OutSelectedFolder + TEXT("/elevation.csv");
		FRecoverableGeoPluginModule::processcsvfile(elefile, subsections, subsectionsize, subsectionsizequads, zscale);

		UE_LOG(LogTemp, Warning, TEXT("Subsections: %d"), subsections);
		UE_LOG(LogTemp, Warning, TEXT("Subsection Size: %d"), subsectionsize);
		UE_LOG(LogTemp, Warning, TEXT("Subsection Size Quads: %d"), subsectionsizequads);
		UE_LOG(LogTemp, Warning, TEXT("ZScale: %f"), zscale);

		// Spawn the actor
		Landscape = World->SpawnActor<ALandscape>(LandscapeClass, SpawnLocation, SpawnRotation);
		Landscape->SetCanBeDamaged(false);
		if (!Landscape)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn landscape actor."));
		}
		// Setup landscape configuration
		Landscape->ComponentSizeQuads = subsections;
		Landscape->NumSubsections = subsectionsize;
		Landscape->SubsectionSizeQuads = subsectionsizequads;
		Landscape->bUseDynamicMaterialInstance = true;


		Landscape->SetLandscapeGuid(FGuid::NewGuid());

		NewMIC = NewObject<UMaterialInstanceConstant>(
			GetTransientPackage(), // You could specify another package if needed
			NAME_None,
			RF_Standalone | RF_Public
		);


		// Assign the parent material to the material instance
		NewMIC->SetParentEditorOnly(Material);
		float zscaleval = zscale;
		float heightrange = 511.992 * zscaleval;
		float height = heightrange / 2;
		float negheight = 0 - height;

		// Now you can set parameters as needed on your NewMIC
		FName ParameterName = "MaxHeightVal"; // The parameter you want to set
		FMaterialParameterInfo ParameterInfo(ParameterName);
		float ScalarValue = height;   // The scalar value for the parameter
		NewMIC->SetScalarParameterValueEditorOnly(ParameterInfo, ScalarValue);

		FMaterialParameterInfo ParameterInfoHolder("VisibleTexture");
		ParameterInfo1 = ParameterInfoHolder;
		NewMIC->SetTextureParameterValueEditorOnly(ParameterInfo1, MyTexture3);

		// Now you can set parameters as needed on your NewMIC
		FName ParameterName2 = "MinHeightVal"; // The parameter you want to set
		FMaterialParameterInfo ParameterInfo2(ParameterName2);
		float ScalarValue2 = negheight;   // The scalar value for the parameter
		NewMIC->SetScalarParameterValueEditorOnly(ParameterInfo2, ScalarValue2);



		// Don't forget to mark the package dirty to ensure it gets saved
		NewMIC->MarkPackageDirty();

		// Then apply the MID to the landscape if needed
		// Note that this is not the usual practice for landscape materials due to performance considerations
		Landscape->LandscapeMaterial = NewMIC;
		Landscape->CreateLandscapeInfo();
		Landscape->SetActorHiddenInGame(false);

		//Setup World partition streaming proxies
		TMap<FGuid, TArray<uint16>> HeightDataMap;
		TMap<FGuid, TArray<FLandscapeImportLayerInfo>> MaterialLayerDataPerLayer;
		HeightDataMap.Add(FGuid(), TArray<uint16>(HeightData, (Mip.SizeX * Mip.SizeY)));
		delete[] HeightData;
		MaterialLayerDataPerLayer.Add(FGuid(), TArray<FLandscapeImportLayerInfo>());
		ULandscapeInfo* LandscapeInfo = Landscape->GetLandscapeInfo();

		// Set the desired scale values
		FVector NewScale(100.0f, 100.0f, zscaleval);
		// Example scale values: X=100, Y=100, Z=50

		// Apply the scale to the landscape's root component
		Landscape->GetRootComponent()->SetWorldScale3D(NewScale);

		Landscape->LOD0ScreenSize = 0.1;
		Landscape->LOD0DistributionSetting = 10.0;
		Landscape->MaxLODLevel = -1;
		Landscape->StaticLightingLOD = 0;



		FVector Scale = Landscape->GetRootComponent()->GetComponentScale();

		UE_LOG(LogTemp, Warning, TEXT("Landscape Z-Scale: %f"), Scale.Z);
		//Setup World partition streaming proxies



		Landscape->Import(
			Landscape->GetLandscapeGuid(),
			0,
			0,
			Mip.SizeX - 1,
			Mip.SizeY - 1,
			Landscape->NumSubsections,
			Landscape->SubsectionSizeQuads,
			HeightDataMap,
			TEXT("NONE"),
			MaterialLayerDataPerLayer,
			ELandscapeImportAlphamapType::Layered

		);



	}
}

// Define the function to return the four parameters
void FRecoverableGeoPluginModule::processcsvfile(const FString& FilePath, int32& subsections, int32& subsectionsize, int32& subsectionsizequads, float& zscaleDouble)
{

	TArray<FString> Lines;

	// Step 1: Load the file into an array of FString
	FFileHelper::LoadFileToStringArray(Lines, *FilePath);
	// Iterate through the array and log each line
	 subsections = FCString::Atoi(*Lines[0]);
	 subsectionsize = FCString::Atoi(*Lines[1]);
	 subsectionsizequads = FCString::Atoi(*Lines[2]);
	 zscaleDouble = FCString::Atod(*Lines[3]);

	
	//int track = 0;

	//UE_LOG(LogTemp, Warning, TEXT("IT GOT CALLED ."))

		//TArray<FString> Fields;
		//Line.ParseIntoArray(Fields, TEXT(","), true);
		// Convert the first field to an integer
		//int32 FirstFieldAsInt = FCString::Atoi(*Fields[0]);
		//UE_LOG(LogTemp, Log, TEXT("FirstFieldAsInt: %d"), FirstFieldAsInt);

			
	}


void FRecoverableGeoPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRecoverableGeoPluginStyle::Initialize();
	FRecoverableGeoPluginStyle::ReloadTextures();

	FRecoverableGeoPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRecoverableGeoPluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FRecoverableGeoPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRecoverableGeoPluginModule::RegisterMenus));
}

void FRecoverableGeoPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRecoverableGeoPluginStyle::Shutdown();

	FRecoverableGeoPluginCommands::Unregister();
}

void FRecoverableGeoPluginModule::PluginButtonClicked()
{
	CreateEditorWindow();
}

void FRecoverableGeoPluginModule::ReadAndDeserializeBinaryFile(const FString& FilePath)
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
			TArray<TArray<FireData>> FireDataGrid;
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
				}
			}

			// Now FireDataGrid contains all your deserialized data in a 2D grid
			// You can process or use this data as needed

			// Example: Log the details of the first element in the grid
			//const FireData& FirstElement = FireDataGrid[113][54];
		//	const FireData& FirstElement1 = FireDataGrid[190][64];
	//		const FireData& FirstElement2 = FireDataGrid[154][198];

			
		//	UE_LOG(LogTemp, Log, TEXT("First Grid Element - Elevation: %f, FuelLoad1: %f, FuelLoad10: %f, FuelLoad100: %f, FuelLoad1000: %f, FuelDepth: %f, Slope: %f, FuelSAV: %f"),
			//	FirstElement.elevation, FirstElement.fuelload1, FirstElement.fuelload10, FirstElement.fuelload100, FirstElement.fuelload1000, FirstElement.fueldepth, FirstElement.slope, FirstElement.fuelsav);
		//	UE_LOG(LogTemp, Log, TEXT("First Grid Element - Elevation: %f, FuelLoad1: %f, FuelLoad10: %f, FuelLoad100: %f, FuelLoad1000: %f, FuelDepth: %f, Slope: %f, FuelSAV: %f"),
		//		FirstElement1.elevation, FirstElement1.fuelload1, FirstElement1.fuelload10, FirstElement1.fuelload100, FirstElement1.fuelload1000, FirstElement1.fueldepth, FirstElement1.slope, FirstElement1.fuelsav);
		
	//		UE_LOG(LogTemp, Log, TEXT("First Grid Element - Elevation: %f, FuelLoad1: %f, FuelLoad10: %f, FuelLoad100: %f, FuelLoad1000: %f, FuelDepth: %f, Slope: %f, FuelSAV: %f"),
	//		FirstElement2.elevation, FirstElement2.fuelload1, FirstElement2.fuelload10, FirstElement2.fuelload100, FirstElement2.fuelload1000, FirstElement2.fueldepth, FirstElement2.slope, FirstElement2.fuelsav);
	//	}
	//	else
	//	{
			UE_LOG(LogTemp, Error, TEXT("Failed to load binary file: %s"), *FilePath);
//		}
	}






void FRecoverableGeoPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRecoverableGeoPluginCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRecoverableGeoPluginCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRecoverableGeoPluginModule, RecoverableGeoPlugin)