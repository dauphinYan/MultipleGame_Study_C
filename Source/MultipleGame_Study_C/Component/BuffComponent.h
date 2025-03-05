#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLEGAME_STUDY_C_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ACharactor_WhiteMan;

public:
	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Heal(float HealAmount, float HealingTime);
	void BuffSpeed(float BaseSpeed, float CrouchSpeed, float BuffTime);
	void SetInitialSpeed(float BaseSpeed, float CrouchSpeed);
	void ResetSpeed();
protected:
	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);
private:
	UPROPERTY()
	class ACharactor_WhiteMan* Character;

	bool bHealing = false;
	float HealingRate = 0;
	float AmountToHeal = 0.f;

	FTimerHandle SpeedBuffTimer;
	float InitialBaseSpeed;
	float InitialCrouchSpeed;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpeedBuff(float BaseSpeed, float CrouchSpeed);
};
