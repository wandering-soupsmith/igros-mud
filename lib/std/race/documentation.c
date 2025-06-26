/* Do not remove the headers from this file! see /USAGE for more info. */

inherit BODY;

/*
 * This is the name of the race.  It is used in the initial selection screen,
 * and also in who info etc.
 */
string query_race()
{
   return "documentation";
}

/*
 * This is the description shown when the user types 'help race' during the
 * selection process
 */
string short_description()
{
   return "This race is used to document the race system.  It shows how races work.\n"
          "\n"
          "For example, consider a race with a racial con bonus of 50.\n"
          "This means:\n"
          "\n"
          "(1) All members of the race will have at least 50 constitution.\n"
          "(2) 100 is the max for all races.\n"
          "(3) Races that start out with a bonus advance slower (since the range is\n"
          "    smaller).\n"
          "\n"
          "This race has a con bonus of 50, so all documentation race members\n"
          "will have between 50 and 100 constitution.\n";
}

/*
 * Define the body parts for this race.
 * Documentation race has the same body layout as humans.
 */
mapping query_body_parts()
{
   return ([
      "left thigh": ([ "slot": 1, "hit_weight": 6 ]),
      "left calf": ([ "slot": 1, "hit_weight": 4 ]),
      "left foot": ([ "slot": 1, "hit_weight": 2 ]),
      "right thigh": ([ "slot": 1, "hit_weight": 6 ]),
      "right calf": ([ "slot": 1, "hit_weight": 4 ]),
      "right foot": ([ "slot": 1, "hit_weight": 2 ]),
      "neck": ([ "slot": 1, "hit_weight": 4 ]),
      "head": ([ "slot": 1, "hit_weight": 8 ]),
      "chest": ([ "slot": 1, "hit_weight": 16 ]),
      "stomach": ([ "slot": 1, "hit_weight": 12 ]),
      "right shoulder": ([ "slot": 1, "hit_weight": 6 ]),
      "right arm": ([ "slot": 1, "hit_weight": 5 ]),
      "right hand": ([ "slot": 1, "hit_weight": 3 ]),
      "left shoulder": ([ "slot": 1, "hit_weight": 6 ]),
      "left arm": ([ "slot": 1, "hit_weight": 5 ]),
      "left hand": ([ "slot": 1, "hit_weight": 3 ]),
   ]);
}

/*
 * These are the bonuses for the derived statistics.  If this is zero, the
 * Normal derivation is used, however, if a bonus exists, then the stat
 * is scaled into that range.
 *
 * For example, consider a race with a racial con bonus of 50.
 *
 * The derived constitution is a weighted average of 3 base stats.  This
 * gives a number in the range [0..100].  The racial bonus says that that
 * number should be adjusted into the range [50..100].  This has three effects:
 *
 * (1) All members of the race will have at least 50 constitution.
 * (2) 100 is the max for all races.
 * (3) Races that start out with a bonus advance slower (since the range is
 *     compressed).  For example, if your base stats go up by 10, then your
 *     con would only increase by 5.
 */

int racial_con_bonus()
{
   return 0;
}

int racial_wis_bonus()
{
   return 0;
}

int racial_cha_bonus()
{
   return 90; // Documentation is a beautiful thing :-)
}

class stat_roll_mods query_roll_mods()
{
   class stat_roll_mods ret = new (class stat_roll_mods);

   /*
    * See the race stat overview in admtool, for race balance.
    */
   ret.str_adjust = 0;
   ret.str_range = 10;

   ret.dex_adjust = 0;
   ret.dex_range = 10;

   ret.con_adjust = 50;
   ret.con_range = 10;

   ret.int_adjust = 0;
   ret.int_range = 10;

   ret.wil_adjust = 0;
   ret.wil_range = 10;

   ret.cha_adjust = 0;
   ret.cha_range = 10;

   return ret;
}

/*
 * Note that this object is used as the player's body object, so you can
 * overload any player functions below.
 */
string base_in_room_desc()
{
   return ::base_in_room_desc() + " [fully documented]";
}
