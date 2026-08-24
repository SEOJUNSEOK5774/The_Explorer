// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PlayerBaseCharacter.generated.h"

class UPlayerAbilitySystemComponent;
class UPlayerAttributeSet;
class UDataAsset_StartUpDataBase;

UCLASS()
class GAME_EXPLORE_API APlayerBaseCharacter : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerBaseCharacter();


	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UPlayerAbilitySystemComponent* PlayerAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UPlayerAttributeSet* PlayerAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;

public:
	FORCEINLINE UPlayerAbilitySystemComponent* GetPlayerAbilitySystemComponent() const { return PlayerAbilitySystemComponent; }

	FORCEINLINE UPlayerAttributeSet* GetPlayerAttributeSet() const { return PlayerAttributeSet; }

};
