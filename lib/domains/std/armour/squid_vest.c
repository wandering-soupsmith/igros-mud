/* Do not remove the headers from this file! see /USAGE for more info. */

inherit ARMOUR;

void setup()
{
   set_adj("flexible");
   set_armour_class(4);
   set_id("vest", "squid vest");
   set_long("This is a flexible vest designed specifically for squid anatomy. It covers the chest area and provides good protection while allowing for tentacle movement.");
   set_body_parts(({"chest"}));
   set_race_type("tentacled");
   set_stat_mods((["con": 1]));
   set_wearmsg("$N $vwrap the $o around $p chest.");
   set_removemsg("$N $vremove the $o from $p chest.");
} 