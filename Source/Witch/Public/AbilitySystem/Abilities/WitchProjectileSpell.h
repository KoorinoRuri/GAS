// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WitchGameplayAbility.h"
#include "WitchProjectileSpell.generated.h"

class AWitchProjectile;

/**
 * 
 */
UCLASS()
class WITCH_API UWitchProjectileSpell : public UWitchGameplayAbility
{
	GENERATED_BODY()

protected:
	//此函数位于 GameplayAbilities.h
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	//要生成的子弹类型
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWitchProjectile> ProjectileClass;
};
