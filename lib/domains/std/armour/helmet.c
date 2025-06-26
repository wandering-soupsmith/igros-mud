/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOUR;

void setup()
{
   set_adj("steel");
   set_armour_class(3);
   set_id("helmet");
   set_long("This is a sturdy steel helmet that protects the head from blows.");
   set_body_parts(({"head"}));
   set_race_type("humanoid");
   set_wearmsg("$N $vput on the $o.");
   set_removemsg("$N $vremove the $o.");
} 