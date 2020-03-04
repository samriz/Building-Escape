// Copyright Sameer Rizvi 2019

#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Engine.h" //for AddOnScreenDebugMessage()
#include "Engine/World.h" //for GetWorld() and GetFirstPlayerController()
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h" //for GetPawn()

#define OUT

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
	OpenAngle += InitialYaw;
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn(); //need to specify first player controller as there can be more than one controller in the scene

	if(!PressurePlate)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Yellow, FString::Printf(TEXT("%s has the OpenDoor component on it but no pressure plate."), *GetOwner()->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the object has a pressure plate attached to it and the Pressure Plate comes into contact with our pawn
	//if(PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens)) 
	if(GetTotalMassOfActors() > MassToOpenDoors)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	//else if(ActorsMass < 40.f) GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Yellow, FString::Printf(TEXT("No enough mass on the Pressure Plate.")));
	else 
	{
		//if the door has been open longer than DoorCloseDelay, then shut the door
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay) CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorSpeed); //this is how we open the door. We go from CurrentYaw to TargetYaw at a speed of 0.02. DeltaTime ensure framerate independence (door will open at same speed no matter what framerate the game is running on)
	FRotator DoorRotation = GetOwner()->GetActorRotation(); //this is what our current doors' rotations are
	DoorRotation.Yaw = CurrentYaw; //the yaw of DoorRotation needs to be the linear interpolation of CurrentYaw and TargetYaw
	GetOwner()->SetActorRotation(DoorRotation); //set this door's rotations to DoorRotation
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::GetTotalMassOfActors()
{
	float TotalMass = 0.f;

	if (!PressurePlate)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Yellow, FString::Printf(TEXT("PressurePlate is not set for %s"), *GetOwner()->GetName()));
		//UE_LOG(LogTemp, Warning, TEXT("PressurePlate is not set for %s"), *GetOwner()->GetName());
		return TotalMass;
	}

	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//add up their masses
	for(AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}