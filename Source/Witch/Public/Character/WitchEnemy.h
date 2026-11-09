// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WitchCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "WitchEnemy.generated.h"


class UWidgetComponent;
/**
 * 
 */
UCLASS()
class WITCH_API AWitchEnemy : public AWitchCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AWitchEnemy();
	
	
	/* enemy interface*/
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	/* enemy interface end*/
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* Combat Interface End */
	
	//生命值发生变化时，用来广播生命值
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
private:
	
	
};
