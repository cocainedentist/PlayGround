// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTarget.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyTarget::AMyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Target(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_Target.SM_Target'"));
	if (SM_Target.Succeeded())
	{
		GetStaticMeshComponent()->SetStaticMesh(SM_Target.Object);
	}
	//GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
	GetStaticMeshComponent()->SetCollisionProfileName(TEXT("OverlapAll"), true);

	//GetStaticMeshComponent()->SetStaticMesh();
	//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//RootComponent = StaticMesh;
	//StaticMesh->SetRelativeTransform

}

// Called when the game starts or when spawned
void AMyTarget::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AMyTarget::ProcessTakeDamage);
	
}

// Called every frame
void AMyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyTarget::ProcessTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Destroy();
}