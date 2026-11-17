// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchAbilitySystemGlobals.h"

#include "WitchAbilityTypes.h"

FGameplayEffectContext* UWitchAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FWitchGameplayEffectContext();
}
