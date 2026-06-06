// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WitchProjectileSpell.h"

#include "Actor/WitchProjectile.h"
#include "Interaction/CombatInterface.h"

void UWitchProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//仅在服务端时才进行生成
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		
		FTransform SpawnTransform;
		//TODO : Set the Rotation
		SpawnTransform.SetLocation(SocketLocation);
		AWitchProjectile* Projectile = GetWorld()->SpawnActorDeferred<AWitchProjectile>(
			ProjectileClass, 
			SpawnTransform, 
			GetOwningActorFromActorInfo(), 
			Cast<APawn>(GetOwningActorFromActorInfo()), 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: Give the projectile a Gameplay Effect Spec for causing damage;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
}
