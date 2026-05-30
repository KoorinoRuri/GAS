// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * WitchGameplayTags
 * 
 * 包含原生 GameplayTags 的单例，原生：由C++内部创建，并且可以在C++中使用
 */

struct FWitchGameplayTags
{
public:
	//static 修饰成员函数，实际上和全局函数没什么区别，不许要通过该类的实例来调用
	static const FWitchGameplayTags& Get(){ return GameplayTags; }
	static void InitializeNativeGameplayTags();
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	
protected:
	
private:
	//static 修饰类成员和变量，代表该变量被该类所有成员共享，在整个程序中只有一份实体，被该类的所有对象共同访问，而非每个对象各存一份副本。
	//静态成员变量存储在全局/静态数据区（.data 或 .bss 段），不在堆或栈上，因此 sizeof(类) 的大小不含静态成员。
	//它随程序启动而分配空间，随程序结束才释放，生命周期等同于全局变量。
	static FWitchGameplayTags GameplayTags; 
};