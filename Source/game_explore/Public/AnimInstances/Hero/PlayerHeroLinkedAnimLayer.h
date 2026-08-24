// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/PlayerBaseAnimInstance.h"
#include "PlayerHeroLinkedAnimLayer.generated.h"

class UPlayerHeroAnimInstance;

/**
 * 
 */
UCLASS()
class GAME_EXPLORE_API UPlayerHeroLinkedAnimLayer : public UPlayerBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UPlayerHeroAnimInstance* GetHeroAnimInstance() const;
};
