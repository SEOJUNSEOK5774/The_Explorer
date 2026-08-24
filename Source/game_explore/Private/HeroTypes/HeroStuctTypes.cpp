// Fill out your copyright notice in the Description page of Project Settings.


#include "HeroTypes/HeroStuctTypes.h"
#include "abilitySystem/Abilities/PlayerGameplayAbility.h"

bool FPlayerHeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGrant;
}
