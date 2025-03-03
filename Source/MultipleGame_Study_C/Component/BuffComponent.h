#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLEGAME_STUDY_C_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ACharactor_WhiteMan;

public:	
	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Heal(float HealAmount, float HealingTime);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ACharactor_WhiteMan* WhiteMan;

	bool bHealing = false;
	float HealingRate = 0;
	float AmountToHeal = 0.f;
};
