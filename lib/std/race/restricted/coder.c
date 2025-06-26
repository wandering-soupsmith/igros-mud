/* Do not remove the headers from this file! see /USAGE for more info. */

/* Do not remove the headers from this file! see /USAGE for more info. */
// Coder race for wizards. Yaynu @ Red Dragon Nov 17 1995.
// Coder is a above average race when it comes to stats. But no crazy stats
// so wizards can test their monsters effectively.

inherit BODY;
string query_race()
{
   return "coder";
}
string short_description()
{
   return "Coder race is a misterious race of supernatural beings. You need to be a wizard in order to be a member of "
          "the coder race.";
}

/*
 * Define the body parts for this race.
 * Coders have the same body layout as humans.
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

class stat_roll_mods query_roll_mods()
{
   class stat_roll_mods ret = new (class stat_roll_mods);

   ret.str_adjust = 15;
   ret.str_range = 8;

   ret.dex_adjust = 10;
   ret.dex_range = 8;

   ret.con_adjust = 5;
   ret.con_range = 8;

   ret.int_adjust = 15;
   ret.int_range = 8;

   ret.wil_adjust = 10;
   ret.wil_range = 8;

   ret.cha_adjust = 5;
   ret.cha_range = 8;

   return ret;
}
