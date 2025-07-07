/* Do not remove the headers from this file! see /USAGE for more info. */

inherit AMMUNITION;

void setup()
{
   set_id("quarrel");
   set_adj("simple");
   set_weapon_class("ammo");
   set_weapon_damage_class(10);
   set_long("It's a simple quarrel, for use in a crossbow.");
   set_combat_messages("combat-sword");
   set_damage_type("slashing");
#ifdef USE_SKILLS
   set_skill_used("combat/ranged");
#endif
}
