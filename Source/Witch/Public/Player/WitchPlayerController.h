// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WitchPlayerController.generated.h"

struct FGameplayTag;
class UWitchInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UWitchAbilitySystemComponent;
class USplineComponent;
/**
 * 
 */
UCLASS()
class WITCH_API AWitchPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AWitchPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> WitchContext;
	
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;
	
	void Move(const FInputActionValue& InputActionValue);
	
	void CursorTrace();
	//Error C2338 : static_assert failed: 'TObjectPtr<T> can only be used with types derived from UObject'
	IEnemyInterface* LastActor;
	//This Actor
	IEnemyInterface* CurrentActor;
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UWitchInputConfig> InputConfig;
	
	//避免在 Held 时每帧都需要进行 cast
	UPROPERTY()
	TObjectPtr<UWitchAbilitySystemComponent> WitchAbilitySystemComponent;
	
	UWitchAbilitySystemComponent* GetASC();
	
	//缓存点击的位置
	FVector CachedDestination = FVector::ZeroVector;
	//跟随光标移动的时间
	float FollowTime = 0.f;
	//短按阈值
	float ShortPressThreshold = 0.5f;
	//为true时，每帧调用一次 AddMovementInput
	bool bAutoRunning = false;
	//鼠标是否正在锁定敌人
	bool bTargeting = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline; 
};
