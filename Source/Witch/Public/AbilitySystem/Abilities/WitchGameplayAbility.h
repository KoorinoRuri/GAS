// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "WitchGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API UWitchGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	
	//如果技能是出生时就有的技能时，使用的输入标签，只适合在最开始赋予角色 Ability 时检查
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartGameplayTag;
};
