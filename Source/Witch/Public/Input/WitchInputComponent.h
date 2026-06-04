// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "WitchInputConfig.h"
#include "WitchInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class WITCH_API UWitchInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	//对UWitchInputConfig不做前向声明而是直接引用头文件的原因是：想让这个函数成为模板函数，而模板函数会在头文件中直接定义
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>//因为我们会在函数中调用绑定的action，于是需要一个user object; PressedFuncType用于处理输入按下时的处理
	void BindAbilityActions(const UWitchInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, const HeldFuncType& HeldFunc);
};
//借助这个函数，回调函数会被绑定到每个输入动作上，处理按下，释放和长按事件
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UWitchInputComponent::BindAbilityActions(const UWitchInputConfig* InputConfig, UserClass* Object,PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, const HeldFuncType& HeldFunc)
{
	check(InputConfig);
	for (const FWitchInputAction& Action : InputConfig->AbilityInputActions)
	{
		//绑定三种输入 
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				//PressedFunc 后面的所有参数，会被作为参数，传递给 PressedFunc
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			
			if (ReleasedFunc)
			{
				//ReleasedFunc 后面的所有参数，会被作为参数，传递给 ReleasedFunc
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			
			if (HeldFunc)
			{
				//HeldFunc 后面的所有参数，会被作为参数，传递给 HeldFunc
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
			
		}
	}
}
