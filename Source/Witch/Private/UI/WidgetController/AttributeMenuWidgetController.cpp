// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/WitchAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	//当属性值发生变化时，也需要进行广播
	UWitchAttributeSet* AS = CastChecked<UWitchAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
				/*FWitchAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				Info.AttributeValue = Pair.Value().GetNumericValue(AS);
				AttributeInfoDelegate.Broadcast(Info);*/
			}
		);
	}
	
	
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UWitchAttributeSet* AS = CastChecked<UWitchAttributeSet>(AttributeSet);
	
	check(AttributeInfo);
	
	/*FWitchAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FWitchGameplayTags::Get().Attributes_Primary_Strength);
	
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);*/
	
	//广播初始值时，不需要知道属性有多少，也不必知道有哪些属性
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
		/*FWitchAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		//执行委托
		//FGameplayAttribute Attr = Pair.Value.Execute();
		
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);*/
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FWitchAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
