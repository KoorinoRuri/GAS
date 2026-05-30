// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchAbilitySystemComponent.h"

#include "WitchGameplayTags.h"

void UWitchAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UWitchAbilitySystemComponent::EffectApplied);
	const FWitchGameplayTags& GameplayTags = FWitchGameplayTags::Get();
	
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Green,FString::Printf(TEXT("Tag : %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

//通过委托，可以在运行时获取某个GameplayEffect正在应用的信息
void UWitchAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1,8.f, FColor::Green,FString("EffectApplied"));
	
	FGameplayTagContainer TagContainer;
	//不使用GrantedTags，而是使用AssetTags，后者可以传递标签：这些标签是我们在效果应用时希望获取到的
	EffectSpec.GetAllAssetTags(TagContainer);
	
	EffectAssetTags.Broadcast(TagContainer);
	
	
}
