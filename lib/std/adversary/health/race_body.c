/* Do not remove the headers from this file! see /USAGE for more info. */

// Race-based body system - replaces per-limb health with race-defined body parts
// 2024: New system for 6-core stats

#include <global.h>
#include <dirs.h>

inherit CLASS_EVENT_INFO;
inherit __DIR__ "diagnose_msg";
inherit M_BODY_STATS;

// Private variable declarations - must be before any function that uses them
private mapping body_parts = ([]);
private string race_name;
private nosave int health_time = time();
private int heal_rate = 15;
private int sober_rate = 30;
private int dead = 0;
private int drunk = 0; // 0: Sober
private int abuse = 0; // 0: New born
private int reflex = 0;
private int reflex_rate = 30;
private nosave int hp_cap;

// Forward declarations
void set_drunk(int d);

varargs void simple_action(string, string);
varargs void filtered_simple_action(mixed msg, function filter, mixed extra, mixed *obs);
void update_health();
void save_me();
int do_unwield(string);
int query_asleep();
int query_stunned();
int query_level();
int max_reflex();
int reflex_max();
varargs int xp_value(object);
int query_con();
varargs float query_capacity(string relation);
varargs int query_max_capacity(string relation);
int query_no_move_capacity();
int query_prone();
int message_filter(object who, mixed extra);
void set_reflex(int mp);
void set_max_health(int);
int karma_impact();
int should_cap_skill(string skillname);
varargs int test_skill(string skill, int opposing_skill, int no_learn);
int query_drunk_percent();
string drunk_diagnose();

//: FUNCTION kill_us
// Kills the adversary.
void kill_us()
{
   dead = 1;
   // Let the inheriting object handle death mechanics
   // The actual die() function should be provided by the adversary
   // that inherits this module
}

// Health system variables for the new simplified system
// private
// int current_health = 100;
// private
// int max_health = 100;

//: FUNCTION setup_race_body
// void setup_race_body(string race);
// Sets up body parts based on the given race.
void setup_race_body(string race)
{
   object race_obj;
   mapping parts;
   
   if (!race)
      return;
      
   race_name = race;
   
   // Try to load the race object directly
   race_obj = load_object(DIR_RACES + "/" + race);
   
   if (!race_obj)
   {
      // Try restricted races
      race_obj = load_object(DIR_RACES + "/restricted/" + race);
   }
   
   if (!race_obj)
   {
      // Fallback to human if race not found
      race_obj = load_object(DIR_RACES + "/human");
      race_name = "human";
   }
   
   // Get body parts from the race object
   if (race_obj)
   {
      parts = race_obj->query_body_parts();
      if (parts)
         body_parts = parts;
   }
   
   // Ensure we have at least some body parts
   if (!sizeof(body_parts))
   {
      body_parts = ([
         "torso": ([ "slot": 1, "hit_weight": 40 ]),
      ]);
   }
}

//: FUNCTION query_body_parts
// mapping query_body_parts();
// Returns the body parts mapping for this race.
mapping query_body_parts()
{
   return copy(body_parts);
}

//: FUNCTION query_race_name
// string query_race_name();
// Returns the current race name.
string query_race_name()
{
   return race_name;
}

//: FUNCTION query_race_type
// string query_race_type();
// Returns the race type for equipment compatibility.
string query_race_type()
{
   object race_obj = find_object("/std/race/" + race_name);
   return race_obj ? race_obj->query_race_type() : "unknown";
}

//: FUNCTION query_limbs
// string *query_limbs();
// Returns all body parts that can be targeted in combat.
string *query_limbs()
{
   return filter(keys(body_parts), (: body_parts[$1]["hit_weight"] > 0 :));
}

//: FUNCTION query_armour_slots
// string *query_armour_slots();
// Returns all body parts that can wear equipment.
string *query_armour_slots()
{
   return filter(keys(body_parts), (: body_parts[$1]["slot"] == 1 :));
}

//: FUNCTION query_random_limb
// string query_random_limb();
// Returns a random limb based on hit weights for combat targeting.
string query_random_limb()
{
   string *limbs = query_limbs();
   int total_weight = 0;
   int roll;
   
   if (!sizeof(limbs))
      return "torso";
      
   foreach (string limb in limbs)
   {
      total_weight += body_parts[limb]["hit_weight"];
   }
   
   if (total_weight <= 0)
      return limbs[0];
      
   roll = random(total_weight);
   
   foreach (string limb in limbs)
   {
      roll -= body_parts[limb]["hit_weight"];
      if (roll < 0)
         return limb;
   }
   
   return limbs[0];
}

//: FUNCTION is_limb
// int is_limb(string limb);
// Returns 1 if the given limb exists and can be targeted.
int is_limb(string limb)
{
   return member_array(limb, keys(body_parts)) >= 0;
}

//: FUNCTION is_armour_slot
// int is_armour_slot(string slot);
// Returns 1 if the given slot can wear equipment.
int is_armour_slot(string slot)
{
   return body_parts[slot] && body_parts[slot]["slot"] == 1;
}

//: FUNCTION query_limb_hit_weight
// int query_limb_hit_weight(string limb);
// Returns the hit weight for a given limb.
int query_limb_hit_weight(string limb)
{
   return body_parts[limb] ? body_parts[limb]["hit_weight"] : 0;
}

//: FUNCTION query_non_limbs
// string *query_non_limbs();
// Returns body parts that can't be targeted (hit_weight = 0).
string *query_non_limbs()
{
   return filter(keys(body_parts), (: body_parts[$1]["hit_weight"] == 0 :));
}

//: FUNCTION query_random_armour_slot
// string query_random_armour_slot();
// Returns a random armour slot.
string query_random_armour_slot()
{
   return query_random_limb();
}

//: FUNCTION is_vital_limb
// int is_vital_limb(string limb);
// In the new system, all limbs are considered vital since we have a single HP pool.
int is_vital_limb(string limb)
{
   return is_limb(limb);
}

//: FUNCTION is_system_limb
// int is_system_limb(string limb);
// In the new system, all limbs are considered system limbs.
int is_system_limb(string limb)
{
   return is_limb(limb);
}

//: FUNCTION query_system_limbs
// string *query_system_limbs();
// Returns all limbs as system limbs.
string *query_system_limbs()
{
   return query_limbs();
}

//: FUNCTION query_vital_limbs
// string *query_vital_limbs();
// Returns all limbs as vital limbs.
string *query_vital_limbs()
{
   return query_limbs();
}

//: FUNCTION query_wielding_limbs
// string *query_wielding_limbs();
// Returns limbs that can wield (hands).
string *query_wielding_limbs()
{
   return filter(keys(body_parts), (: member_array($1, ({"left hand", "right hand"})) >= 0 :));
}

//: FUNCTION query_attacking_limbs
// string *query_attacking_limbs();
// Returns limbs that can attack (arms and hands).
string *query_attacking_limbs()
{
   return filter(keys(body_parts), (: member_array($1, ({"left arm", "right arm", "left hand", "right hand"})) >= 0 :));
}

//: FUNCTION query_mobile_limbs
// string *query_mobile_limbs();
// Returns limbs that can move (legs).
string *query_mobile_limbs()
{
   return filter(keys(body_parts), (: member_array($1, ({"left leg", "right leg", "left foot", "right foot"})) >= 0 :));
}

//: FUNCTION query_ghost
// int query_ghost();
// Returns 1 if the adversary is dead.
int query_ghost()
{
   return dead;
}

//: FUNCTION query_health
// int query_health(string limb);
// Returns the current health (same for all limbs in new system).
varargs int query_health(string limb)
{
   // Use the M_BODY_STATS current HP instead of our own
   return query_hp();
}

//: FUNCTION query_max_health
// int query_max_health(string limb);
// Returns the max health (same for all limbs in new system).
varargs int query_max_health(string limb)
{
   // Use the M_BODY_STATS calculation instead of our own
   return query_hp_pure();
}

//: FUNCTION set_health
// void set_health(string limb, int x);
// Sets health for the entire body.
varargs void set_health(string limb, int x)
{
   if (x <= 0)
      kill_us();
   
   set_hp(x);
}

//: FUNCTION hurt_us
// int hurt_us(int x, string limb);
// Hurts the entire body.
varargs int hurt_us(int x, string limb)
{
   int damage_dealt = hurt_hp(x);
   if (query_health() <= 0)
      kill_us();
   return damage_dealt;
}

//: FUNCTION heal_us
// void heal_us(int x, string limb);
// Heals the entire body.
varargs void heal_us(int x, string limb)
{
   heal_hp(x);
}

//: FUNCTION heal_all
// void heal_all();
// Heals the entire body to full.
void heal_all()
{
   heal_all_resources();
   set_drunk(0);
   reflex = max_reflex();
}

//: FUNCTION update_health
// void update_health();
// Updates health over time.
void update_health()
{
   // In the new system, health updates are handled by M_BODY_STATS
   // This function is kept for compatibility but doesn't need to do anything
}

//: FUNCTION diagnose
// string diagnose();
// Returns a string describing the current state.
string diagnose()
{
   string ret = "";
   int health_percent = (query_health() * 100) / query_max_health();
   
   ret += "You are " + diagnose_msg(health_percent) + ".\n";
   
   if (query_drunk_percent() > 0)
      ret += "You are " + drunk_diagnose() + ".\n";
      
   return ret;
}

//: FUNCTION query_drunk
// Returns the points of drunkeness currently held by this adversary.
int query_drunk()
{
   return drunk;
}

//: FUNCTION query_max_drunk()
// The maximum level of drunk points this adversary can hold.
int query_max_drunk()
{
   return 20 + query_level() + query_con();
}

//: FUNCTION query_abuse
// Returns the abuse points held by this adversary.
int query_abuse()
{
   return abuse;
}

//: FUNCTION query_max_abuse
// Returns the maximum abuse points held by this adversary before they die permanently.
int query_max_abuse()
{
   return 1000 + (10 * query_level() + 5 * query_con());
}

//: FUNCTION query_abuse_percent
// Returns the abuse percentage.
int query_abuse_percent()
{
   return CLAMP((100 * abuse) / query_max_abuse(), 0, 100);
}

//: FUNCTION abuse_body
// Abuse this adversary for a number of points.
int abuse_body(int a)
{
   int cap_skill = should_cap_skill("misc/life/endocrines");
   if (test_skill("misc/life/endocrines", a, cap_skill))
   {
      abuse += to_int(a / 2);
   }
   else
      abuse += a;
   return 1;
}

//: FUNCTION remove_abuse
// Removes abuse points.
int remove_abuse(int a)
{
   int cap_skill = should_cap_skill("misc/life/endocrines");
   if (test_skill("misc/life/endocrines", a, cap_skill))
      abuse += to_int(a * 2);
   else
      abuse -= a;

   if (abuse < 0)
      abuse = 0;
   return 1;
}

//: FUNCTION set_drunk
// Set a drunk level at a specific point.
void set_drunk(int d)
{
   drunk = CLAMP(d, 0, query_max_drunk());
}

//: FUNCTION drink_alchohol
// Drink alchohol and add 'd' to the drunk level.
int drink_alchohol(int d)
{
   int cap_skill = should_cap_skill("misc/life/boozing");
   if (test_skill("misc/life/boozing", d, cap_skill))
   {
      drunk += to_int(d / 2);
   }
   else
      drunk += d;
}

//: FUNCTION query_drunk_percent
// Returns the drunk level in percent.
int query_drunk_percent()
{
   return (100 * drunk) / query_max_drunk();
}

//: FUNCTION drunk_diagnose
// Returns a string with a description of the drunkeness level.
string drunk_diagnose()
{
   string dd;
   switch (query_drunk_percent())
   {
   case 0..19:
      break;
   case 20..30:
      dd = "a little tipsy";
      break;
   case 31..40:
      dd = "a little drunk";
      break;
   case 41..50:
      dd = "very drunk";
      break;
   case 51..60:
      dd = "extremely drunk";
      break;
   case 61..70:
      dd = "in a drunken stupor";
      break;
   case 71..80:
      dd = "in a deep drunken stupor";
      break;
   case 81..90:
      dd = "barely awake";
      break;
   default:
      dd = "close to an alchohol coma";
      break;
   }
   return dd;
}

//: FUNCTION sober_up
// Removes a number of drunk points.
void sober_up(int s)
{
   drunk -= s;
   if (drunk < 0)
      drunk = 0;
}

//: FUNCTION can_drink
// Returns 1 if the adversary can drink.
int can_drink()
{
   return query_drunk() < query_max_drunk();
}

//: FUNCTION query_reflex
// Returns the amount of reflex currently had by the adversary.
int query_reflex()
{
   return reflex;
}

//: FUNCTION max_reflex
// Returns the max reflex based on the intelligence stat and level.
int max_reflex()
{
   return to_int((this_object()->query_int() || 1) * (1 + (query_level() / 10.0)));
}

//: FUNCTION set_reflex
// Set the reflex to an integer, but never higher than max_reflex().
void set_reflex(int mp)
{
   if (mp > max_reflex())
      mp = max_reflex();
   reflex = mp;
}

//: FUNCTION spend_reflex
// Spends reflex nomatter whether there is enough or too little.
int spend_reflex(int m)
{
   reflex -= m;
   if (reflex < 0)
   {
      reflex = 0;
      return 0;
   }
   return 1;
}

//: FUNCTION use_reflex
// Uses reflex from the reflex pool only if it's available.
int use_reflex(int m)
{
   if (reflex - m >= 0)
   {
      reflex -= m;
      return 1;
   }
   return 0;
}

//: FUNCTION restore_reflex
// Restore us a specified amount, truncating at max_reflex().
protected
void restore_reflex(int x)
{
   if (dead)
      return;

   reflex += x;
   if (reflex > max_reflex())
      reflex = max_reflex();
}

//: FUNCTION set_heal_rate
// Set the heal rate.
void set_heal_rate(int x)
{
   heal_rate = x;
}

//: FUNCTION query_heal_rate
// Returns the heal rate.
int query_heal_rate()
{
   return heal_rate;
}

//: FUNCTION reincarnate
// Brings the adversary back to life.
void reincarnate()
{
   dead = 0;
   heal_all();
   reflex = max_reflex();
   drunk = 0;
   abuse = 0;
}

//: FUNCTION can_move
// Returns 1 if the adversary can move.
int can_move()
{
   return !dead && !query_stunned() && !query_asleep();
}

//: FUNCTION disable_limb
// Disables a limb (not used in new system but kept for compatibility).
void disable_limb(string limb)
{
   // In the new system, limbs can't be disabled individually
   // This function is kept for compatibility but doesn't do anything
}

//: FUNCTION enable_limb
// Enables a limb (not used in new system but kept for compatibility).
void enable_limb(string limb)
{
   // In the new system, limbs can't be disabled individually
   // This function is kept for compatibility but doesn't do anything
}

//: FUNCTION get_health
// mapping get_health();
// Returns health mapping (simplified for new system).
mapping get_health()
{
   return ([ "torso": ([ "health": query_health(), "max_health": query_max_health() ]) ]);
}

//: FUNCTION query_worst_limb
// mixed *query_worst_limb(int vital);
// Returns worst limb info (simplified for new system).
varargs mixed *query_worst_limb(int vital)
{
   return ({ "torso", query_health(), query_max_health() });
}

//: FUNCTION badly_wounded
// string badly_wounded();
// Returns description of wound state.
string badly_wounded()
{
   int health_percent = (query_health() * 100) / query_max_health();
   
   if (health_percent < 10)
      return "You are critically wounded and near death.";
   else if (health_percent < 25)
      return "You are badly wounded.";
   else if (health_percent < 50)
      return "You are wounded.";
   else
      return "You are slightly wounded.";
}

//: FUNCTION very_wounded
// string very_wounded();
// Returns description of severe wound state.
string very_wounded()
{
   int health_percent = (query_health() * 100) / query_max_health();
   
   if (health_percent < 5)
      return "You are mortally wounded and barely alive.";
   else if (health_percent < 15)
      return "You are critically wounded.";
   else
      return "You are very badly wounded.";
} 