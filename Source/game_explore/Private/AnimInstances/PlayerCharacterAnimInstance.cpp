// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<APlayerBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UPlayerCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	bIsFalling = OwningMovementComponent->IsFalling();
}

