// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DoorOpener.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();
	UPROPERTY(BlueprintAssignable)
		FOnOpenRequest onOpenRequest;
	UPROPERTY(BlueprintAssignable)
		FOnOpenRequest onCloseRequest;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		float targetRotation = 60.0f;

private:
	UPROPERTY(EditAnywhere)
		float triggerMass = 35.0f;


	UPROPERTY(EditAnywhere)
		ATriggerVolume* pressurePlate = nullptr;

	AActor* actorThatOpens = nullptr;

	AActor* owner;
	float openTime;


	void OpenDoor();
	void CloseDoor(); 
	float GetTotalMassOfActorsOnPlate();

	UFUNCTION(BlueprintCallable)
		float getTargetAngle();
};
