/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOUR;

void setup()
{
   set_adj("woolen");
   set_armour_class(1);
   set_id("cloak");
   set_long("This is a warm woolen cloak that provides protection from the elements and some basic armor protection.");
   set_whole_body(1);
   set_race_type("humanoid");
   set_stat_mods((["dex": 1]));
   set_wearmsg("$N $vwrap the $o around $p shoulders.");
   set_removemsg("$N $vremove the $o from $p shoulders.");
} 