// Fill out your copyright notice in the Description page of Project Settings.


#include "WitchGameplayTags.h"
#include "GameplayTagsManager.h"

FWitchGameplayTags FWitchGameplayTags::GameplayTags;

void FWitchGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("减少受到的伤害，提升格挡几率"));
}