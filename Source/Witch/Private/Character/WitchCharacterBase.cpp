// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/WitchCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/WitchAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AWitchCharacterBase::AWitchCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

UAbilitySystemComponent* AWitchCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AWitchCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AWitchCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AWitchCharacterBase::InitAbilityActorInfo()
{
}

void AWitchCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle =  GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AWitchCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AWitchCharacterBase::AddCharacterAbilities()
{
	UWitchAbilitySystemComponent* WitchASC = CastChecked<UWitchAbilitySystemComponent>(AbilitySystemComponent);
	
	//只应在服务端添加能力，需要检查权限
	if (!HasAuthority()) return;
	WitchASC->AddCharacterAbilities(StartupAbilities);
	
	
}



