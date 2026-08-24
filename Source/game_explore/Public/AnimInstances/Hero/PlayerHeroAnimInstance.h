// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlayerCharacterAnimInstance.h"
#include "PlayerHeroAnimInstance.generated.h"

class APlayerHeroCharacter;

/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UPlayerHeroAnimInstance : public UPlayerCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AnimNotify_TurnEnd();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float DeltaYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float LookPitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float HeadYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float ChestYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float WaistYaw = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	float TurnThreshold = 70.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bShouldTurn = false;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bTurnRight = false;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bIsTurning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bDisableTurnInPlace = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	float TurnRecoverDuration = 0.2f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnRecoverTime = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	float MaxTurnDuration = 0.8f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float CurrentTurnTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnStartDeltaYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float MovementDirection = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsArmed = false;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float CurrentTurnDuration = 0.8f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnPlayRate = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	float TurnInputSettleDelay = 0.12f;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float ArmedAimYaw = 0.f;

	float PreviousActorYaw = 0.f;

	float ContinuousAimYaw = 0.f;

	float TurnInputSettleTime = 0.f;

	float PreviousControlYaw = 0.f;

	bool bHasPreviousControlYaw = false;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|Refrences")
	APlayerHeroCharacter* OwningHeroCharacter;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bShouldEnterRelaxState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float EnterRelaxtStateThreshold = 5.f;

	float IdleElpasedTime;
};
