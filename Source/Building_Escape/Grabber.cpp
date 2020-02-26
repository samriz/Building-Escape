// Copyright Sameer Rizvi 2019

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	//check for Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle) UE_LOG(LogTemp, Warning, TEXT("Physics handle component found"))
	if(!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("The following object does not have a Physics Handle Component attached to it: %s"), *GetOwner()->GetName())
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if(InputComponent) 
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab); //passing address of function, not calling function

		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release); //passing address of function, not calling function
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."))

	GetFirstPhysicsBodyInReach();//only raycast when key is pressed and see if we reach any actors with physics body collision channel set

	//if we hit something then attach the physics handle
	//TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released."))
	//TODO remove/release the physics handle
}

// Called every frame
//TickComponent is a "hot loop" because it is called often
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the physics handle is attached, move the object we are holding

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	//Get player's viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);

	//draw a line from player showing reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(
		GetWorld(), 
		PlayerViewPointLocation, 
		LineTraceEnd, 
		FColor(0,255,0), 
		false, 
		0.f,
		0, 
		5.f
	);

	FHitResult Hit;
	//ray-cast out to a certain distance (reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit, //First blocking hit found
		PlayerViewPointLocation, //Start location of the ray
		LineTraceEnd, //End location of the ray
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), //List of object types it's looking for
		TraceParams //Additional parameters used for the trace
	); //TRUE if any hit is found

	//see what it hits
	AActor* ActorHit = Hit.GetActor();

	//log out to test
	if(ActorHit) UE_LOG(LogTemp, Warning, TEXT("Line trace has hit: %s"), *ActorHit->GetName())

	return Hit;
}