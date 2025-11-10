// Fill out your copyright notice in the Description page of Project Settings.
// MyRocket.h


#include "MyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(28.f, 3.f, 4.f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Box);
	StaticMesh->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Rocket(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Rocket.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM_Rocket.Object);
	}

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	//SetLifeSpan(3.f);

	//Delay를 사용하기위한 code
	FLatentActionInfo Info;
	Info.ExecutionFunction = TEXT("Timeout");
	Info.CallbackTarget = this;
	Info.UUID = 0;
	Info.Linkage = 1;

	UKismetSystemLibrary::Delay(GetWorld(), 1.0f, Info);

	// Delegate UE
	OnActorBeginOverlap.AddDynamic(this, &AMyActor::ProcessActorBeginOverlap);
	OnActorBeginOverlap.AddDynamic(this, &AMyActor::ProcessActorBeginOverlap);
	OnActorBeginOverlap.Remove(this, TEXT("ProcessActorBeginOverlap"));


	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::Timeout()
{
	//UE_LOG(LogTemp, Warning, TEXT("DIE"));
	Destroy();
}

void AMyActor::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UGameplayStatics::ApplyDamage(OtherActor, 10.0f, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, nullptr);
}

