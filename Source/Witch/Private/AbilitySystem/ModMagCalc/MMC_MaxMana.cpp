// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/WitchAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UWitchAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	//在 effect spec 里捕捉属性的时机？effect spec刚创建时或者效果被应用时？
	//Snapshotting 就是 effect spec 一创建就进行捕捉，不快照的话就是效果应用时才捕捉
	IntelligenceDef.bSnapshot = false;
	
	// Array of Attributes to capture
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//可以收集来自 source 和 Target 的 Tags
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	//要捕获一个属性并获取它的大小，需要创建这个结构体
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// 捕获 Intelligence
	float Intelligence = 0.f;
	//类似于 slash 项目里创建的 BoxHitResult，引用形式传入函数后会在函数体内修改成我们需要的值
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);
	
	//获取 Level
	ICombatInterface* CombatInterface =  Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
}
