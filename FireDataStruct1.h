// FireDataStruct.h

#pragma once

#include "CoreMinimal.h"
#include "FireDataStruct1.generated.h"


USTRUCT(BlueprintType)
struct FireData1
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double elevation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fuelload1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fuelload10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fuelload100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fuelload1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fueldepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double slope;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double fuelsav;

}; 


