// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PlayerBaseCharacter.h"
#include "GameplayTagContainer.h"
#include "PlayerHeroCharacter.generated.h"

class UCameraComponent;
class UDataAsset_InputConfig;
class USceneComponent;
class USkeletalMeshComponent;
class UHeroCombatComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API APlayerHeroCharacter : public APlayerBaseCharacter
{
	GENERATED_BODY()


public:
	APlayerHeroCharacter();

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bDisableTurnInPlace = false;

	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void SetCinematicActive(bool bActive);

	UFUNCTION(BlueprintPure, Category = "Cinematic")
	bool IsCinematicActive() const
	{
		return bIsCinematicActive;
	}

	void StartTurnInPlace(float InSignedDeltaYaw, float InDuration);
	void FinishTurnInPlace();


protected:

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void UpdateFootIK(float DeltaSeconds);
	void AttachCameraPivotToSocket();
	float TraceFootOffset(const FName& FootBoneName) const;
	bool bStartupDataGranted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Cinematic",meta = (AllowPrivateAccess = "true"))
	bool bIsCinematicActive = false;
	


#pragma region Components


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TrueFirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

#pragma endregion

#pragma region Inputs

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	FVector2D CachedMoveInput = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Movement|Facing")
	float ForwardFacingInterpSpeed = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float WalkSpeed = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Dash")
	float DashSpeed = 360.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement|Dash")
	bool bIsDashing = false;

	bool bDashInputHeld = false;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_JumpStart();
	void Input_JumpStop();
	void Input_MoveStop();
	void Input_DashStart();
	void Input_DashStop();
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	

#pragma endregion


	UPROPERTY(BlueprintReadOnly, Category = "FootIK")
	float LeftFootOffsetZ = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "FootIK")
	float RightFootOffsetZ = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "FootIK")
	float PelvisOffsetZ = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	bool bIsTurnInPlaceActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnStartYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnTargetYaw = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnElapsedTime = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnDuration = 0.8f;

	UPROPERTY(BlueprintReadOnly, Category = "Turn")
	float TurnDeltaYaw = 0.f;


public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const {return HeroCombatComponent;}

	UFUNCTION(BlueprintPure, Category = "Combat")
	bool IsArmed() const;
};

