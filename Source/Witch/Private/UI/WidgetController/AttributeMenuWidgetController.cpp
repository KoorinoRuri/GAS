// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/WitchAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "WitchGameplayTags.h"
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UWitchAttributeSet* AS = CastChecked<UWitchAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	
	FWitchAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FWitchGameplayTags::Get().Attributes_Primary_Strength);
	
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}
