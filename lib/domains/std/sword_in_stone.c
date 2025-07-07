/* Do not remove the headers from this file! see /USAGE for more info. */

inherit WEAPON;

void setup()
{
   set_weapon_class("sword");
   set_weapon_damage_class(25);
   set_damage_type("edged");
#ifdef USE_SIZE
   set_size(MEDIUM);
#endif
#ifdef USE_MASS
   set_mass(MEDIUM);
#endif
   set_long("Well, at least you weren't chosen by a lady in a lake.\n");
}
