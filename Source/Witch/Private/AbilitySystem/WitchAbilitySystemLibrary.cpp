// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchAbilitySystemLibrary.h"
#include "UI/WidgetController/WitchWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/WitchPlayerState.h"
#include "UI/HUD/WitchHUD.h"

//用来在蓝图中调用，获取WidgetController
UOverlayWidgetController* UWitchAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AWitchHUD* WitchHUD = Cast<AWitchHUD>(PC->GetHUD()))
		{
			
			AWitchPlayerState* PS = PC->GetPlayerState<AWitchPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return WitchHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UWitchAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if (AWitchHUD* WitchHUD = Cast<AWitchHUD>(PC->GetHUD()))
		{
			
			AWitchPlayerState* PS = PC->GetPlayerState<AWitchPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return WitchHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
