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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindAttachedPhysicsComponent();
	SetupInputHandler();
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	if (HitResult.GetActor()) {
		collisionHandler->GrabComponent(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.GetComponent()->GetOwner()->GetActorLocation(), 
			true);
	}
}
void UGrabber::Release() {
	collisionHandler->ReleaseComponent();
}

void UGrabber::FindAttachedPhysicsComponent()
{
	collisionHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!collisionHandler) {
		UE_LOG(LogTemp, Error, TEXT("%s PhysicsHandle component not found!"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputHandler()
{
	inputHandler = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!inputHandler) {
		UE_LOG(LogTemp, Error, TEXT("%s Input component not found!"), *(GetOwner()->GetName()));
	}
	else {
		inputHandler->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		inputHandler->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player view point
	APlayerController *player = GetWorld()->GetFirstPlayerController();
	FRotator rotation;
	FVector location;
	player->GetPlayerViewPoint(OUT location, OUT rotation);
	//UE_LOG(LogTemp, Warning, TEXT("Parameters modified:\nLocation: %s\nRotation: %s"), *location.ToString(),*rotation.ToString());

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
	return objectHit;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (collisionHandler->GrabbedComponent) {
		APlayerController *player = GetWorld()->GetFirstPlayerController();
		FRotator rotation;
		FVector location;
		player->GetPlayerViewPoint(OUT location, OUT rotation);
		collisionHandler->SetTargetLocation(location + rotation.Vector() * reach);
	}
}

