/* Do not remove the headers from this file! see /USAGE for more info. */

// This is a simple skills module, demonstrating one way to integrate the
// Lima skills system into the combat system
// It is designed to return a number in the range 0 to 100 for chance to hit,
// based on skills in the range 0 to MAX_SKILL_VALUE

#include <classes.h>
#include <combat_modules.h>
#include <log.h>
#include <config/skills.h>

#define DAMAGE_STAT_IMPACT 5
#define DEFEND_STAT_IMPACT 5
#define DISARM_STAT_IMPACT 2
#define TO_HIT_STAT_IMPACT 5

inherit CLASS_SKILL;
inherit "/std/modules/m_combat_formulas";

private
nosave int combat_debug = 0;

int query_level();
int query_weapon_class();
int aggregate_skill(string skill);
int query_to_hit_bonus(object target);
int query_disarm_bonus(object target);
int query_damage_bonus(object target);
int query_anti_disarm(object target);
int cached_skill_rank(string s);

int level_skill_cap()
{
   return query_level() / 5 || 1;
}

void debug_combat(int cd)
{
   combat_debug = cd;
}

int should_cap_skill(string skillname)
{
   return cached_skill_rank(skillname) >= level_skill_cap();
}

int chance_to_hit(object weapon, object target)
{
   // Use new combat formulas module
   return calculate_hit_chance(this_object(), target, weapon);
}

int defend_chance(object weapon, object target)
{
   // Use new combat formulas module - combine dodge and block chances
   int dodge_chance = calculate_dodge_chance(target, this_object());
   int block_chance = calculate_block_chance(target, this_object());
   
   // Return the higher of the two (target chooses best defense)
   return dodge_chance > block_chance ? dodge_chance : block_chance;
}

int disarm_chance(object target)
{
   int defend_mod = 1;
   int res;
   object weapon = this_object()->query_weapon();
   int chance_to_hit = chance_to_hit(weapon, target);
   int defend_disarm_value = target->aggregate_skill("combat/defense/disarm") * defend_mod;
   // Calculate anti_disarm chances from the target + her weapon.
   int disarm_prevention = (target ? target->query_anti_disarm() : 0) +
                           (target->query_weapon() ? target->query_weapon()->query_anti_disarm() : 0) +
                           ((target->query_int() / DISARM_STAT_IMPACT));
   int train_limit;

   if (target->should_cap_skill("combat/defense/disarm"))
      train_limit = 1;

   // TBUG("disarm," + "combat/disarm" + "," + chance_to_hit + "\n");
   target->test_skill("combat/defense/disarm", chance_to_hit, train_limit);

   res = chance_to_hit - ((defend_disarm_value * 50) / MAX_SKILL_VALUE);

   // TBUG("Chance_to_hit: "+chance_to_hit+" Defend_disarm: "+(defend_disarm_value/100.0)+" Res: "+res+
   //" Disarm prev:"+disarm_prevention+" To hit bonus: "+(weapon ? weapon->query_to_hit_bonus(target) :
   //query_to_hit_bonus(target))+" Disarm bonus "+query_disarm_bonus(target)); Subtract to_hit bonus and add disarm
   // bonus.
   return to_int((res - (weapon ? weapon->query_to_hit_bonus(target) : query_to_hit_bonus(target)) +
                  query_disarm_bonus(target) - disarm_prevention) /
                 10);
}

int calculate_damage(object weapon, object target)
{
   // Use new combat formulas module
   string skill_used = weapon ? query_weapon_skill(weapon) : "combat/unarmed";
   return calculate_base_damage(this_object(), weapon, skill_used);
}
