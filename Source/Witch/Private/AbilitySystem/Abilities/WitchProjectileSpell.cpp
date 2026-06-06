// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WitchProjectileSpell.h"

void UWitchProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}
