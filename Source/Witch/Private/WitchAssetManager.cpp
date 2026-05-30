// Fill out your copyright notice in the Description page of Project Settings.


#include "WitchAssetManager.h"
#include "WitchGameplayTags.h"


UWitchAssetManager& UWitchAssetManager::Get()
{
	check(GEngine);
	
	UWitchAssetManager* WitchAssetManager = Cast<UWitchAssetManager>(GEngine->AssetManager);
	return *WitchAssetManager;
	
}

void UWitchAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FWitchGameplayTags::InitializeNativeGameplayTags();
}
