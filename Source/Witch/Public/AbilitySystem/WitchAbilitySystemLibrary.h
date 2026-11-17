// Fill out your copyright notice in the Description page of Project Settings.


//为了想获取WidgetController时，不需要每次都去HUD里找

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "WitchAbilitySystemLibrary.generated.h"


struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
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
	
	//根据角色和等级初始化属性
	UFUNCTION(BlueprintCallable, Category="WitchAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float level, UAbilitySystemComponent* ASC);
	
	
	UFUNCTION(BlueprintCallable, Category="WitchAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category="WitchAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	//BlueprintPure（蓝图纯函数）的核心作用是定义一个“无副作用”的查询或计算函数。它最直观的特征是在蓝图中生成的节点没有执行引脚（Exec Pins），其执行完全由数据流驱动。
	//BlueprintPure 最常用于实现 Getter 函数
	UFUNCTION(BlueprintPure, Category="WitchAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category="WitchAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	//注意这里是 BlueprintCallable
	//UPARAM(ref) 表示这确实是一个输入参数，不加的话蓝图节点会将 非const的& 转化为输出节点，默认函数内会修改
	UFUNCTION(BlueprintCallable, Category="WitchAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	
	UFUNCTION(BlueprintCallable, Category="WitchAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
};
