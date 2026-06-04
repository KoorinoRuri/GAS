// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchAbilitySystemComponent.h"

#include "WitchGameplayTags.h"
#include "AbilitySystem/Abilities/WitchGameplayAbility.h"

void UWitchAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UWitchAbilitySystemComponent::EffectApplied);
	
	//const FWitchGameplayTags& GameplayTags = FWitchGameplayTags::Get();
	//GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Green,FString::Printf(TEXT("Tag : %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

void UWitchAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		//只要继承自UWitchGameplayAbility，我们就可以拿到其 StartGameplayTag，把这个 Tag 加入到 DynamicAbilityTags 里
		if (const UWitchGameplayAbility* WitchAbility = Cast<UWitchGameplayAbility>(AbilitySpec.Ability))
		{
			//DynamicAbilityTags 是为了能在运行时灵活添加和移除而设计的
			AbilitySpec.DynamicAbilityTags.AddTag(WitchAbility->StartGameplayTag);
			//const 和 非const 都可以
			GiveAbility(AbilitySpec);
		}
		
		
		
		
		//不能是const
		//GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

//从PlayerController中调用
void UWitchAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//GetActivatableAbilities 指我们拥有且可以激活的 Ability
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//检查是否与标签完全对应
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//在 AbilitySpec 上设置一个布尔标记，用来跟踪按键是否被按下
			AbilitySpecInputPressed(AbilitySpec);
			
			//如果没激活，则尝试激活他
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UWitchAbilitySystemComponent::AbilityInputTagsReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	
	//GetActivatableAbilities 指我们拥有且可以激活的 Ability
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//检查是否与标签完全对应
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			//在 AbilitySpec 上设置一个布尔标记，用来跟踪按键是否被松开
			//仅仅告诉 Ability，按键已经松开，何时结束由 Ability自己判断
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
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
