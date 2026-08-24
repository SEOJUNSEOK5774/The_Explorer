// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

class UPawnCombatComponent;
class UPlayerAbilitySystemComponent;

UENUM(BlueprintType)
enum class EPlayerAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 *
 */
UCLASS()
class GAME_EXPLORE_API UPlayerGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "PlayerAbility")
	EPlayerAbilityActivationPolicy AbilityActivationPolicy = EPlayerAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Player|Ability")
	UPlayerAbilitySystemComponent* GetPlayerAbilitySystemComponentFromActorInfo() const;
};
