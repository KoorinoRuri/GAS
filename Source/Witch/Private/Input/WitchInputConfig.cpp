// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/WitchInputConfig.h"

const UInputAction* UWitchInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FWitchInputAction& InputAction : AbilityInputActions )
	{
		if (InputAction.InputAction && InputAction.InputTag.MatchesTag(InputTag))
		{
			return InputAction.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
