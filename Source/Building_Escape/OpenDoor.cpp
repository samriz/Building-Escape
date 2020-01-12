// Copyright Sameer Rizvi 2019

#include "GameFramework/Actor.h"
#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString())
	UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), GetOwner()->GetActorRotation().Yaw)

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, DeltaTime * 1.f); //this is how we open the door. We go from CurrentYaw to TargetYaw at a speed of 0.02. DeltaTime ensure framerate independence (door will open at same speed no matter what framerate the game is running on)
	FRotator DoorRotation = GetOwner()->GetActorRotation(); //this is what our current doors' rotations are
	DoorRotation.Yaw = CurrentYaw; //the yaw of DoorRotation needs to be the linear interpolation of CurrentYaw and TargetYaw

	//if(PressurePlate->IsOverlappingActor(GetWorld()->GetFirstPlayerController()->GetPawn()))
	//{
		 GetOwner()->SetActorRotation(DoorRotation); //set this door's rotations to DoorRotation
	//}
}