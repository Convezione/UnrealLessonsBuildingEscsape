// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "DoorOpener.h"


// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
	actorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!pressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *owner->GetName());
	// ...
	
}

void UDoorOpener::OpenDoor() {

	//owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
	onOpenRequest.Broadcast();
}
void UDoorOpener::CloseDoor() {
	onCloseRequest.Broadcast();
	//owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	if (GetTotalMassOfActorsOnPlate()>=triggerMass) 
		OpenDoor();
	else
		CloseDoor();
	// ...
}

float UDoorOpener::GetTotalMassOfActorsOnPlate() {
	float totalMass = 0.0f;
	TSet<AActor*> overlappingActors;
	if (!pressurePlate)
		return totalMass;
	pressurePlate->GetOverlappingActors(OUT overlappingActors);
	for (AActor*& actor : overlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return totalMass;
}

