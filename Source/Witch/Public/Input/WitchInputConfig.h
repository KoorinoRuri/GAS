// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WitchInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FWitchInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};


/**
 * 
 */
UCLASS()
class WITCH_API UWitchInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	//返回与指定标签相关的 InputAction
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWitchInputAction> AbilityInputActions;
};
