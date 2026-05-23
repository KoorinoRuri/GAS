// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "WitchPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class WITCH_API AWitchPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AWitchPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;//来自IAbilitySystemInterface
	UAttributeSet* GetAttributeSet() const{ return AttributeSet; };
	
	FORCEINLINE int32 GetPlayerLevel() const{ return Level; };
	
protected:
	UPROPERTY(VisibleAnywhere)//暴露给蓝图，因为 Initialize Attributes from a Data Table 时需要设置 DataTable
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	
	
private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;
};
