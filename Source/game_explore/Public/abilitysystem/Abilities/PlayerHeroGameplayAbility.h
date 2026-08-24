// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "abilitysystem/Abilities/PlayerGameplayAbility.h"
#include "PlayerHeroGameplayAbility.generated.h"


class APlayerHeroCharacter;
class APlayerHeroController;
class UHeroCombatComponent;

/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UPlayerHeroGameplayAbility : public UPlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	APlayerHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	APlayerHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

private:
	TWeakObjectPtr<APlayerHeroCharacter> CachedPlayerHeroCharacter;
	TWeakObjectPtr<APlayerHeroController> CachedPlayerHeroController;
};
