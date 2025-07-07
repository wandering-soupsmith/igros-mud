/* Do not remove the headers from this file! see /USAGE for more info. */

inherit INDOOR_ROOM;

void setup()
{
   set_brief("Combat Defense Test Room");
   set_long("This is a testing chamber for the new defense and mitigation combat system. "
            "The room is equipped with various training dummies and equipment to test "
            "dodge, block, armor, and resistance mechanics.\n");
   
   set_exits(([
      "out": "/domains/std/rooms/Labroom"
   ]));
   
   add_item("dummy", "A training dummy for testing combat mechanics.");
   add_item("equipment", "Various weapons and armor for testing different damage types.");
}

void init()
{
   ::init();
   add_action("test_dodge", "test_dodge");
   add_action("test_block", "test_block");
   add_action("test_armor", "test_armor");
   add_action("test_resistance", "test_resistance");
   add_action("spawn_combatant", "spawn");
}

int test_dodge(string arg)
{
   object combatant;
   
   if (!arg)
   {
      write("Usage: test_dodge <high|low>\n");
      write("  high - Spawns a high-DEX character (should dodge more)\n");
      write("  low  - Spawns a low-DEX character (should dodge less)\n");
      return 1;
   }
   
   combatant = clone_object("/std/adversary");
   combatant->set_name("test_combatant");
   combatant->set_short("a test combatant");
   combatant->set_long("A test combatant for dodge testing.");
   
   if (arg == "high")
   {
      combatant->set_dex(80);
      combatant->set_skill("combat/defense/dodge", 50);
      write("Spawned high-DEX combatant (DEX: 80, Dodge: 50)\n");
   }
   else if (arg == "low")
   {
      combatant->set_dex(20);
      combatant->set_skill("combat/defense/dodge", 10);
      write("Spawned low-DEX combatant (DEX: 20, Dodge: 10)\n");
   }
   else
   {
      write("Invalid option. Use 'high' or 'low'.\n");
      return 1;
   }
   
   combatant->move(this_object());
   write("Test combatant spawned. Attack it to see dodge mechanics in action.\n");
   return 1;
}

int test_block(string arg)
{
   object combatant;
   object shield;
   
   if (!arg)
   {
      write("Usage: test_block <with|without>\n");
      write("  with    - Spawns a character with a shield\n");
      write("  without - Spawns a character without a shield\n");
      return 1;
   }
   
   combatant = clone_object("/std/adversary");
   combatant->set_name("block_test");
   combatant->set_short("a block test combatant");
   combatant->set_long("A test combatant for block testing.");
   combatant->set_str(60);
   combatant->set_skill("combat/defense/block", 30);
   
   if (arg == "with")
   {
      shield = clone_object("/std/weapon");
      shield->set_name("shield");
      shield->set_short("a wooden shield");
      shield->set_long("A sturdy wooden shield for blocking attacks.");
      shield->set_weapon_type("shield");
      shield->set_weapon_class("light");
      shield->set_shield_type("light");
      shield->move(combatant);
      combatant->wield(shield);
      write("Spawned combatant with shield.\n");
   }
   else if (arg == "without")
   {
      write("Spawned combatant without shield.\n");
   }
   else
   {
      write("Invalid option. Use 'with' or 'without'.\n");
      return 1;
   }
   
   combatant->move(this_object());
   write("Block test combatant spawned. Attack it to see block mechanics in action.\n");
   return 1;
}

int test_armor(string arg)
{
   object combatant;
   object armor;
   
   if (!arg)
   {
      write("Usage: test_armor <with|without>\n");
      write("  with    - Spawns a character with armor\n");
      write("  without - Spawns a character without armor\n");
      return 1;
   }
   
   combatant = clone_object("/std/adversary");
   combatant->set_name("armor_test");
   combatant->set_short("an armor test combatant");
   combatant->set_long("A test combatant for armor testing.");
   
   if (arg == "with")
   {
      armor = clone_object("/std/armour");
      armor->set_name("leather_armor");
      armor->set_short("leather armor");
      armor->set_long("A suit of leather armor that provides protection.");
      armor->set_ac(8);
      armor->set_slot("chest");
      armor->move(combatant);
      combatant->wear(armor);
      write("Spawned combatant with leather armor (AC: 8).\n");
   }
   else if (arg == "without")
   {
      write("Spawned combatant without armor.\n");
   }
   else
   {
      write("Invalid option. Use 'with' or 'without'.\n");
      return 1;
   }
   
   combatant->move(this_object());
   write("Armor test combatant spawned. Attack it to see armor mitigation in action.\n");
   return 1;
}

int test_resistance(string arg)
{
   object combatant;
   
   if (!arg)
   {
      write("Usage: test_resistance <human|squid>\n");
      write("  human - Spawns a human (no special resistances)\n");
      write("  squid - Spawns a squid (fire/electric vulnerable, cold/sonic resistant)\n");
      return 1;
   }
   
   combatant = clone_object("/std/adversary");
   combatant->set_name("resistance_test");
   combatant->set_short("a resistance test combatant");
   combatant->set_long("A test combatant for resistance testing.");
   
   if (arg == "human")
   {
      combatant->set_race("human");
      write("Spawned human combatant (no special resistances).\n");
   }
   else if (arg == "squid")
   {
      combatant->set_race("squid");
      write("Spawned squid combatant with aquatic resistances:\n");
      write("  Vulnerable: fire (-25%), electric (-20%), plasma (-15%), corrosive (-10%), piercing (-10%)\n");
      write("  Resistant: cold (+15%), sonic (+10%), psionic (+5%), edged (+5%)\n");
   }
   else
   {
      write("Invalid option. Use 'human' or 'squid'.\n");
      return 1;
   }
   
   combatant->move(this_object());
   write("Resistance test combatant spawned. Attack with different damage types to see resistance effects.\n");
   return 1;
}

int spawn_combatant(string arg)
{
   object combatant;
   
   combatant = clone_object("/std/adversary");
   combatant->set_name("test_combatant");
   combatant->set_short("a test combatant");
   combatant->set_long("A generic test combatant for combat testing.");
   combatant->move(this_object());
   
   write("Generic test combatant spawned.\n");
   return 1;
} 