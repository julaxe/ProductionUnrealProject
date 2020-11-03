// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "GameFramework/SpringArmComponent.h"

#include "VehiclePawn.generated.h"

/**
 *
 */
UCLASS()
class GAME_PRODUCTION_01_API AVehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	AVehiclePawn();

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//throttle/steering
	void ApplyThrottle(float val);
	void ApplySteering(float val);

	//Look around
	void LookUp(float val);
	void LookRight(float val);

	//handbrake
	void OnHandBrakePressed();
	void OnHandBrakeReleased();

	//Update in air Physics
	void UpdateInAirControl(float DeltaTime);

protected:

	
	//Spring arm that will offset the camera
	UPROPERTY(Category = "InAirMovement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float AirMovement;

	UPROPERTY(Category = "InAirMovement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float GroundMovement;
	//Camera component that will be our viewpoint
	/*UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;*/
};