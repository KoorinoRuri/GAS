// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WitchPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/WitchInputComponent.h"
#include "Interaction/EnemyInterface.h"

AWitchPlayerController::AWitchPlayerController()
{
	bReplicates = true;
}

void AWitchPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AWitchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(WitchContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//只有在本地控制的机器并且有有效的local player时，Subsystem才有效，我们不希望游戏因此崩溃，因此不应该使用（断言assert）: check(Subsystem);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(WitchContext,0);
	}
	
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AWitchPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UWitchInputComponent* WitchInputComponent = CastChecked<UWitchInputComponent>(InputComponent);
	WitchInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AWitchPlayerController::Move);
	
	WitchInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AWitchPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0,Rotation.Yaw,0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
		
	}
	
}

void AWitchPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = CurrentActor;
	CurrentActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/*
	 *Line Trace from cursor 包含 5 种情况
	 * A: LastActor == nullptr && CurrentActor == nullptr
	 *	- Do nothing
	 * B: LastActor == nullptr && CurrentActor != nullptr
	 *  - Highlight	CurrentActor
	 * C: LastActor != nullptr && CurrentActor == nullptr
	 *	- UnHighlight LastACtor
	 * D: Both != nullptr && LastActor != CurrentActor
	 *  - UnHighlight LastActor , Highlight CurrentActor
	 * E: Both != nullptr && LastActor == CurrentActor
	 *  - Do Nothing
	 */
	if (LastActor == nullptr)
	{
		if (CurrentActor != nullptr)// Case B
		{
			CurrentActor->HighLightActor();
		}
	}
	else
	{
		if (CurrentActor == nullptr)// Case C
		{
			LastActor->UnHighLightActor();
		}
		else
		{
			if (LastActor != CurrentActor)// Case D
			{
				LastActor->UnHighLightActor();
				CurrentActor->HighLightActor();
			}
		}
	}
}

void AWitchPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	
}

void AWitchPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	
}

void AWitchPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	
}
