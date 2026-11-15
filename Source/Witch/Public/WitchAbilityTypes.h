#pragma once

#include "GameplayEffectTypes.h"
#include "WitchAbilityTypes.generated.h"

//BlueprintType : 允许一个 C++ 类型（类、结构体、枚举或接口）作为变量类型、参数或返回值，在蓝图中被使用。
USTRUCT(BlueprintType)
struct FWitchGameplayEffectContext : public FGameplayEffectContext //继承FGameplayEffectContext，添加我们想要记录的信息
{
	GENERATED_BODY()
	
public:
	
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	//此函数来自GameplayEffectTypes，需要重写他
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}
	
	/** Custom serialization, subclasses must override this */
	//决定这个结构体如何被序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
protected:
	
	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	
private:
};