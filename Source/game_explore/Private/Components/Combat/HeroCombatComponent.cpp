// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HeroCombatComponent.h"
#include "Items/Weapons/PlayerHeroWeaponBase.h"

APlayerHeroWeaponBase* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
    return Cast<APlayerHeroWeaponBase>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}
