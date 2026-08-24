// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "HeroCombatComponent.generated.h"

class APlayerHeroWeaponBase;
/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UHeroCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Player|Combat")
	APlayerHeroWeaponBase* GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag) const;
};
