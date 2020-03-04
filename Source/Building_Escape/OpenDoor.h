// Copyright Sameer Rizvi 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float);
	void CloseDoor(float);
	float GetTotalMassOfActors();

private:
	//private variables
	float InitialYaw, CurrentYaw;

	UPROPERTY(EditAnywhere) //expose TargetYaw to the editor
	float OpenAngle{90.f};

	UPROPERTY(EditAnywhere)
	float DoorSpeed{1.f};

	float DoorLastOpened{0.f};

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay{0.1f};

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;

	AActor* ActorThatOpens;
};