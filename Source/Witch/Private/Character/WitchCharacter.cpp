// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/WitchPlayerController.h"
#include "Player/WitchPlayerState.h"
#include "UI/HUD/WitchHUD.h"

AWitchCharacter::AWitchCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;//将角色朝向旋转至移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);//旋转速度
	GetCharacterMovement()->bConstrainToPlane = true;//移动限制在平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;//开始时确保角色在地面
	
	bUseControllerRotationPitch = false;//不使用控制器旋转
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AWitchCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//Init ability actor info for the server
	InitAbilityActorInfo();
	
}

void AWitchCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	//Init ability info for the client
	InitAbilityActorInfo();
}

void AWitchCharacter::InitAbilityActorInfo()
{
	AWitchPlayerState* WitchPlayerState = GetPlayerState<AWitchPlayerState>();
	check(WitchPlayerState);
	WitchPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(WitchPlayerState,this);
	Cast<UWitchAbilitySystemComponent>(WitchPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	AbilitySystemComponent = WitchPlayerState->GetAbilitySystemComponent();
	AttributeSet = WitchPlayerState->GetAttributeSet();
	
	//至此 InitOverlay() 所需的参数都已经具备，可以在这里调用
	//获取controller，WitchPlayerController 在多人游戏中是可能为空的, 我们不希望游戏因此崩溃, 因此不应该使用（断言assert）: check
	if (AWitchPlayerController* WitchPlayerController = Cast<AWitchPlayerController>(GetController()))
	{	//只有本地玩家的HUD有效
		if (AWitchHUD* WitchHUD = Cast<AWitchHUD>(WitchPlayerController->GetHUD()))
		{
			WitchHUD->InitOverlay(WitchPlayerController,WitchPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitializePrimaryAttributes();
}
