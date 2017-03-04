// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	constexpr float PLAYER_REACH_FOR_GRABBING = 100.0f;

	reach = PLAYER_REACH_FOR_GRABBING;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber instance created!"));
	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point
 	APlayerController *player = GetWorld()->GetFirstPlayerController();
	FRotator rotation;
	FVector location;
	player->GetPlayerViewPoint( OUT location, OUT rotation);
	UE_LOG(LogTemp, Warning, TEXT("Parameters modified:\nLocation: %s\nRotation: %s"), *location.ToString(),*rotation.ToString());

	//DrawDebugLine(GetWorld(), location, location + rotation.Vector()*reach, FColor(255, 0, 0, 255),false,0.0f,0.0f,10.0f);
	
	FHitResult objectHit;
	FCollisionQueryParams parameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT objectHit,
		location,
		location + rotation.Vector() * reach,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		parameters
		);
	AActor* actorHit = objectHit.GetActor();
	if (actorHit) {
		UE_LOG(LogTemp,Warning,TEXT("Line trace hit: %s"), *(actorHit->GetName()))
	}
}

