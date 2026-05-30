// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "WitchAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API UWitchAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UWitchAssetManager& Get();
	
protected:
	//这个函数会在很早的阶段调用
	virtual void StartInitialLoading() override;
};
