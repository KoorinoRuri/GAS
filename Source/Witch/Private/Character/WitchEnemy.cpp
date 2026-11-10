// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchEnemy.h"

#include "AbilitySystem/WitchAbilitySystemComponent.h"
#include "AbilitySystem/WitchAbilitySystemLibrary.h"
#include "AbilitySystem/WitchAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/WitchUserWidget.h"
#include "Witch/Witch.h"

AWitchEnemy::AWitchEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UWitchAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet  = CreateDefaultSubobject<UWitchAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AWitchEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	
	//设置小部件控制器
	if (UWitchUserWidget* WitchUserWidget = Cast<UWitchUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		WitchUserWidget->SetWidgetController(this);
	}
	
	//此时能力系统已经初始化完成，可以在这里绑定能力系统的一些委托
	if (const UWitchAttributeSet* WitchAS = CastChecked<UWitchAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
			);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WitchAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
			);
		//广播初始值
		OnHealthChanged.Broadcast(WitchAS->GetHealth());
		OnMaxHealthChanged.Broadcast(WitchAS->GetMaxHealth());
	}
	
	
}

void AWitchEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UWitchAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	InitializeDefaultAttributes();
}

void AWitchEnemy::InitializeDefaultAttributes() const
{
	UWitchAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AWitchEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AWitchEnemy::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AWitchEnemy::GetPlayerLevel()
{
	return Level;
	
}
