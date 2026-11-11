// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WitchProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
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
		
		//火球方向
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
		
		//把DamageEffectHandle绑定到Projectiles上
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle =  SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		
		//使用get获取，因为FWitchGameplayTags是单例
		const FWitchGameplayTags GameplayTags = FWitchGameplayTags::Get();
		
		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		
		//“按调用者设置数值”
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		//handle现在携带一个键值对，key：GameplayTags.Damage，value：50.f
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
