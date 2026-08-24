// Fill out your copyright notice in the Description page of Project Settings.


#include "abilitysystem/Abilities/PlayerHeroGameplayAbility.h"
#include "Characters/PlayerHeroCharacter.h"
#include "Controllers/PlayerHeroController.h"

APlayerHeroCharacter* UPlayerHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedPlayerHeroCharacter.IsValid())
    {
        CachedPlayerHeroCharacter = Cast<APlayerHeroCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedPlayerHeroCharacter.IsValid() ? CachedPlayerHeroCharacter.Get() : nullptr;
}

APlayerHeroController* UPlayerHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedPlayerHeroController.IsValid())
    {
        CachedPlayerHeroController = Cast<APlayerHeroController>(CurrentActorInfo->PlayerController);
    }

    return CachedPlayerHeroController.IsValid() ? CachedPlayerHeroController.Get() : nullptr;
}

UHeroCombatComponent* UPlayerHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}
