// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState_Character.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLEGAME_STUDY_C_API APlayerState_Character : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);

protected:
	virtual void OnRep_Score() override;
	UFUNCTION()
	virtual void OnRep_Defeats();

private:
	UPROPERTY()
	class ACharactor_WhiteMan* Character;
	UPROPERTY()
	class APlayerController_Character* PlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_Defeats)
	int32 Defeats;

};
