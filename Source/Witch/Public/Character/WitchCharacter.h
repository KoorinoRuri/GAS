// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/WitchCharacterBase.h"
#include "WitchCharacter.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API AWitchCharacter : public AWitchCharacterBase
{
	GENERATED_BODY()
public:
	AWitchCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* Combat Interface End */
private:
	virtual void InitAbilityActorInfo() override;
};
