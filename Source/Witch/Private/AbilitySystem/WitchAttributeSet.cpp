// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WitchAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UWitchAttributeSet::UWitchAttributeSet()
{
	

}

void UWitchAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//注册 health 进行复制；COND_None：无条件复制；REPNOTIFY_Always：只要在服务器上设置了值，就要进行复制，不论与原来的值相比是否有变化
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);

	
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UWitchAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	
}

//只要属性发生改变，就会触发，“Pre”代表触发时机是属性值真正改变之前，不过该函数只建议用于clamp数值的边界
void UWitchAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	//只能对 从 modifier 查询到的返回值进行 clamp？但实际上 5.6 版本并没有这个 bug
	Super::PreAttributeBaseChange(Attribute, NewValue);
	
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UWitchAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//source = causer of the effect, target = target of the effect (owner of this AS)
	
	//获取 Source 的信息
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		
		Props.SourceController =  Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		//如果为空，则转换为 Pawn 再获取 Controller
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		
		if (Props.SourceController)
		{
			//ACharacter* SourceCharacter = SourceController->GetCharacter();
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	
	//获取 Target 的信息
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController =  Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

//在GamePlay Effect 修改属性后执行
//在这里收集尽可能多的有用的数据
void UWitchAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	
}

//属性回调函数
void UWitchAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	//告诉GAS系统，我们正在同步一个值，它的值刚从服务器拉取并改变，GAS记录这个变化，并记住旧值，以备需要回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet,Health, OldHealth);
}


void UWitchAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet,Mana, OldMana);
}


void UWitchAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Strength, OldStrength);
}

void UWitchAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Intelligence, OldIntelligence);
}

void UWitchAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Resilience, OldResilience);
}

void UWitchAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldVigor);
}

void UWitchAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldArmor);
}

void UWitchAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldArmorPenetration);
}

void UWitchAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldBlockChance);
}

void UWitchAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldCriticalHitChance);
}

void UWitchAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldCriticalHitDamage);
}

void UWitchAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldCriticalHitResistance);
}

void UWitchAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldHealthRegeneration);
}

void UWitchAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet, Vigor, OldManaRegeneration);
}

void UWitchAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet,MaxHealth, OldMaxHealth);
}

void UWitchAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UWitchAttributeSet,MaxMana, OldMaxMana);
}