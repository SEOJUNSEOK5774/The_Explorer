// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/PlayerInputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "abilitysystem/PlayerAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"

#include "PlayerDebugHelper.h"

APlayerHeroCharacter::APlayerHeroCharacter()

{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 180.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.25f;
	GetCharacterMovement()->GravityScale = 1.0f;

	// 전체 캐릭터 Mesh = 3인칭 / 그림자용
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetCastShadow(true);
	GetMesh()->bCastHiddenShadow = true;

	// 1인칭용 머리 없는 Mesh
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(GetMesh());

	FirstPersonMesh->SetRelativeLocation(FVector::ZeroVector);
	FirstPersonMesh->SetRelativeRotation(FRotator::ZeroRotator);
	FirstPersonMesh->SetRelativeScale3D(FVector::OneVector);

	// 내 화면에서만 보이게
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetOwnerNoSee(false);

	// 1인칭용 메쉬는 그림자 끄기
	FirstPersonMesh->SetCastShadow(false);
	FirstPersonMesh->bCastHiddenShadow = false;

	// 전체 메쉬 애니메이션을 따라가게
	FirstPersonMesh->SetLeaderPoseComponent(GetMesh());

	CameraPivot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraPivot"));
	CameraPivot->SetupAttachment(GetMesh());
	CameraPivot->SetRelativeLocation(FVector::ZeroVector);
	CameraPivot->SetRelativeRotation(FRotator::ZeroRotator);
	CameraPivot->SetAbsolute(false, true, false);

	TrueFirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TrueFirstPersonCamera"));
	TrueFirstPersonCamera->SetupAttachment(CameraPivot);
	TrueFirstPersonCamera->SetRelativeLocation(FVector(3.f, 0.f, 1.f));
	TrueFirstPersonCamera->SetRelativeRotation(FRotator::ZeroRotator);
	TrueFirstPersonCamera->bUsePawnControlRotation = false;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

}

void APlayerHeroCharacter::SetCinematicActive(bool bActive)
{
	bIsCinematicActive = bActive;

	if (bIsCinematicActive)
	{
		// 진행 중인 Turn In Place 즉시 취소
		bIsTurnInPlaceActive = false;
		bDisableTurnInPlace = true;

		TurnElapsedTime = 0.f;
	}
	else
	{
		// 다음 Tick에서 기존 이동/무장 조건에 맞게 다시 결정됨
		bDisableTurnInPlace = false;
	}
}

void APlayerHeroCharacter::StartTurnInPlace(float InSignedDeltaYaw, float InDuration)
{
	bIsTurnInPlaceActive = true;

	TurnStartYaw = GetActorRotation().Yaw;

	TurnDeltaYaw = FMath::Clamp(InSignedDeltaYaw, -180.f, 180.f);

	TurnTargetYaw = FRotator::NormalizeAxis(TurnStartYaw + TurnDeltaYaw);

	TurnElapsedTime = 0.f;

	TurnDuration = FMath::Max(InDuration, KINDA_SMALL_NUMBER);


}

void APlayerHeroCharacter::FinishTurnInPlace()
{
	if (!bIsTurnInPlaceActive)
	{
		return;
	}

	bIsTurnInPlaceActive = false;

	SetActorRotation(
		FRotator(0.f, TurnTargetYaw, 0.f)
	);


}

void APlayerHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority() || bStartupDataGranted)
	{
		return;
	}

	if (UDataAsset_StartUpDataBase* StartupData =CharacterStartUpData.LoadSynchronous())
	{
		StartupData->GiveToAbilitySystemComponent(PlayerAbilitySystemComponent);

		bStartupDataGranted = true;
	}
}

void APlayerHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayInputComponent)
{
	if (!InputConfigDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("InputConfigDataAsset is not assigned on %s"), *GetName());
		return;
	}

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem);

	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UPlayerInputComponent* PlayerInputComponent = CastChecked<UPlayerInputComponent>(PlayInputComponent);

	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, PlayerGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, PlayerGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset,PlayerGameplayTags::InputTag_Dash,ETriggerEvent::Started,this,&ThisClass::Input_DashStart);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset,PlayerGameplayTags::InputTag_Dash,ETriggerEvent::Completed,this,&ThisClass::Input_DashStop);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset,PlayerGameplayTags::InputTag_Dash,ETriggerEvent::Canceled,this,&ThisClass::Input_DashStop);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, PlayerGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &ThisClass::Input_JumpStart);
    PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, PlayerGameplayTags::InputTag_Jump, ETriggerEvent::Completed, this, &ThisClass::Input_JumpStop);
	PlayerInputComponent->BindNativeInputAction(InputConfigDataAsset, PlayerGameplayTags::InputTag_Move, ETriggerEvent::Completed, this, &ThisClass::Input_MoveStop);
	PlayerInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void APlayerHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttachCameraPivotToSocket();
	

}

void APlayerHeroCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (!MovementComponent)
	{
		return;
	}

	const float Speed2D = GetVelocity().Size2D();
	const bool bIsMoving = Speed2D > 3.f;

	const bool bForwardPressed = CachedMoveInput.Y > 0.1f;

	const bool bCanDash = !bIsCinematicActive && bDashInputHeld && bForwardPressed && !MovementComponent->IsFalling();

	bIsDashing = bCanDash;

	MovementComponent->MaxWalkSpeed = bIsDashing ? DashSpeed : WalkSpeed;

	MovementComponent->bOrientRotationToMovement = false;

	// 대시 등 기존의 일반 계산은 위쪽에서 계속 처리

	if (bIsCinematicActive)
	{
		bIsTurnInPlaceActive = false;
		bDisableTurnInPlace = true;
		TurnElapsedTime = 0.f;
	}
	else
	{
		// 기존 무장/비무장 조건을 그대로 유지
		bDisableTurnInPlace = bIsMoving;


		if (bIsMoving && bForwardPressed && Controller)
		{
			const float TargetYaw = Controller->GetControlRotation().Yaw;

			const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), FRotator(0.f, TargetYaw, 0.f), DeltaSeconds, ForwardFacingInterpSpeed);

			SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
		}

		// 기존 Turn In Place 처리
		if (bIsTurnInPlaceActive)
		{
			TurnElapsedTime += DeltaSeconds;

			if (IsArmed() && Controller)
			{
				// 무장은 Turn 중에도 계속 최신 카메라 방향을 목표로 사용
				TurnTargetYaw = Controller->GetControlRotation().Yaw;

				const FRotator TargetRotation(0.f,TurnTargetYaw,0.f);

				const FRotator NewRotation =FMath::RInterpTo(GetActorRotation(),TargetRotation,DeltaSeconds,8.f);

				SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
			}
			else
			{
				// 비무장은 기존 방식 유지
				const float Alpha =FMath::Clamp(TurnElapsedTime / TurnDuration,0.f,1.f);

				const float SmoothAlpha =FMath::InterpEaseInOut(0.f,1.f,Alpha,2.f);

				const float DeltaYaw =FMath::FindDeltaAngleDegrees(TurnStartYaw,TurnTargetYaw);

				const float NewYaw =FRotator::NormalizeAxis(TurnStartYaw + DeltaYaw * SmoothAlpha);

				SetActorRotation(FRotator(0.f, NewYaw, 0.f));
			}
		}

		// 시네마틱 중에는 실행되면 안 되는 카메라 회전
		if (Controller && CameraPivot)
		{
			FRotator CameraRotation = Controller->GetControlRotation();

			CameraRotation.Roll = 0.f;
			CameraPivot->SetWorldRotation(CameraRotation);
		}
	}
}


void APlayerHeroCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	AttachCameraPivotToSocket();
}


void APlayerHeroCharacter::UpdateFootIK(float DeltaSeconds)
{
	const float LeftOffset = TraceFootOffset(TEXT("Toes4_L"));
	const float RightOffset = TraceFootOffset(TEXT("Toes4_R"));

	LeftFootOffsetZ = FMath::FInterpTo(LeftFootOffsetZ, LeftOffset, DeltaSeconds, 12.f);
	RightFootOffsetZ = FMath::FInterpTo(RightFootOffsetZ, RightOffset, DeltaSeconds, 12.f);

	const float TargetPelvis = FMath::Min(LeftFootOffsetZ, RightFootOffsetZ);
	PelvisOffsetZ = FMath::FInterpTo(PelvisOffsetZ, TargetPelvis, DeltaSeconds, 10.f);
}

void APlayerHeroCharacter::AttachCameraPivotToSocket()
{
	static const FName CameraSocketName(TEXT("CameraSocket"));

	if (!CameraPivot || !GetMesh())
	{
		return;
	}

	if (GetMesh()->DoesSocketExist(CameraSocketName))
	{
		CameraPivot->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			CameraSocketName
		);

		CameraPivot->SetRelativeLocation(FVector::ZeroVector);
		CameraPivot->SetRelativeRotation(FRotator::ZeroRotator);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CameraSocket not found on character mesh."));
	}
}

float APlayerHeroCharacter::TraceFootOffset(const FName& FootBoneName) const
{
	if (!GetMesh()) return 0.f;

	const FVector FootLocation = GetMesh()->GetSocketLocation(FootBoneName);

	const FVector TraceStart = FootLocation + FVector(0.f, 0.f, 20.f);
	const FVector TraceEnd = FootLocation - FVector(0.f, 0.f, 80.f);

	UE_LOG(LogTemp, Warning, TEXT("Trace Bone: %s"), *FootBoneName.ToString());

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 0.1f, 0, 2);

	if (bHit)
	{
		DrawDebugSphere(GetWorld(), Hit.Location, 5, 12, FColor::Red, false, 0.1f);

		float Offset = Hit.Location.Z - FootLocation.Z;

		UE_LOG(LogTemp, Warning, TEXT("HIT | Bone: %s Offset: %.2f"),
			*FootBoneName.ToString(),
			Offset);

		return Offset;
	}

	UE_LOG(LogTemp, Warning, TEXT("MISS | Bone: %s"), *FootBoneName.ToString());

	return 0.0f;
}

void APlayerHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	CachedMoveInput = MovementVector;

	if (!Controller) return;


	const FRotator ControlRot = Controller->GetControlRotation();
	const FRotator MovementRotation(0.f, ControlRot.Yaw, 0.f);

	const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

	if (MovementVector.Y != 0.f)
	{
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}

}
void APlayerHeroCharacter::Input_JumpStart()
{
	Jump();
}


void APlayerHeroCharacter::Input_JumpStop()
{
	StopJumping();
}

void APlayerHeroCharacter::Input_MoveStop()
{
	CachedMoveInput = FVector2D::ZeroVector;
}

void APlayerHeroCharacter::Input_DashStart()
{
	bDashInputHeld = true;
}

void APlayerHeroCharacter::Input_DashStop()
{
	bDashInputHeld = false;
}

void APlayerHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	PlayerAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void APlayerHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	PlayerAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

bool APlayerHeroCharacter::IsArmed() const
{
	return HeroCombatComponent && HeroCombatComponent->CurrentEquippedWeaponTag.IsValid();
}


