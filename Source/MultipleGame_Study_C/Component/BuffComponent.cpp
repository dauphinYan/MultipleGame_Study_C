#include "BuffComponent.h"
#include "MultipleGame_Study_C/Charactor/Charactor_WhiteMan.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{

}

