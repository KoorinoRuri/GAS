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
		// 我们在服务端，所以监听 Target Data
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		// 一旦服务器调用 Activate()，服务器会把它的回调函数绑定到这个委托上
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		//如果TargetData先被设置
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			//告诉服务器，还在等待远程玩家数据
			SetWaitingOnRemotePlayerData();
		}
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

//在收到 TargetData 也就是复制过来的 TargetData 后会被调用
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	//告诉 AbilitySystem，TargetData已经接收，不用再存储或者缓存了
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		//改为广播handle，这样就能获得命中结果和目标数据中包含的其他所有信息
		ValidData.Broadcast(DataHandle);
	}
}
