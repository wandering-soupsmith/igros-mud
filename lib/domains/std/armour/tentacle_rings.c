/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOUR;

void setup()
{
   set_adj("set of");
   set_armour_class(2);
   set_id("rings", "tentacle rings");
   set_long("These are ornate rings designed to fit around squid tentacles. They provide minimal protection but enhance the wearer's magical abilities.");
   set_body_parts(({"tentacle 1", "tentacle 2", "tentacle 3", "tentacle 4"}));
   set_race_type("tentacled");
   set_stat_mods((["int": 1, "wil": 1]));
   set_wearmsg("$N $vslip the $o onto $p tentacles.");
   set_removemsg("$N $vremove the $o from $p tentacles.");
} 