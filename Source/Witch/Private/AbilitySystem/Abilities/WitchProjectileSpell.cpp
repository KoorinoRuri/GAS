// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WitchProjectileSpell.h"

#include "Actor/WitchProjectile.h"
#include "Interaction/CombatInterface.h"

//Ability激活时自动执行
void UWitchProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	
	
}

void UWitchProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	//仅在服务端时才进行生成
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
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
