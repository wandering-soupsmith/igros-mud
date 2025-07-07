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

/*
 * Define damage resistances for this race.
 * Positive values = resistance (reduces damage)
 * Negative values = vulnerability (increases damage)
 * Values are percentages (e.g., 25 = 25% resistance)
 */
mapping query_damage_resistances()
{
   return ([
      // Squids are aquatic creatures with unique resistances
      "radiant": 0,      // No special radiant resistance
      "plasma": -15,     // Vulnerable to plasma (aquatic creature)
      "fire": -25,       // Very vulnerable to fire (aquatic creature)
      "electric": -20,   // Very vulnerable to electricity (conductive)
      "sonic": 10,       // Resistant to sonic (adapted to underwater pressure)
      "cold": 15,        // Resistant to cold (adapted to deep water)
      "psionic": 5,      // Slightly resistant to psionic (high intelligence)
      "corrosive": -10,  // Vulnerable to corrosive (soft tissue)
      "poison": 0,       // No special poison resistance
      "disease": -5,     // Slightly vulnerable to disease
      "necrotic": 0,     // No special necrotic resistance
      "drain": 0,        // No special drain resistance
      "void": 0,         // No special void resistance
      "edged": 5,        // Slippery skin provides slight edged resistance
      "piercing": -10,   // Vulnerable to piercing attacks
      "blunt": 0         // No special blunt resistance
   ]);
}

/*
 * Get resistance for a specific damage type
 */
int query_damage_resistance(string damage_type)
{
   mapping resistances = query_damage_resistances();
   return resistances[damage_type] || 0;
} 