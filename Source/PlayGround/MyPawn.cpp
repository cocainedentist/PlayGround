// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"

#include "MyActor.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 에디터 드래그 앤 드롭, 설정은 전부 다 코드로 변경 가능
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector (46.0f, 62.0f, 10.f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	// 개발할때는 거의 안함. -> 연습이니까 하는거다 실제로는 안해
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Box);
	Left->SetRelativeLocation(FVector(37.f, -21.f, 1.f));

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Box);
	Right->SetRelativeLocation(FVector(37.f, 21.f, 1.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->SocketOffset = FVector(0, 0, 50.0f);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bEnableCameraLag = true;
	SpringArm->SocketOffset = FVector(0, 0, 50.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(70.f, 0, 0));

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	//Movement->MaxSpeed = 2000.f;
	//Movement->MaxSpeed = 2000.f;

	MyActor = AMyActor::StaticClass();


	static ConstructorHelpers::FObjectFinder<UInputAction> IA_FireAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (IA_FireAsset.Succeeded())
	{
		IA_Fire = IA_FireAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MovementAsset(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Movement.IA_Movement'"));
	if (IA_MovementAsset.Succeeded())
	{
		IA_Movement = IA_MovementAsset.Object;
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	/* 시작하면 Rocket 발사
	FVector SpawnLocation = Arrow->GetComponentLocation();
	//FRotator SpawnRotation = MyActor->GetActorRotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(MyActor, SpawnLocation, FRotator::ZeroRotator, SpawnParams);*/
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector());

	Left->AddLocalRotation(FRotator(0, 0, 3600.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 7200.0f));
	Right->AddLocalRotation(FRotator(0, 0, 3600.0f * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * 7200.0f));

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &AMyPawn::EnhancedFire);
		UIC->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &AMyPawn::ProcessMovement);
	}

	/* PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	//PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, AMyPawn::Fire);

	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll); */
}

void AMyPawn::EnhancedFire(const FInputActionValue& Value)
{
	Fire();
}

void AMyPawn::ProcessMovement(const FInputActionValue& Value)
{
	FVector2D WantedRotation = Value.Get<FVector2D>();
	//WantedRotation = WantedRotation * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	//AddActorLocalRotation(FRotator(WantedRotation.X, 0, WantedRotation.Y));
	Pitch(WantedRotation.Y);
	Roll(WantedRotation.X);
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(), Arrow->K2_GetComponentToWorld());
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(Value * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),0, 0));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0, 0, Value * 60.0 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

