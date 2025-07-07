/* Do not remove the headers from this file! see /USAGE for more info. */

inherit OBJ;
inherit M_DAMAGE_SOURCE;
inherit M_WIELDABLE;
inherit M_GETTABLE;
inherit M_MESSAGES;
inherit M_SALVAGEABLE;

inherit M_DURABILITY;

#define WEAPON_LVL_PRICE 25

// Private variables for weapon properties
private
nosave string weapon_type;
private
nosave string weapon_class_name;
private
nosave int weapon_damage_class = 1;
private
nosave int weapon_scaling = 5;
private
nosave float skill_factor = 1.0;
private
nosave string damage_type = "edged";

void mudlib_setup()
{
   ::mudlib_setup();
   add_id("weapon");
   set_combat_messages("combat-bludgeon");
   set_skill_used("combat/melee/club");
   set_salvageable((["wood":40, "textile":10, "metal":50]));
   add_save(({"persist_flags", "weapon_type", "weapon_class_name", "weapon_damage_class", "weapon_scaling", "skill_factor", "damage_type"}));
}

int query_value()
{
   int value = ::query_value();
   int level = query_weapon_class() + (query_raw_hit_bonus() * 2) + query_raw_disarm_bonus() + query_raw_anti_disarm() +
               (query_must_dual_wield() ? -1 : 0) + (query_raw_damage_bonus() * 3);
   if (!value)
      value = WEAPON_LVL_PRICE * level - random(WEAPON_LVL_PRICE);
   set_value(value);
   return value;
}

nomask void set_proper_name(string str)
{
   error("Do not use set_proper_name() with weapons.");
}

mixed ob_state()
{
   return object::ob_state() + "/" + !!m_wieldable::ob_state();
}

void remove()
{
   object::remove();
   m_wieldable::remove();
}

int indirect_kill_liv_with_obj()
{
   return 1;
}

//: FUNCTION set_weapon_type
// void set_weapon_type(string type);
// Set the weapon type (melee, ranged, magical, etc.)
void set_weapon_type(string type)
{
   weapon_type = type;
}

//: FUNCTION query_weapon_type
// string query_weapon_type();
// Get the weapon type
string query_weapon_type()
{
   return weapon_type;
}

//: FUNCTION set_weapon_class
// void set_weapon_class(string class_name);
// Set the weapon class name (sword, dagger, axe, etc.)
void set_weapon_class(string class_name)
{
   weapon_class_name = class_name;
}

//: FUNCTION query_weapon_class
// string query_weapon_class();
// Get the weapon class name
string query_weapon_class()
{
   return weapon_class_name;
}

//: FUNCTION set_weapon_damage_class
// void set_weapon_damage_class(int dmg_class);
// Set the weapon damage class (1-5)
void set_weapon_damage_class(int dmg_class)
{
   weapon_damage_class = dmg_class;
}

//: FUNCTION query_weapon_damage_class
// int query_weapon_damage_class();
// Get the weapon damage class
int query_weapon_damage_class()
{
   return weapon_damage_class;
}

//: FUNCTION set_weapon_scaling
// void set_weapon_scaling(int scaling);
// Set the weapon stat scaling factor
void set_weapon_scaling(int scaling)
{
   weapon_scaling = scaling;
}

//: FUNCTION query_weapon_scaling
// int query_weapon_scaling();
// Get the weapon stat scaling factor
int query_weapon_scaling()
{
   return weapon_scaling;
}

//: FUNCTION set_skill_factor
// void set_skill_factor(float factor);
// Set the skill factor multiplier
void set_skill_factor(float factor)
{
   skill_factor = factor;
}

//: FUNCTION query_skill_factor
// float query_skill_factor();
// Get the skill factor multiplier
float query_skill_factor()
{
   return skill_factor;
}

//: FUNCTION set_damage_type
// void set_damage_type(string type);
// Set the damage type of the weapon
void set_damage_type(string type)
{
   damage_type = type;
}

//: FUNCTION query_damage_type
// string query_damage_type();
// Get the damage type of the weapon
string query_damage_type()
{
   return damage_type;
}
