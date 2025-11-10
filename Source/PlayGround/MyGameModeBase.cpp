// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	// UE_LOG(LogTemp, Warning, TEXT("안녕 세상아"));
	DefaultPawnClass = AMyPawn::StaticClass(); // CDO 포인터, 기본 객체, 결국은 클래스 이름
	PlayerControllerClass = AMyPlayerController::StaticClass();

}