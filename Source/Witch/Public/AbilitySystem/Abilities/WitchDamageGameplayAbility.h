// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WitchGameplayAbility.h"
#include "WitchDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API UWitchDamageGameplayAbility : public UWitchGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	//key：伤害类型，value：伤害值（使用curve table中的曲线按等级获取）
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
