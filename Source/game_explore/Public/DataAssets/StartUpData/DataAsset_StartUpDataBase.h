// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UPlayerGameplayAbility;
class UPlayerAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void GiveToAbilitySystemComponent(UPlayerAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UPlayerGameplayAbility > > ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray< TSubclassOf < UPlayerGameplayAbility > > ReactiveAbilities;

	void GrantAbilities(const TArray< TSubclassOf < UPlayerGameplayAbility > >& InAbilitiesToGive, UPlayerAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
