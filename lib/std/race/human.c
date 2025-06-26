/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BODY;

/*
 * This is the name of the race.  It is used in the initial selection screen,
 * and also in who info etc.
 */
string query_race()
{
   return "human";
}

/*
 * This is the description shown when the user types 'help race' during the
 * selection process
 */
string short_description()
{
   return "Humans are pretty average.  If this were a real mud, a better description would be provided (we hope).\n";
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
      "head": ([ "slot": 1, "hit_weight": 10 ]),
      "torso": ([ "slot": 1, "hit_weight": 40 ]),
      "left arm": ([ "slot": 1, "hit_weight": 15 ]),
      "right arm": ([ "slot": 1, "hit_weight": 15 ]),
      "left hand": ([ "slot": 1, "hit_weight": 5 ]),
      "right hand": ([ "slot": 1, "hit_weight": 5 ]),
      "left leg": ([ "slot": 1, "hit_weight": 10 ]),
      "right leg": ([ "slot": 1, "hit_weight": 10 ]),
      "left foot": ([ "slot": 1, "hit_weight": 0 ]), // can wear but not hit
      "right foot": ([ "slot": 1, "hit_weight": 0 ]),
   ]);
}

class stat_roll_mods query_roll_mods()
{
   class stat_roll_mods ret = new (class stat_roll_mods);

   /*
    * See the race stat overview in admtool, for race balance.
    */
   ret.str_adjust = 9;
   ret.str_range = 10;

   ret.dex_adjust = 3;
   ret.dex_range = 10;

   ret.con_adjust = 0;
   ret.con_range = 10;

   ret.int_adjust = 3;
   ret.int_range = 5;

   ret.wil_adjust = -15;
   ret.wil_range = 10;

   ret.cha_adjust = 0;
   ret.cha_range = 10;

   return ret;
}
