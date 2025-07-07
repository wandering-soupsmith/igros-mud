/* Do not remove the headers from this file! see /USAGE for more info. */

// stats.c - Generic formula module for basing all combat calculations
//           upon bodystats.
// Iizuka: June 3, 1998.
// 2024: Updated to use m_combat_formulas module

inherit "/std/modules/m_combat_formulas";

// Forward declarations
int query_str();
int query_dex();
int query_weapon_class();
object query_weapon();

int base_chance_to_be_hit()
{
   return 50 - query_dex();
}

int base_chance_to_hit(object target)
{
   return 50 + query_dex();
}

int base_disarm_chance(object target)
{
   return query_str() + query_dex() / 2;
}

int disarm_chance(object target)
{
   return base_disarm_chance(target) - target->base_disarm_chance(this_object());
}

//: FUNCTION calculate_damage
// int calculate_damage(object weapon, object target);
// Calculate damage using the new combat formulas module
int calculate_damage(object weapon, object target)
{
   string skill_used;
   
   if (!weapon)
   {
      skill_used = "combat/unarmed";
   }
   else
   {
      skill_used = query_weapon_skill(weapon);
   }
   
   return calculate_base_damage(this_object(), weapon, skill_used);
}

//: FUNCTION chance_to_hit
// int chance_to_hit(object weapon, object target);
// Calculate hit chance using the new combat formulas module
int chance_to_hit(object weapon, object target)
{
   return calculate_hit_chance(this_object(), target, weapon);
}

//: FUNCTION chance_to_dodge
// int chance_to_dodge(object target);
// Calculate dodge chance using the new combat formulas module
int chance_to_dodge(object target)
{
   return calculate_dodge_chance(this_object(), target);
}

//: FUNCTION chance_to_block
// int chance_to_block(object target);
// Calculate block chance using the new combat formulas module
int chance_to_block(object target)
{
   return calculate_block_chance(this_object(), target);
}
