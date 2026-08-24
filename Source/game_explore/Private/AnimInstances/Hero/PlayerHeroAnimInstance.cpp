// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/PlayerHeroAnimInstance.h"
#include "Characters/PlayerHeroCharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

void UPlayerHeroAnimInstance::AnimNotify_TurnEnd()
{
    if (!bIsTurning)
    {
        return;
    }

    if (APlayerHeroCharacter* HeroCharacter = Cast<APlayerHeroCharacter>(TryGetPawnOwner()))
    {
        HeroCharacter->FinishTurnInPlace();
    }

    bIsTurning = false;
    bShouldTurn = false;
    TurnRecoverTime = bIsArmed ? 0.03f : TurnRecoverDuration;
}

void UPlayerHeroAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    if (OwningCharacter)
    {
        OwningHeroCharacter = Cast<APlayerHeroCharacter>(OwningCharacter);
    }
}

void UPlayerHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

    if (bHasAcceleration)
    {
        IdleElpasedTime = 0.f;
        bShouldEnterRelaxState = false;
    }
    else
    {
        IdleElpasedTime += DeltaSeconds;

        bShouldEnterRelaxState = (IdleElpasedTime >= EnterRelaxtStateThreshold);
    }
}

void UPlayerHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* Pawn = TryGetPawnOwner();
    if (!Pawn) return;

    ACharacter* Char = Cast<ACharacter>(Pawn);
    if (!Char) return;

    APlayerHeroCharacter* HeroCharacter = Cast<APlayerHeroCharacter>(Pawn);

    Speed = Pawn->GetVelocity().Size2D();

    const FVector LocalVelocity =
        Pawn->GetActorTransform().InverseTransformVectorNoScale(Pawn->GetVelocity());

    MovementDirection = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));

    if (Char->GetCharacterMovement())
    {
        bIsInAir = Char->GetCharacterMovement()->IsFalling();
    }

    if (HeroCharacter)
    {
        bDisableTurnInPlace = HeroCharacter->bDisableTurnInPlace;
    }

    if (TurnRecoverTime > 0.f)
    {
        TurnRecoverTime -= DeltaSeconds;

        if (TurnRecoverTime < 0.f)
        {
            TurnRecoverTime = 0.f;
        }
    }

    const FRotator ControlRot = Pawn->GetControlRotation();

    const float ControlYaw = ControlRot.Yaw;
    const float ActorYaw = Pawn->GetActorRotation().Yaw;

    const float RawDeltaYaw = FMath::FindDeltaAngleDegrees(ActorYaw, ControlYaw);

    DeltaYaw = FMath::Clamp(RawDeltaYaw, -90.f, 90.f);

    float Pitch = ControlRot.Pitch;

    bIsArmed = HeroCharacter && HeroCharacter->IsArmed();

    const float ActiveTurnThreshold = bIsArmed ? 45.f : TurnThreshold;

    if (!bHasPreviousControlYaw)
    {
        PreviousControlYaw = ControlYaw;
        PreviousActorYaw = ActorYaw;
        ContinuousAimYaw = RawDeltaYaw;
        bHasPreviousControlYaw = true;
    }

    const float InputYawDelta = FMath::FindDeltaAngleDegrees(PreviousControlYaw, ControlYaw);
    const float ActorYawDelta = FMath::FindDeltaAngleDegrees(PreviousActorYaw, ActorYaw);

    PreviousControlYaw = ControlYaw;
    PreviousActorYaw = ActorYaw;

    ContinuousAimYaw += InputYawDelta - ActorYawDelta;

    const float StableDeltaYaw = bIsArmed ? ContinuousAimYaw : RawDeltaYaw;

    const float ControlYawSpeed = FMath::Abs(InputYawDelta) / FMath::Max(DeltaSeconds, KINDA_SMALL_NUMBER);

    if (!bIsTurning && ControlYawSpeed < 15.f)
    {
        TurnInputSettleTime += DeltaSeconds;
    }
    else
    {
        TurnInputSettleTime = 0.f;
    }

    if (Pitch > 180.f)
    {
        Pitch -= 360.f;
    }

    LookPitch = FMath::Clamp(Pitch, -60.f, 60.f);

    const bool bInputSettled = TurnInputSettleTime >= TurnInputSettleDelay;

    const bool bTurnInputReady = bIsArmed || bInputSettled;

    const bool bCanStartTurn = !bIsTurning && TurnRecoverTime <= 0.f && !bDisableTurnInPlace && !bIsInAir && Speed < 3.f && bTurnInputReady && FMath::Abs(StableDeltaYaw) > ActiveTurnThreshold;

    if (bCanStartTurn)
    {
        TurnInputSettleTime = 0.f;

        const float SignedTurnDeltaYaw = FMath::Clamp(StableDeltaYaw, -180.f, 180.f);

        const float TurnAngle = FMath::Abs(SignedTurnDeltaYaw);

        bIsTurning = true;
        bShouldTurn = true;
        bTurnRight = SignedTurnDeltaYaw > 0.f;

        CurrentTurnTime = 0.f;
        TurnStartDeltaYaw = SignedTurnDeltaYaw;

        CurrentTurnDuration = FMath::GetMappedRangeValueClamped(FVector2D(ActiveTurnThreshold, 180.f), FVector2D(MaxTurnDuration, 1.2f), TurnAngle);

        TurnPlayRate = MaxTurnDuration / CurrentTurnDuration;

        if (HeroCharacter)
        {
            HeroCharacter->StartTurnInPlace(SignedTurnDeltaYaw, CurrentTurnDuration);
        }
    }

    if (bIsTurning)
    {
        CurrentTurnTime += DeltaSeconds;

        if (CurrentTurnTime >= CurrentTurnDuration)
        {
            bIsTurning = false;
            bShouldTurn = false;

            TurnRecoverTime = bIsArmed ? 0.03f : TurnRecoverDuration;

            if (HeroCharacter)
            {
                HeroCharacter->FinishTurnInPlace();

                const float FinalActorYaw = HeroCharacter->GetActorRotation().Yaw;

                const float FinalControlYaw = HeroCharacter->GetControlRotation().Yaw;

                ContinuousAimYaw = FMath::FindDeltaAngleDegrees( FinalActorYaw, FinalControlYaw );

                PreviousActorYaw = FinalActorYaw;
                PreviousControlYaw = FinalControlYaw;

                ArmedAimYaw = FMath::Clamp( ContinuousAimYaw, -60.f, 60.f );
            }
        }
    }

    bShouldTurn = bIsTurning;

    if (bIsArmed)
    {

        ArmedAimYaw = FMath::Clamp(ContinuousAimYaw, -60.f, 60.f);

        HeadYaw = 0.f;
        ChestYaw = 0.f;
        WaistYaw = 0.f;
    }

    else if (bIsTurning || TurnRecoverTime > 0.f)
    {
        HeadYaw = FMath::FInterpTo(HeadYaw, 0.f, DeltaSeconds, 10.f);
        ChestYaw = FMath::FInterpTo(ChestYaw, 0.f, DeltaSeconds, 10.f);
        WaistYaw = FMath::FInterpTo(WaistYaw, 0.f, DeltaSeconds, 10.f);
    }
    else
    {
        HeadYaw = FMath::FInterpTo(HeadYaw, DeltaYaw, DeltaSeconds, 8.f);
        ChestYaw = FMath::FInterpTo(ChestYaw, HeadYaw, DeltaSeconds, 6.f);
        WaistYaw = FMath::FInterpTo(WaistYaw, ChestYaw, DeltaSeconds, 3.f);
    }
}
