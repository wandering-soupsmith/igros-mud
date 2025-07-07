/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** MOCK_COMBATANT
**
** Mock object for testing combat formulas.
** Provides all the functions needed by the combat formulas module.
**
** 2024: Created for combat formulas testing
*/

inherit OBJ;

// Private data
private mapping stats;
private mapping skills;
private int encumbrance;
private int fatigue;
private object shield;

void mudlib_setup()
{
   ::mudlib_setup();
   add_id("mock", "combatant", "test");
   set_long("This is a mock combatant for testing combat formulas.");
   
   // Initialize default stats
   stats = (["str": 10, "dex": 10, "con": 10, "int": 10, "wil": 10, "cha": 10]);
   skills = ([]);
   encumbrance = 0;
   fatigue = 100;
   shield = 0;
}

// Stat query functions
int query_str() { return stats["str"]; }
int query_dex() { return stats["dex"]; }
int query_con() { return stats["con"]; }
int query_int() { return stats["int"]; }
int query_wil() { return stats["wil"]; }
int query_cha() { return stats["cha"]; }

// Skill and status query functions
int query_skill(string skill) { return skills[skill] || 0; }
int query_encumbrance() { return encumbrance; }
int query_fatigue() { return fatigue; }
object query_shield() { return shield; }

// Setter functions for testing
void set_stat(string stat, int value) { stats[stat] = value; }
void set_skill(string skill, int value) { skills[skill] = value; }
void set_encumbrance(int value) { encumbrance = value; }
void set_fatigue(int value) { fatigue = value; }
void set_shield(object s) { shield = s; } 