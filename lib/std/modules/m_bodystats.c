/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** bodystats.c -- body statistics (characteristics) for 6-core system
**
** This module should be inherited into anything requiring physical
** statistics/characteristics.  Typically, this will be used by a
** player's body, or an NPC.
**
** Stat values have no maximum cap.  Typical values:
**
**	10	Joe Human
**	20	Newbie Hero (average value at character creation time)
**	50	Mid-level experience
**	80	Master/high-level
**	90	History will remember you
**	95	Frightening
**	100+	Beyond mortal limits
**
** Statistics have a number of basic values which are set at
** character creation time.  These differentiate starting characters
** from one another.
**
**	Strength:	physical stature, power, brute force
**		Weakling vs. strong/powerful
**
**	Dexterity:	body control, speed, flexibility, coordination
**		Uncoordinated vs. excellent control
**
**	Constitution:	physical/mental stamina, sturdiness
**		Elf vs. dwarf
**
**	Intelligence:	inherent capability for learning
**		Dumb vs. smart/capable
**
**	Willpower:	drive, stamina, mental fortitude
**		Unmotivated vs. driven/fanatical
**
**	Charisma:	natural attraction, leadership, persuasion
**		Nobody vs. great leader/speaker
**
** Derived statistics are calculated from the base statistics:
**
**	HP:		Hit Points = constitution * 3 + strength * 2
**	Fatigue:	Fatigue = willpower * 3 + constitution * 2
**	Mana:		Mana = intelligence * 3 + willpower * 2
**
** 2024: Updated to new 6-core stat system
*/

#include <classes.h>
#include <config/stats.h>
#include <hooks.h>

// Stat calculation formulas
// HP = constitution * 3 + strength * 2
// Fatigue = willpower * 3 + constitution * 2  
// Mana = intelligence * 3 + willpower * 2

#define HP_CON_FACTOR     3
#define HP_STR_FACTOR     2
#define FATIGUE_WIL_FACTOR 3
#define FATIGUE_CON_FACTOR 2
#define MANA_INT_FACTOR   3
#define MANA_WIL_FACTOR   2

private
inherit "/std/classes/statmods";

mixed call_hooks(string, int);
void recompute_derived();
void refresh_stats();

private
int stat_str; /* strength */
private
int stat_dex; /* dexterity */
private
int stat_con; /* constitution */
private
int stat_int; /* intelligence */
private
int stat_wil; /* willpower */
private
int stat_cha; /* charisma */

private
int mod_str; /* strength */
private
int mod_dex; /* dexterity */
private
int mod_con; /* constitution */
private
int mod_int; /* intelligence */
private
int mod_wil; /* willpower */
private
int mod_cha; /* charisma */

private
int stat_hp; /* hit points */
private
int stat_fatigue; /* fatigue */
private
int stat_mana; /* mana */

/* Because of the complexity of the system, a bonus to any stat affects
 * the derived stats.  To avoid having to recall all the hooks for all 
 * the base stats every time one of the derived stats is queried, we 
 * cache the bonus here, reducing the number of hook calls from O(N^2) 
 * in stats to O(N).  One should make sure refresh_stats() gets called 
 * occasionally; it gets called automatically once a round during combat, 
 * but if you are using stats in a non-combat setting you might want to 
 * manually call it to make sure the latest modifications are being used.
 */
private
int cur_str;
private
int cur_dex;
private
int cur_con;
private
int cur_int;
private
int cur_wil;
private
int cur_cha;
private
int cur_hp;
private
int cur_fatigue;
private
int cur_mana;

/*
** Base statistics
**
** query_xxx_pure() returns the "pure" unadulterated statistic.  These
**   values are typically never used.
**
** query_xxx() returns the statistic, adjusted for all additional factors
**   such as temporary boosts, detriments, etc.
*/
nomask int query_str_pure()
{
   return stat_str;
}

nomask int query_dex_pure()
{
   return stat_dex;
}

nomask int query_con_pure()
{
   return stat_con;
}

nomask int query_int_pure()
{
   return stat_int;
}

nomask int query_wil_pure()
{
   return stat_wil;
}

nomask int query_cha_pure()
{
   return stat_cha;
}

nomask int query_str()
{
   return cur_str;
}

nomask int query_dex()
{
   return cur_dex;
}

nomask int query_con()
{
   return cur_con;
}

nomask int query_int()
{
   return cur_int;
}

nomask int query_wil()
{
   return cur_wil;
}

nomask int query_cha()
{
   return cur_cha;
}

void set_mod_str(int s)
{
   mod_str = s;
   recompute_derived();
}

void set_mod_dex(int s)
{
   mod_dex = s;
   recompute_derived();
}

void set_mod_con(int s)
{
   mod_con = s;
   recompute_derived();
}

void set_mod_int(int s)
{
   mod_int = s;
   recompute_derived();
}

void set_mod_wil(int s)
{
   mod_wil = s;
   recompute_derived();
}

void set_mod_cha(int s)
{
   mod_cha = s;
   recompute_derived();
}

int query_mod_str()
{
   return mod_str;
}

int query_mod_dex()
{
   return mod_dex;
}

int query_mod_con()
{
   return mod_con;
}

int query_mod_int()
{
   return mod_int;
}

int query_mod_wil()
{
   return mod_wil;
}

int query_mod_cha()
{
   return mod_cha;
}

void inc_mod_str()
{
   mod_str++;
   recompute_derived();
}

void inc_mod_dex()
{
   mod_dex++;
   recompute_derived();
}

void inc_mod_con()
{
   mod_con++;
   recompute_derived();
}

void inc_mod_int()
{
   mod_int++;
   recompute_derived();
}

void inc_mod_wil()
{
   mod_wil++;
   recompute_derived();
}

void inc_mod_cha()
{
   mod_cha++;
   recompute_derived();
}

int spare_points()
{
   int total_pts = this_object()->query_level() * STAT_PTS_PER_LEVEL;
   total_pts -= mod_str + mod_dex + mod_con + mod_int + mod_wil + mod_cha;
   return total_pts;
}

/*
** Derived statistics
*/
//: FUNCTION refresh_stats
// refresh_stats() recalculates all the stats and requery's all the bonuses.
// Combat calls this once a round.  If you are using stats in a non-combat
// setting, you might want to call this first.
void refresh_stats()
{
   //: HOOK str_bonus
   // Used to modify strength
   //: HOOK dex_bonus
   // Used to modify dexterity
   //: HOOK con_bonus
   // Used to modify constitution
   //: HOOK int_bonus
   // Used to modify intelligence
   //: HOOK wil_bonus
   // Used to modify willpower
   //: HOOK cha_bonus
   // Used to modify charisma
   int adj_str, adj_dex, adj_con, adj_int, adj_wil, adj_cha;

   // Temporarily disable hook bonuses and modifiers for testing
   cur_str = stat_str; // + mod_str + (adj_str = call_hooks("str_bonus", HOOK_SUM));
   cur_dex = stat_dex; // + mod_dex + (adj_dex = call_hooks("dex_bonus", HOOK_SUM));
   cur_con = stat_con; // + mod_con + (adj_con = call_hooks("con_bonus", HOOK_SUM));
   cur_int = stat_int; // + mod_int + (adj_int = call_hooks("int_bonus", HOOK_SUM));
   cur_wil = stat_wil; // + mod_wil + (adj_wil = call_hooks("wil_bonus", HOOK_SUM));
   cur_cha = stat_cha; // + mod_cha + (adj_cha = call_hooks("cha_bonus", HOOK_SUM));

   // Calculate derived stats with new formulas
   cur_hp = (cur_con * HP_CON_FACTOR) + (cur_str * HP_STR_FACTOR);
   cur_fatigue = (cur_wil * FATIGUE_WIL_FACTOR) + (cur_con * FATIGUE_CON_FACTOR);
   cur_mana = (cur_int * MANA_INT_FACTOR) + (cur_wil * MANA_WIL_FACTOR);
}

void recompute_derived()
{
   stat_hp = ((stat_con + mod_con) * HP_CON_FACTOR) + ((stat_str + mod_str) * HP_STR_FACTOR);
   stat_fatigue = ((stat_wil + mod_wil) * FATIGUE_WIL_FACTOR) + ((stat_con + mod_con) * FATIGUE_CON_FACTOR);
   stat_mana = ((stat_int + mod_int) * MANA_INT_FACTOR) + ((stat_wil + mod_wil) * MANA_WIL_FACTOR);
   refresh_stats();
}

nomask void set_str(int stat)
{
   stat_str = stat;
   recompute_derived();
}

nomask void set_dex(int stat)
{
   stat_dex = stat;
   recompute_derived();
}

nomask void set_con(int stat)
{
   stat_con = stat;
   recompute_derived();
}

nomask void set_int(int stat)
{
   stat_int = stat;
   recompute_derived();
}

nomask void set_wil(int stat)
{
   stat_wil = stat;
   recompute_derived();
}

nomask void set_cha(int stat)
{
   stat_cha = stat;
   recompute_derived();
}

nomask int query_hp_pure()
{
   return stat_hp;
}

nomask int query_fatigue_pure()
{
   return stat_fatigue;
}

nomask int query_mana_pure()
{
   return stat_mana;
}

nomask int query_hp()
{
   return cur_hp;
}

nomask int query_fatigue()
{
   return cur_fatigue;
}

nomask int query_mana()
{
   return cur_mana;
}

//: FUNCTION set_hp
// void set_hp(int hp);
// Set the current hit points to a specific value.
void set_hp(int hp)
{
   int max_hp = query_hp_pure();
   if (hp > max_hp)
      hp = max_hp;
   if (hp < 0)
      hp = 0;
   cur_hp = hp;
}

//: FUNCTION set_fatigue
// void set_fatigue(int fatigue);
// Set the current fatigue to a specific value.
void set_fatigue(int fatigue)
{
   int max_fatigue = query_fatigue_pure();
   if (fatigue > max_fatigue)
      fatigue = max_fatigue;
   if (fatigue < 0)
      fatigue = 0;
   cur_fatigue = fatigue;
}

//: FUNCTION set_mana
// void set_mana(int mana);
// Set the current mana to a specific value.
void set_mana(int mana)
{
   int max_mana = query_mana_pure();
   if (mana > max_mana)
      mana = max_mana;
   if (mana < 0)
      mana = 0;
   cur_mana = mana;
}

//: FUNCTION hurt_hp
// int hurt_hp(int damage);
// Reduce hit points by the specified amount. Returns the actual damage dealt.
int hurt_hp(int damage)
{
   int old_hp = cur_hp;
   cur_hp -= damage;
   if (cur_hp < 0)
      cur_hp = 0;
   return old_hp - cur_hp;
}

//: FUNCTION heal_hp
// int heal_hp(int healing);
// Increase hit points by the specified amount. Returns the actual healing done.
int heal_hp(int healing)
{
   int old_hp = cur_hp;
   int max_hp = query_hp_pure();
   cur_hp += healing;
   if (cur_hp > max_hp)
      cur_hp = max_hp;
   return cur_hp - old_hp;
}

//: FUNCTION hurt_fatigue
// int hurt_fatigue(int amount);
// Reduce fatigue by the specified amount. Returns the actual amount reduced.
int hurt_fatigue(int amount)
{
   int old_fatigue = cur_fatigue;
   cur_fatigue -= amount;
   if (cur_fatigue < 0)
      cur_fatigue = 0;
   return old_fatigue - cur_fatigue;
}

//: FUNCTION heal_fatigue
// int heal_fatigue(int amount);
// Increase fatigue by the specified amount. Returns the actual amount increased.
int heal_fatigue(int amount)
{
   int old_fatigue = cur_fatigue;
   int max_fatigue = query_fatigue_pure();
   cur_fatigue += amount;
   if (cur_fatigue > max_fatigue)
      cur_fatigue = max_fatigue;
   return cur_fatigue - old_fatigue;
}

//: FUNCTION hurt_mana
// int hurt_mana(int amount);
// Reduce mana by the specified amount. Returns the actual amount reduced.
int hurt_mana(int amount)
{
   int old_mana = cur_mana;
   cur_mana -= amount;
   if (cur_mana < 0)
      cur_mana = 0;
   return old_mana - cur_mana;
}

//: FUNCTION heal_mana
// int heal_mana(int amount);
// Increase mana by the specified amount. Returns the actual amount increased.
int heal_mana(int amount)
{
   int old_mana = cur_mana;
   int max_mana = query_mana_pure();
   cur_mana += amount;
   if (cur_mana > max_mana)
      cur_mana = max_mana;
   return cur_mana - old_mana;
}

//: FUNCTION heal_all_resources
// void heal_all_resources();
// Restore all resources (HP, fatigue, mana) to maximum.
void heal_all_resources()
{
   cur_hp = query_hp_pure();
   cur_fatigue = query_fatigue_pure();
   cur_mana = query_mana_pure();
}

/*
** Character creation stuff
**
** query_roll_mods() may be overriden by subclasses to provide for changes
**   in the statistics generation.
**
** init_stats() may only be called once (admins may call whenever)
*/
class stat_roll_mods query_roll_mods()
{
   // should be overloaded
   return new (class stat_roll_mods);
}

private
nomask int roll_stat(int adjust, int range)
{
   if (!range)
      range = DEFAULT_RANGE;

   return STAT_BASE_VALUE + adjust + random(range) - (range + 1) / 2;
}

void reset_stat_increases()
{
   mod_str = 0;
   mod_dex = 0;
   mod_con = 0;
   mod_int = 0;
   mod_wil = 0;
   mod_cha = 0;
   refresh_stats();
}

//: FUNCTION init_stats
// Rolls the stats for the first time, based on the proper racial adjustments.
// Admins can call this to reinitialize a player's stats (for example, in the
// case of abysmally horrific (near minimum) rolls).
nomask void init_stats()
{
   class stat_roll_mods mods;

   mods = query_roll_mods();
   if (mods.str_adjust + mods.dex_adjust + mods.con_adjust + 
       mods.int_adjust + mods.wil_adjust + mods.cha_adjust != 0)
      error("illegal stat adjustment values\n");

   stat_str = roll_stat(mods.str_adjust, mods.str_range);
   stat_dex = roll_stat(mods.dex_adjust, mods.dex_range);
   stat_con = roll_stat(mods.con_adjust, mods.con_range);
   stat_int = roll_stat(mods.int_adjust, mods.int_range);
   stat_wil = roll_stat(mods.wil_adjust, mods.wil_range);
   stat_cha = roll_stat(mods.cha_adjust, mods.cha_range);

   reset_stat_increases();

   recompute_derived();
   refresh_stats();
}

/*
** Object inheriting this module should be sure to call this.
*/
void internal_setup(mixed args...)
{
   // Initialize stats if not already done
   if (!stat_str && !stat_dex && !stat_con && !stat_int && !stat_wil && !stat_cha)
   {
      init_stats();
   }
}

/*
** Backward compatibility functions for old stat system
** These map the old 4-stat system to the new 6-stat system
*/
nomask int query_agi_pure()
{
   return stat_dex; // Map agility to dexterity
}

nomask int query_agi()
{
   return cur_dex; // Map agility to dexterity
}

void set_mod_agi(int s)
{
   set_mod_dex(s); // Map agility to dexterity
}

int query_mod_agi()
{
   return query_mod_dex(); // Map agility to dexterity
}

void inc_mod_agi()
{
   inc_mod_dex(); // Map agility to dexterity
}

nomask void set_agi(int stat)
{
   set_dex(stat); // Map agility to dexterity
}

/*
** Map old derived stats to new system
*/
nomask int query_wis_pure()
{
   return stat_wil; // Map wisdom to willpower
}

nomask int query_wis()
{
   return cur_wil; // Map wisdom to willpower
}

nomask int query_man_pure()
{
   return stat_mana; // Map mana to new mana
}

nomask int query_man()
{
   return cur_mana; // Map mana to new mana
}
