/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** M_COMBAT_FORMULAS
**
** Centralized combat mathematics module for the 6-core stat system.
** Provides pure math functions for hit chance, dodge, block, and damage calculations.
**
** 2024: Created for new race-based body system
*/

#include <flags.h>

// Forward declarations for functions defined later in this module
string query_weapon_stat(object weapon);
string query_weapon_skill(object weapon);
int query_weapon_scaling(object weapon);
float query_skill_factor(object weapon);

// Forward declarations for functions from inheriting objects
int query_str();
int query_dex();
int query_con();
int query_int();
int query_wil();
int query_cha();
object query_weapon();
string query_weapon_class();
int query_weapon_damage_class();
int query_skill(string skill);
int query_encumbrance();
int query_fatigue();
object query_shield();

// Helper macro for clamping values
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

//: FUNCTION query_weapon_stat
// string query_weapon_stat(object weapon);
// Get the primary stat for a weapon type (STR for melee, DEX for ranged, etc)
string query_weapon_stat(object weapon)
{
   string weapon_type;
   
   if (!weapon)
      return "str"; // Unarmed combat uses STR
   
   weapon_type = weapon->query_weapon_type() || "melee";
   
   switch (weapon_type)
   {
   case "ranged":
   case "bow":
   case "crossbow":
   case "thrown":
      return "dex";
   
   case "magical":
   case "staff":
   case "wand":
      return "int";
   
   case "melee":
   case "sword":
   case "axe":
   case "mace":
   case "dagger":
   default:
      return "str";
   }
}

//: FUNCTION query_weapon_skill
// string query_weapon_skill(object weapon);
// Get skill used for weapon (combat/melee/sword, combat/ranged/bow, etc)
string query_weapon_skill(object weapon)
{
   string weapon_type;
   string weapon_class;
   
   if (!weapon)
      return "combat/unarmed"; // Unarmed combat
   
   weapon_type = weapon->query_weapon_type() || "melee";
   weapon_class = weapon->query_weapon_class() || "sword";
   
   switch (weapon_type)
   {
   case "ranged":
      return "combat/ranged/" + weapon_class;
   
   case "magical":
      return "combat/magical/" + weapon_class;
   
   case "melee":
   default:
      return "combat/melee/" + weapon_class;
   }
}

//: FUNCTION query_weapon_scaling
// int query_weapon_scaling(object weapon);
// Get the scaling factor for stat contribution to damage
int query_weapon_scaling(object weapon)
{
   string weapon_class;
   
   if (!weapon)
      return 8; // Unarmed scaling
   
   weapon_class = weapon->query_weapon_class() || "sword";
   
   switch (weapon_class)
   {
   case "dagger":
      return 5;
   
   case "sword":
      return 5;
   
   case "axe":
      return 5;
   
   case "mace":
      return 6;
   
   case "bow":
      return 4;
   
   case "crossbow":
      return 3;
   
   default:
      return 5;
   }
}

//: FUNCTION query_skill_factor
// float query_skill_factor(object weapon);
// Get the skill factor multiplier for damage calculation
float query_skill_factor(object weapon)
{
   string weapon_class;
   
   if (!weapon)
      return 0.5; // Unarmed skill factor
   
   weapon_class = weapon->query_weapon_class() || "sword";
   
   switch (weapon_class)
   {
   case "dagger":
      return 0.7;
   
   case "sword":
      return 1.0;
   
   case "axe":
      return 1.2;
   
   case "mace":
      return 1.1;
   
   case "bow":
      return 0.8;
   
   case "crossbow":
      return 0.6;
   
   default:
      return 1.0;
   }
}

//: FUNCTION calculate_hit_chance
// int calculate_hit_chance(object attacker, object defender, object weapon);
// Calculate chance to hit (0-100)
// Considers: attacker weapon skill, defender defense skill, stat contributions
int calculate_hit_chance(object attacker, object defender, object weapon)
{
   int base_chance = 50;
   int weapon_skill;
   int defense_skill;
   int stat_bonus;
   int hit_chance;
   string weapon_skill_name;
   string primary_stat;
   
   if (!attacker || !defender)
      return base_chance;
   
   // Get weapon skill
   weapon_skill_name = query_weapon_skill(weapon);
   weapon_skill = attacker->query_skill(weapon_skill_name) || 0;
   
   // Get defense skill (dodge)
   defense_skill = attacker->query_skill("combat/defense/dodge") || 0;
   
   // Get primary stat for weapon
   primary_stat = query_weapon_stat(weapon);
   
   // Get stat bonus using specific stat functions
   switch (primary_stat)
   {
   case "str":
      stat_bonus = attacker->query_str() / 10;
      break;
   case "dex":
      stat_bonus = attacker->query_dex() / 10;
      break;
   case "con":
      stat_bonus = attacker->query_con() / 10;
      break;
   case "int":
      stat_bonus = attacker->query_int() / 10;
      break;
   case "wil":
      stat_bonus = attacker->query_wil() / 10;
      break;
   case "cha":
      stat_bonus = attacker->query_cha() / 10;
      break;
   default:
      stat_bonus = 0;
      break;
   }
   
   // Calculate hit chance
   hit_chance = base_chance + (weapon_skill * 2) - (defense_skill * 1.5) + stat_bonus;
   
   // Clamp to 5-95 range
   hit_chance = CLAMP(hit_chance, 5, 95);
   
   return hit_chance;
}

//: FUNCTION calculate_dodge_chance
// int calculate_dodge_chance(object defender, object attacker);
// Calculate dodge chance (0-100)
// Considers: DEX, dodge skill, encumbrance penalty
int calculate_dodge_chance(object defender, object attacker)
{
   int base_chance = 10;
   int dodge_skill;
   int dex_bonus;
   int encumbrance_penalty;
   int dodge_chance;
   
   if (!defender)
      return base_chance;
   
   // Get dodge skill
   dodge_skill = defender->query_skill("combat/defense/dodge") || 0;
   
   // Get DEX bonus
   dex_bonus = defender->query_dex() / 10;
   
   // Get encumbrance penalty
   encumbrance_penalty = defender->query_encumbrance() / 2;
   
   // Calculate dodge chance
   dodge_chance = base_chance + (dodge_skill * 1.5) + dex_bonus - encumbrance_penalty;
   
   // Clamp to 0-50 range (dodge shouldn't be too high)
   dodge_chance = CLAMP(dodge_chance, 0, 50);
   
   return dodge_chance;
}

//: FUNCTION calculate_block_chance
// int calculate_block_chance(object defender, object attacker);
// Calculate block chance (0-100)
// Considers: STR, block skill, shield equipped, fatigue available
int calculate_block_chance(object defender, object attacker)
{
   int base_chance = 5;
   int block_skill;
   int str_bonus;
   int shield_bonus;
   int fatigue_penalty;
   int block_chance;
   
   if (!defender)
      return base_chance;
   
   // Get block skill
   block_skill = defender->query_skill("combat/defense/block") || 0;
   
   // Get STR bonus
   str_bonus = defender->query_str() / 15;
   
   // Get shield bonus
   shield_bonus = defender->query_shield() ? 15 : 0;
   
   // Get fatigue penalty
   fatigue_penalty = (100 - defender->query_fatigue()) / 10;
   
   // Calculate block chance
   block_chance = base_chance + (block_skill * 1.2) + str_bonus + shield_bonus - fatigue_penalty;
   
   // Clamp to 0-40 range (block shouldn't be too high)
   block_chance = CLAMP(block_chance, 0, 40);
   
   return block_chance;
}

//: FUNCTION calculate_base_damage
// int calculate_base_damage(object attacker, object weapon, string skill_used);
// Calculate base damage before mitigation
// Uses new formula with weapon base, stat scaling, skill factor, class mult
int calculate_base_damage(object attacker, object weapon, string skill_used)
{
   int weapon_base;
   int stat_bonus;
   int skill_bonus;
   int subtotal;
   int final_damage;
   float class_multiplier;
   float variance;
   string primary_stat;
   int weapon_scaling;
   float skill_factor;
   int damage_class;
   
   if (!attacker)
      return 0;
   
   // Handle unarmed combat
   if (!weapon)
   {
      weapon_base = 8;
      weapon_scaling = 8;
      skill_factor = 0.5;
      damage_class = 1;
      primary_stat = "str";
   }
   else
   {
      weapon_base = weapon->query_weapon_class() || 10;
      weapon_scaling = query_weapon_scaling(weapon);
      skill_factor = query_skill_factor(weapon);
      damage_class = weapon->query_weapon_damage_class() || 1;
      primary_stat = query_weapon_stat(weapon);
   }
   
   // Get stat bonus using specific stat functions
   switch (primary_stat)
   {
   case "str":
      stat_bonus = attacker->query_str() / weapon_scaling;
      break;
   case "dex":
      stat_bonus = attacker->query_dex() / weapon_scaling;
      break;
   case "con":
      stat_bonus = attacker->query_con() / weapon_scaling;
      break;
   case "int":
      stat_bonus = attacker->query_int() / weapon_scaling;
      break;
   case "wil":
      stat_bonus = attacker->query_wil() / weapon_scaling;
      break;
   case "cha":
      stat_bonus = attacker->query_cha() / weapon_scaling;
      break;
   default:
      stat_bonus = 0;
      break;
   }
   
   // Get skill bonus
   skill_bonus = (attacker->query_skill(skill_used) || 0) * skill_factor;
   
   // Calculate subtotal
   subtotal = weapon_base + stat_bonus + skill_bonus;
   
   // Apply class multiplier
   class_multiplier = 1.0 + (damage_class - 1) * 0.25;
   
   // Apply variance (0.85 to 1.15)
   variance = 0.85 + (random(31) / 100.0);
   
   // Calculate final damage
   final_damage = (int)(subtotal * class_multiplier * variance);
   
   // Ensure minimum damage
   final_damage = final_damage > 1 ? final_damage : 1;
   
   return final_damage;
}

//: FUNCTION calculate_block_fatigue_cost
// int calculate_block_fatigue_cost(object defender);
// Calculate fatigue cost for blocking based on shield properties
int calculate_block_fatigue_cost(object defender)
{
   object shield;
   int base_cost = 5;
   int shield_cost = 0;
   
   if (!defender)
      return base_cost;
   
   shield = defender->query_shield();
   if (shield)
   {
      // Heavy shields cost more fatigue
      if (shield->query_shield_type() == "heavy")
         shield_cost = 8;
      else if (shield->query_shield_type() == "medium")
         shield_cost = 5;
      else
         shield_cost = 3; // Light shield
   }
   
   return base_cost + shield_cost;
}

//: FUNCTION calculate_armor_mitigation
// int calculate_armor_mitigation(object defender, string hit_limb, int base_damage);
// Calculate damage reduction from armor on the hit body part
int calculate_armor_mitigation(object defender, string hit_limb, int base_damage)
{
   mixed armor_pieces;
   int total_ac = 0;
   int mitigated_damage;
   
   if (!defender || !hit_limb)
      return base_damage;
   
   // Get armor worn on the specific limb
   armor_pieces = defender->query_worn_on(hit_limb);
   if (armor_pieces)
   {
      if (arrayp(armor_pieces))
      {
         foreach (object armor in armor_pieces)
         {
            if (armor && function_exists("query_ac", armor))
               total_ac += armor->query_ac();
         }
      }
      else if (objectp(armor_pieces))
      {
         object ap = (object)armor_pieces;
         if (function_exists("query_ac", ap))
            total_ac += ap->query_ac();
      }
   }
   
   // Check for whole-body items (cloaks, robes)
   armor_pieces = defender->query_worn_on("*");
   if (armor_pieces)
   {
      if (arrayp(armor_pieces))
      {
         foreach (object armor in armor_pieces)
         {
            if (armor && function_exists("query_ac", armor))
               total_ac += armor->query_ac();
         }
      }
      else if (objectp(armor_pieces))
      {
         object ap = (object)armor_pieces;
         if (function_exists("query_ac", ap))
            total_ac += ap->query_ac();
      }
   }
   
   // Apply armor reduction
   mitigated_damage = base_damage - total_ac;
   
   // Ensure minimum damage of 1
   return mitigated_damage > 1 ? mitigated_damage : 1;
}

//: FUNCTION calculate_resistance_mitigation
// int calculate_resistance_mitigation(object defender, string damage_type, int damage);
// Calculate damage reduction from race-based resistances
int calculate_resistance_mitigation(object defender, string damage_type, int damage)
{
   int resistance;
   int final_damage;
   
   if (!defender || !damage_type)
      return damage;
   
   // Get resistance from race
   resistance = defender->query_damage_resistance(damage_type) || 0;
   
   // Apply percentage-based resistance
   if (resistance != 0)
   {
      final_damage = damage * (100 - resistance) / 100;
      // Ensure minimum damage of 1
      return final_damage > 1 ? final_damage : 1;
   }
   
   return damage;
} 