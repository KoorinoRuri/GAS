// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "WitchAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API UWitchAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	//AllocGameplayEffectContext在 MakeEffectContext()函数中被调用
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
