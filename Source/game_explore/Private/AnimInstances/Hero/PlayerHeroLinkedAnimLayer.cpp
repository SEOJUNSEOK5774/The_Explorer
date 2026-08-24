// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/PlayerHeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/PlayerHeroAnimInstance.h"

UPlayerHeroAnimInstance* UPlayerHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
    return Cast<UPlayerHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
