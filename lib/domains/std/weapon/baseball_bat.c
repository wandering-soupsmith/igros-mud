#include <combat_modules.h>

inherit WEAPON;

void setup()
{
   set_id("baseball bat", "bat");
   set_weapon_class("club");
   set_weapon_damage_class(10);
   set_damage_type("blunt");
   set_long("A wooden baseball bat with chipped blank paint and the 'slugger' on the side.\n");
   set_anti_disarm(2);
   set_damage_bonus(-2);
   set_weight(3);
   set_salvageable((["wood":100]));
   set_must_dual_wield(1);
}
