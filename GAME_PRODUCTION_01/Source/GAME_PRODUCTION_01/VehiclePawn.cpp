// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

static const FName NAME_SteerInput("MoveRight");
static const FName NAME_ThrottleInput("MoveForward");

AVehiclePawn::AVehiclePawn()
{

	//just gonna use some Unity functions that already are defined in term of vehicles. we can play with the if we want but in this case we are just going to use them.
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	//Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;

	//Torque Setup
	Vehicle4W->MaxEngineRPM = 5700.f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	//Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	//Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	//Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.f;
	SpringArm->bUsePawnControlRotation = true;

	//Create the chase camera component, also avoid the camera going thru walls
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.f;
}

void AVehiclePawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateInAirControl(DeltaSeconds);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Axis
	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AVehiclePawn::ApplyThrottle);
	PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AVehiclePawn::ApplySteering);

	PlayerInputComponent->BindAxis("LookUp", this, &AVehiclePawn::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AVehiclePawn::LookRight);

	//Actions
	PlayerInputComponent->BindAction("HandBreak", IE_Pressed, this, &AVehiclePawn::OnHandBrakePressed);
	PlayerInputComponent->BindAction("HandBreak", IE_Released, this, &AVehiclePawn::OnHandBrakeReleased);

}

void AVehiclePawn::ApplyThrottle(float val)
{
	GetVehicleMovementComponent()->SetThrottleInput(val);
}

void AVehiclePawn::ApplySteering(float val)
{
	GetVehicleMovementComponent()->SetSteeringInput(val);
}

void AVehiclePawn::LookUp(float val)
{
	if (val != 0.0f)
	{
		AddControllerPitchInput(val);
	}
}

void AVehiclePawn::LookRight(float val)
{
	if (val != 0.0f)
	{
		AddControllerYawInput(val);
	}
}

void AVehiclePawn::OnHandBrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AVehiclePawn::OnHandBrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void AVehiclePawn::UpdateInAirControl(float DeltaTime)
{
}
