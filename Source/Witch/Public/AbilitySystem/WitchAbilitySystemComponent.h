// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "WitchAbilitySystemComponent.generated.h"

//供 WidgetController 绑定
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);

/**
 * 
 */
UCLASS()
class WITCH_API UWitchAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	//当此函数被调用时，我们可以知道 AbilityActorInfo 已经被设置完成,即在InitAbilityActorInfo后调用
	//此时也代表可以绑定委托了
	void AbilityActorInfoSet();
	
	FEffectAssetTags EffectAssetTags;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	
protected:
	//FOnGameplayEffectAppliedDelegate的回调函数
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
