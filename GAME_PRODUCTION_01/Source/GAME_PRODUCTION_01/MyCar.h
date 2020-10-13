// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "GameFramework/SpringArmComponent.h"

#include "MyCar.generated.h"

/**
 *
 */
UCLASS()
class GAME_PRODUCTION_01_API AMyCar : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	AMyCar();

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
	UPROPERTY(Category = Camera, EditDefaultsOnly, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	//Camera component that will be our viewpoint
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
};
