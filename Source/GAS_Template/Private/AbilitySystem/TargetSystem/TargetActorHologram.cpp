// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetSystem/TargetActorHologram.h"
#include "Abilities/GameplayAbility.h"
#include "DrawDebugHelpers.h"

ATargetActorHologram::ATargetActorHologram()
{
	PrimaryActorTick.bCanEverTick = true;
	LongOfTrace = 0;

}

void ATargetActorHologram::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	PrimaryPC = Cast<APlayerController>(Ability->GetAvatarActorFromActorInfo()->GetInstigatorController());
	MasterPawn = PrimaryPC->GetPawn();
	SetInstigator(InstigatorPawn);

}

// This function is called when Wait Target Data Task is confirmed
void ATargetActorHologram::ConfirmTargetingAndContinue()
{
	InstigatorPawn->SetActorLocation(this->GetActorLocation());
	FGameplayAbilityTargetDataHandle TargetData;
	TargetDataReadyDelegate.Broadcast(TargetData);

}

void ATargetActorHologram::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	FHitResult HitResult;

	FVector StartTrace = MasterPawn->GetActorLocation();
	StartTrace.Z += HeightOfTrace;


	FVector ViewPoint;
	FRotator ViewRotation;
	PrimaryPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FVector RotationVector = ViewRotation.Vector();
	
	LongOfTrace = LongOfActorCurve->GetFloatValue(ViewRotation.Pitch);
	UE_LOG(LogTemp, Warning, TEXT("%f"), ViewRotation.Pitch);
	UE_LOG(LogTemp, Warning, TEXT("%f"), LongOfTrace);

	RotationVector = FVector(RotationVector.X * LongOfTrace, RotationVector.Y * LongOfTrace, 0);


	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, StartTrace + RotationVector, ECC_Visibility, QueryParams);
	if (bDebug)
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Orange, false, 0.f, 0, 3.f);


	FHitResult HitResult2;
	bool TryTrace2 = GetWorld()->LineTraceSingleByChannel(HitResult2, HitResult.TraceEnd, FVector(HitResult.TraceEnd.X, HitResult.TraceEnd.Y,HitResult.TraceEnd.Z * -1), ECC_Visibility, QueryParams);
	if (bDebug)
		DrawDebugLine(GetWorld(), HitResult2.TraceStart, HitResult2.TraceEnd, FColor::Blue, false, 0.f, 0, 3.f);

	SetActorLocation(HitResult2.Location);
}
