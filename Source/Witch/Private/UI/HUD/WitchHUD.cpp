// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WitchHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/WitchUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

//创建OverlayWidgetController
UOverlayWidgetController* AWitchHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//绑定委托
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AWitchHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this,AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		//绑定委托
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void AWitchHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("Overlay Widget Class uninitialized, fill out BP_WItchHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill out BP_WItchHUD"));
	//创建OverlayWidget
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget = Cast<UWitchUserWidget>(Widget);
	//OverlayWidget = CreateWidget<UWitchUserWidget>(GetWorld(),OverlayWidgetClass);
	//创建OverlayWidgetController
	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	
	//将WidgetController设置到 OverlayWidget上
	OverlayWidget->SetWidgetController(WidgetController);
	//此处是广播初始值的时机
	WidgetController->BroadcastInitialValues();
	
	//OverlayWidget->AddToViewport();
	Widget->AddToViewport();
}


