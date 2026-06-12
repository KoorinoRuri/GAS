// Fill out your copyright notice in the Description page of Project Settings.


#include "WitchAssetManager.h"

#include "AbilitySystemGlobals.h"
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
	//使用 TargetData 时，必须先调用这个函数初始化一些东西
	UAbilitySystemGlobals::Get().InitGlobalData();
}
