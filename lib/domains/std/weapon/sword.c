/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;

void setup()
{
   set_adj("dull");
   set_weapon_class("sword");
   set_weapon_damage_class(15);
   set_damage_type("edged");
#ifdef USE_SIZE
   set_size(MEDIUM);
#endif
#ifdef USE_MASS
   set_mass(MEDIUM);
#endif
   set_value(1000);
   add_combat_message("miss", "$N $vtake a clumsy swipe at $t, but only $vsucceed in making a fool of $r.");
}
