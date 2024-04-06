// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "AssetRegistry/AssetRegistryModule.h"
#include "ImageUtils.h"
#include "Landscape.h"
#include "Containers/Map.h"
#include "Modules/ModuleManager.h"
#include "Factories/TextureFactory.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FgeorefpluginModule : public IModuleInterface
{
public:



	void CreateEditorWindow();

	FReply OnMyButtonClicked();
	FReply OnMyButtonClicked2();
	FReply OnMyButtonClicked3();

	FReply OnChangeLandscapeTextureClicked(UTexture2D* MyTexture);

	ALandscape* Landscape;

	UMaterialInstanceConstant* NewMIC;
	FMaterialParameterInfo ParameterInfo1;


	void GenerateLandscapeFromTexture(UTexture2D* MyTexture);
	void ConfigureGeoReferencingFromCSV();


	void processcsvfile(const FString& FilePath, int32& subsections, int32& subsectionsize, int32& zscale, float& zscaleDouble);

	float zscale;


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	void ReadAndDeserializeBinaryFile(const FString& FilePath);
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
