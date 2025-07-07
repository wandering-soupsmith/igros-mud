/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** TEST_COMBAT_FORMULAS
**
** Test file for validating the new combat formulas module.
** Tests various scenarios to ensure formulas work correctly.
**
** 2024: Created for combat formulas validation
*/

inherit OBJ;
inherit "/std/modules/m_combat_formulas";

// Forward declarations
void setup_test_objects();
void test_tier1_baseline();
void test_dodge_mechanics();
void test_weapon_scaling();
void test_stat_scaling();
void test_unarmed_combat();
void test_weapon_stat_mapping();

// Test objects
object test_attacker;
object test_defender;
object test_sword;
object test_dagger;
object test_axe;

void setup()
{
   set_adj("combat");
   set_id("test", "formula test");
   set_long("This is a test object for validating combat formulas.");
}

//: FUNCTION run_tests
// void run_tests();
// Run all combat formula tests
void run_tests()
{
   tell_user(this_user(), "=== Combat Formulas Test Suite ===\n");
   
   setup_test_objects();
   
   test_tier1_baseline();
   test_dodge_mechanics();
   test_weapon_scaling();
   test_stat_scaling();
   test_unarmed_combat();
   
   tell_user(this_user(), "=== All Tests Complete ===\n");
}

//: FUNCTION setup_test_objects
// void setup_test_objects();
// Create test objects with specific stats
void setup_test_objects()
{
   // Create test attacker (STR 50 fighter, sword skill 10)
   test_attacker = new("/domains/std/test/mock_combatant");
   test_attacker->set_stat("str", 50);
   test_attacker->set_stat("dex", 45);
   test_attacker->set_stat("con", 55);
   test_attacker->set_stat("int", 40);
   test_attacker->set_stat("wil", 35);
   test_attacker->set_stat("cha", 40);
   
   // Create test defender (DEX 60 rogue, dodge skill 15)
   test_defender = new("/domains/std/test/mock_combatant");
   test_defender->set_stat("str", 45);
   test_defender->set_stat("dex", 60);
   test_defender->set_stat("con", 50);
   test_defender->set_stat("int", 45);
   test_defender->set_stat("wil", 40);
   test_defender->set_stat("cha", 50);
   
   // Create test weapons
   test_sword = new("/std/weapon");
   test_sword->set_weapon_type("melee");
   test_sword->set_weapon_class("sword");
   test_sword->set_weapon_damage_class(20);
   test_sword->set_weapon_scaling(5);
   test_sword->set_skill_factor(1.0);
   
   test_dagger = new("/std/weapon");
   test_dagger->set_weapon_type("melee");
   test_dagger->set_weapon_class("dagger");
   test_dagger->set_weapon_damage_class(13);
   test_dagger->set_weapon_scaling(5);
   test_dagger->set_skill_factor(0.7);
   
   test_axe = new("/std/weapon");
   test_axe->set_weapon_type("melee");
   test_axe->set_weapon_class("axe");
   test_axe->set_weapon_damage_class(26);
   test_axe->set_weapon_scaling(5);
   test_axe->set_skill_factor(1.2);
}

//: FUNCTION test_tier1_baseline
// void test_tier1_baseline();
// Test Tier 1 baseline: STR 50 fighter, sword skill 10, damage class 2 sword
void test_tier1_baseline()
{
   int hit_chance;
   int damage;
   int i;
   int total_damage;
   int avg_damage;
   string skill_used;
   
   tell_user(this_user(), "\n--- Tier 1 Baseline Test ---\n");
   
   // Set up skills
   skill_used = query_weapon_skill(test_sword);
   test_attacker->set_skill(skill_used, 10);
   test_defender->set_skill("combat/defense/dodge", 10);
   
   // Test hit chance
   hit_chance = calculate_hit_chance(test_attacker, test_defender, test_sword);
   tell_user(this_user(), sprintf("Hit chance: %d%% (expected ~70%%)\n", hit_chance));
   
   // Test damage (run multiple times for variance)
   tell_user(this_user(), "Damage samples:\n");
   for (i = 0; i < 5; i++)
   {
      damage = calculate_base_damage(test_attacker, test_sword, skill_used);
      tell_user(this_user(), sprintf("  Sample %d: %d damage\n", i + 1, damage));
   }
   
   // Verify damage is in expected range (40-50)
   total_damage = 0;
   for (i = 0; i < 10; i++)
   {
      total_damage += calculate_base_damage(test_attacker, test_sword, skill_used);
   }
   avg_damage = total_damage / 10;
   tell_user(this_user(), sprintf("Average damage: %d (expected ~40-50)\n", avg_damage));
}

//: FUNCTION test_dodge_mechanics
// void test_dodge_mechanics();
// Test dodge mechanics: DEX 60 rogue, dodge skill 15
void test_dodge_mechanics()
{
   int dodge_chance;
   
   tell_user(this_user(), "\n--- Dodge Mechanics Test ---\n");
   
   // Set up dodge skill
   test_defender->set_skill("combat/defense/dodge", 15);
   
   // Test dodge chance
   dodge_chance = calculate_dodge_chance(test_defender, test_attacker);
   tell_user(this_user(), sprintf("Dodge chance: %d%% (expected ~30%%)\n", dodge_chance));
   
   // Test with encumbrance penalty
   test_defender->set_encumbrance(20);
   dodge_chance = calculate_dodge_chance(test_defender, test_attacker);
   tell_user(this_user(), sprintf("Dodge chance (encumbered): %d%% (should be lower)\n", dodge_chance));
}

//: FUNCTION test_weapon_scaling
// void test_weapon_scaling();
// Test weapon scaling: Compare dagger vs sword vs axe damage
void test_weapon_scaling()
{
   int dagger_damage;
   int sword_damage;
   int axe_damage;
   int dagger_total;
   int sword_total;
   int axe_total;
   int i;
   string dagger_skill;
   string sword_skill;
   string axe_skill;
   
   tell_user(this_user(), "\n--- Weapon Scaling Test ---\n");
   
   // Set up skills
   dagger_skill = query_weapon_skill(test_dagger);
   sword_skill = query_weapon_skill(test_sword);
   axe_skill = query_weapon_skill(test_axe);
   
   test_attacker->set_skill(dagger_skill, 10);
   test_attacker->set_skill(sword_skill, 10);
   test_attacker->set_skill(axe_skill, 10);
   
   // Calculate average damage for each weapon
   dagger_total = 0;
   sword_total = 0;
   axe_total = 0;
   
   for (i = 0; i < 10; i++)
   {
      dagger_total += calculate_base_damage(test_attacker, test_dagger, dagger_skill);
      sword_total += calculate_base_damage(test_attacker, test_sword, sword_skill);
      axe_total += calculate_base_damage(test_attacker, test_axe, axe_skill);
   }
   
   dagger_damage = dagger_total / 10;
   sword_damage = sword_total / 10;
   axe_damage = axe_total / 10;
   
   tell_user(this_user(), sprintf("Dagger average damage: %d\n", dagger_damage));
   tell_user(this_user(), sprintf("Sword average damage: %d\n", sword_damage));
   tell_user(this_user(), sprintf("Axe average damage: %d\n", axe_damage));
   
   // Verify damage progression: dagger < sword < axe
   if (dagger_damage < sword_damage && sword_damage < axe_damage)
   {
      tell_user(this_user(), "✓ Damage progression correct: dagger < sword < axe\n");
   }
   else
   {
      tell_user(this_user(), "✗ Damage progression incorrect\n");
   }
}

//: FUNCTION test_stat_scaling
// void test_stat_scaling();
// Test stat scaling: Compare different stat values
void test_stat_scaling()
{
   int low_str_damage;
   int high_str_damage;
   int i;
   string skill_used;
   
   tell_user(this_user(), "\n--- Stat Scaling Test ---\n");
   
   // Set up weapon and skill
   skill_used = query_weapon_skill(test_sword);
   test_attacker->set_skill(skill_used, 10);
   
   // Test low STR damage
   test_attacker->set_stat("str", 20);
   low_str_damage = 0;
   for (i = 0; i < 5; i++)
   {
      low_str_damage += calculate_base_damage(test_attacker, test_sword, skill_used);
   }
   low_str_damage = low_str_damage / 5;
   
   // Test high STR damage
   test_attacker->set_stat("str", 80);
   high_str_damage = 0;
   for (i = 0; i < 5; i++)
   {
      high_str_damage += calculate_base_damage(test_attacker, test_sword, skill_used);
   }
   high_str_damage = high_str_damage / 5;
   
   tell_user(this_user(), sprintf("Low STR (20) average damage: %d\n", low_str_damage));
   tell_user(this_user(), sprintf("High STR (80) average damage: %d\n", high_str_damage));
   
   if (high_str_damage > low_str_damage)
   {
      tell_user(this_user(), "✓ Stat scaling working: higher STR = higher damage\n");
   }
   else
   {
      tell_user(this_user(), "✗ Stat scaling not working correctly\n");
   }
}

//: FUNCTION test_unarmed_combat
// void test_unarmed_combat();
// Test unarmed combat calculations
void test_unarmed_combat()
{
   int unarmed_damage;
   int i;
   string skill_used;
   
   tell_user(this_user(), "\n--- Unarmed Combat Test ---\n");
   
   // Set up unarmed skill
   skill_used = "combat/unarmed";
   test_attacker->set_skill(skill_used, 10);
   
   // Test unarmed damage (no weapon)
   unarmed_damage = 0;
   for (i = 0; i < 10; i++)
   {
      unarmed_damage += calculate_base_damage(test_attacker, 0, skill_used);
   }
   unarmed_damage = unarmed_damage / 10;
   
   tell_user(this_user(), sprintf("Unarmed average damage: %d\n", unarmed_damage));
   
   if (unarmed_damage > 0)
   {
      tell_user(this_user(), "✓ Unarmed combat working\n");
   }
   else
   {
      tell_user(this_user(), "✗ Unarmed combat not working\n");
   }
}

//: FUNCTION test_weapon_stat_mapping
// void test_weapon_stat_mapping();
// Test weapon stat mapping functions
void test_weapon_stat_mapping()
{
   string stat;
   string skill;
   
   tell_user(this_user(), "\n--- Weapon Stat Mapping Test ---\n");
   
   // Test melee weapon stat mapping
   stat = query_weapon_stat(test_sword);
   skill = query_weapon_skill(test_sword);
   tell_user(this_user(), sprintf("Sword stat: %s, skill: %s\n", stat, skill));
   
   // Test ranged weapon stat mapping
   test_sword->set_weapon_type("ranged");
   stat = query_weapon_stat(test_sword);
   skill = query_weapon_skill(test_sword);
   tell_user(this_user(), sprintf("Ranged weapon stat: %s, skill: %s\n", stat, skill));
   
   // Test magical weapon stat mapping
   test_sword->set_weapon_type("magical");
   stat = query_weapon_stat(test_sword);
   skill = query_weapon_skill(test_sword);
   tell_user(this_user(), sprintf("Magical weapon stat: %s, skill: %s\n", stat, skill));
} 