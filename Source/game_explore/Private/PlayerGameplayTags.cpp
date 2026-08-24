// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameplayTags.h"


namespace PlayerGameplayTags
{
	/** Input Tags **/

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dash, "InputTag.Dash");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipGun, "InputTag.EquipGun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipGun, "InputTag.UnequipGun");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttackGun, "InputTag.LightAttackGun");


	/** Player tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Gun, "Player.Ability.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Gun, "Player.Ability.Unequip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_LightAttack_Gun, "Player.Ability.Attack.LightAttack.Gun");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Gun, "Player.Weapon.Gun");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Gun, "Player.Event.Equip.Gun");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Gun, "Player.Event.Unequip.Gun");

}