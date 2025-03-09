#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LagCompensationComponent.generated.h"

USTRUCT(BlueprintType)
struct FBoxInformation
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector BoxExtent;
};

USTRUCT(BlueprintType)
struct FFramePackage
{
	GENERATED_BODY()

	UPROPERTY()
	float Time;

	UPROPERTY()
	TMap<FName, FBoxInformation> HitBoxInfo;
};

USTRUCT(BlueprintType)
struct FServerSideRewindResult
{
	GENERATED_BODY()

	UPROPERTY()
	bool bHitConfirmed;

	UPROPERTY()
	bool bHeadShot;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MULTIPLEGAME_STUDY_C_API ULagCompensationComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class ACharactor_WhiteMan;

public:
	ULagCompensationComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ShowFramePackage(const FFramePackage& Package, FColor Color);
	FServerSideRewindResult ServerSideRewind(ACharactor_WhiteMan* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation, float HitTime);

	UFUNCTION(Server, Reliable)
	void Server_ScoreRequest(
		ACharactor_WhiteMan* HitCharacter,
		const FVector_NetQuantize& TraceStart,
		const FVector_NetQuantize& HitLocation,
		float HitTime,
		class AWeapon* DamageCauser
	);

protected:
	virtual void BeginPlay() override;
	void SaveFramePackage(FFramePackage& Package);
	FFramePackage InterBetweenFrames(const FFramePackage& OlderFrame, const FFramePackage& YoungerFrame, float HitTime);
	FServerSideRewindResult ConfirmHit(const FFramePackage& Package, ACharactor_WhiteMan* HitCharacter, const FVector_NetQuantize& TraceStart, const FVector_NetQuantize& HitLocation);
	void CacheBoxPositions(ACharactor_WhiteMan* HitCharacter, FFramePackage& OutFramePackage);
	void MoveBoxes(ACharactor_WhiteMan* HitCharacter, const FFramePackage& Package);
	void ResetBoxes(ACharactor_WhiteMan* HitCharacter, const FFramePackage& Package);
	void EnableCharacterMeshCollision(ACharactor_WhiteMan* HitCharacter, ECollisionEnabled::Type CollisionEnabled);
private:
	UPROPERTY()
	ACharactor_WhiteMan* Character;

	UPROPERTY()
	class APlayerController_Character* Controller;

	TDoubleLinkedList<FFramePackage> FrameHistory;

	UPROPERTY(EditDefaultsOnly)
	float MaxRecordTime = 4.f;
};
