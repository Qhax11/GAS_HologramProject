// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TargetSystem/TargetActorHologram.h"
#include "Abilities/GameplayAbility.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemLog.h"

ATargetActorHologram::ATargetActorHologram()
{
	PrimaryActorTick.bCanEverTick = true;
	LenghtOfTrace = 0;

	// Doesent work??
	bDestroyOnConfirmation = false;
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
	FVector Location = this->GetActorLocation();
	// This 90 is because of character's height
	Location = FVector(Location.X, Location.Y, Location.Z + 90);
	InstigatorPawn->SetActorLocation(Location);
	InstigatorPawn->SetActorRotation(this->GetActorRotation());

	BP_ActorBeforeDestroy();
	//ABILITY_LOG(Warning, TEXT("AGameplayAbilityTargetActor::CancelTargeting called with null ASC! Actor %s"), *GetName());

	// We need hologram ref in ability so we give the self ref.
	TArray<TWeakObjectPtr<AActor>> OverlapedActors;
	OverlapedActors.Add(this);
	FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlapedActors);
	TargetDataReadyDelegate.Broadcast(TargetData);

	// Its doing brodcast to task and its destroy the actor why idk?
	//Super::ConfirmTargetingAndContinue();
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

	FVector ViewPoint;
	FRotator ViewRotation;
	PrimaryPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FVector RotationVector = ViewRotation.Vector();
	
	if (!LenghtOfTraceCurve)
	{
		UE_LOG(LogTemp, Error, TEXT("The lenght curve is empty in % s"), *this->GetClass()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("The lenght curve is empty in % s"), *this->GetClass()->GetName()));
		return;
	}

	// We get lenght of trace from curve graph and this lenght connected to pitch of player view's rotation
	LenghtOfTrace = LenghtOfTraceCurve->GetFloatValue(ViewRotation.Pitch);
	RotationVector = FVector(RotationVector.X * LenghtOfTrace, RotationVector.Y * LenghtOfTrace, 0);

	// First trace for forward line
	FVector TraceStart = MasterPawn->GetActorLocation();
	TraceStart.Z += HeightOfTrace;
	FHitResult HitResult;
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceStart + RotationVector, ECC_Visibility, QueryParams);

	if (bDebug)
		DrawDebugLine(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd, FColor::Orange, false, 0.f, 0, 3.f);

	// This trace is for down line and we set location of hologram actor to down trace's hit location
	FHitResult HitResult2;
	// We draw trace to down
	FVector DownVector = FVector(HitResult.TraceEnd.X, HitResult.TraceEnd.Y, HitResult.TraceEnd.Z * -1);
	bool TryTrace2 = GetWorld()->LineTraceSingleByChannel(HitResult2, HitResult.TraceEnd, DownVector, ECC_Visibility, QueryParams);
	if (bDebug)
		DrawDebugLine(GetWorld(), HitResult2.TraceStart, HitResult2.TraceEnd, FColor::Blue, false, 0.f, 0, 3.f);

	SetActorLocation(HitResult2.Location);


}

