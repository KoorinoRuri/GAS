// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

//会自动执行
void UTargetDataUnderMouse::Activate()
{
	Super::Activate();
	
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		//如果是客户端
		SendMouseCursorData();
	}
	else
	{
		//TODO : 我们在服务端，所以监听 Target Data
	}
	
	
}

//需要重点看一下这个函数
void UTargetDataUnderMouse::SendMouseCursorData()
{
	//告诉系统，范围内（也就是下面）的所有操作都应该被预测
	//请求服务器允许我们在本地执行下面的操作
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle, 
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	
	//判断是否可以广播
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//改为广播handle，这样就能获得命中结果和目标数据中包含的其他所有信息
		ValidData.Broadcast(DataHandle);
	}
}
