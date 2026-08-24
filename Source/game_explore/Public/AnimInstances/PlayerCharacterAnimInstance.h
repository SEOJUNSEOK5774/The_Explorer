// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlayerBaseAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

class APlayerBaseCharacter;
class UCharacterMovementComponent;


/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UPlayerCharacterAnimInstance : public UPlayerBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	APlayerBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	bool bIsFalling ;

	
};
