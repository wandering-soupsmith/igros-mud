/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BODY;

/*
 * This is the name of the race.  It is used in the initial selection screen,
 * and also in who info etc.
 */
string query_race()
{
   return "squid";
}

/*
 * This is the race type for equipment compatibility.
 */
string query_race_type()
{
   return "tentacled";
}

/*
 * This is the description shown when the user types 'help race' during the
 * selection process
 */
string short_description()
{
   return "Squids are mysterious aquatic creatures with multiple tentacles. They are highly intelligent and adaptable to both land and sea environments.\n";
}

/*
 * Define the body parts for this race.
 * Each body part has:
 *   slot: 1 if it can wear equipment, 0 if not
 *   hit_weight: relative chance of being hit in combat (0 = can't be hit)
 */
mapping query_body_parts()
{
   return ([
      "tentacle 1": ([ "slot": 1, "hit_weight": 8 ]),
      "tentacle 2": ([ "slot": 1, "hit_weight": 8 ]),
      "tentacle 3": ([ "slot": 1, "hit_weight": 8 ]),
      "tentacle 4": ([ "slot": 1, "hit_weight": 8 ]),
      "lower tentacle 1": ([ "slot": 1, "hit_weight": 4 ]),
      "lower tentacle 2": ([ "slot": 1, "hit_weight": 4 ]),
      "lower tentacle 3": ([ "slot": 1, "hit_weight": 4 ]),
      "lower tentacle 4": ([ "slot": 1, "hit_weight": 4 ]),
      "lower tentacle 5": ([ "slot": 1, "hit_weight": 4 ]),
      "lower tentacle 6": ([ "slot": 1, "hit_weight": 4 ]),
      "neck": ([ "slot": 1, "hit_weight": 4 ]),
      "head": ([ "slot": 1, "hit_weight": 15 ]),
      "chest": ([ "slot": 1, "hit_weight": 25 ]),
   ]);
}

class stat_roll_mods query_roll_mods()
{
   class stat_roll_mods ret = new (class stat_roll_mods);

   /*
    * Squid stats - highly intelligent and dexterous
    */
   ret.str_adjust = 5;
   ret.str_range = 8;

   ret.dex_adjust = 8;
   ret.dex_range = 8;

   ret.con_adjust = 3;
   ret.con_range = 8;

   ret.int_adjust = 10;
   ret.int_range = 8;

   ret.wil_adjust = 5;
   ret.wil_range = 8;

   ret.cha_adjust = 2;
   ret.cha_range = 8;

   return ret;
} 