// Fill out your copyright notice in the Description page of Project Settings.


//为了想获取WidgetController时，不需要每次都去HUD里找

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WitchAbilitySystemLibrary.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class WITCH_API UWitchAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	//引擎中很多函数库都要求传入一个世界上下文对象，因为它需要一个引用，能让它以某种方式追溯到世界中我们想要操作的对象
	UFUNCTION(BlueprintPure, Category="WitchAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	
	UFUNCTION(BlueprintPure, Category="WitchAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
