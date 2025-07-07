/* Do not remove the headers from this file! see /USAGE for more info. */

inherit AMMUNITION;

void setup()
{
   set_id("13mm plasma round", "ammo", "clip");
   set_weapon_class("ammo");
   set_weapon_damage_class(20);
   set_long("A clip of 13 mm plasma rounds.");
   set_combat_messages("combat-firearm");
   set_damage_type("energy");
   set_skill_used("combat/ranged/pistol");
   set_charges(6);
   set_ammo_type("13mm plasma");
   set_destruct_on_spent();
   set_value(48);
}

string short()
{
   string short = ::short(1);
   return "magazine of " + short;
}
