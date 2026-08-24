// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/PlayerHeroWeaponBase.h"

void APlayerHeroWeaponBase::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> APlayerHeroWeaponBase::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}
