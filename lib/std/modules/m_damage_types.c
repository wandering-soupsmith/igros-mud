/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** M_DAMAGE_TYPES
**
** Damage type registry and utilities for the combat system.
** Defines valid damage types and their properties for weapons and resistances.
**
** 2024: Created for new combat system with damage types and resistances
*/

// Define valid damage types and their properties
private mapping damage_types = ([
   // Energy damage types
   "radiant": ([ 
      "category": "energy", 
      "description": "Pure light and holy energy damage",
      "effect": "burns with holy light"
   ]),
   "plasma": ([ 
      "category": "energy", 
      "description": "Superheated plasma damage",
      "effect": "burns with intense heat"
   ]),
   "fire": ([ 
      "category": "energy", 
      "description": "Burning flame damage",
      "effect": "burns and scorches"
   ]),
   "electric": ([ 
      "category": "energy", 
      "description": "Electrical shock damage",
      "effect": "shocks and electrocutes"
   ]),
   "sonic": ([ 
      "category": "energy", 
      "description": "Sound wave damage",
      "effect": "vibrates and shatters"
   ]),
   "cold": ([ 
      "category": "energy", 
      "description": "Freezing ice damage",
      "effect": "freezes and chills"
   ]),
   
   // Mental damage types
   "psionic": ([ 
      "category": "mental", 
      "description": "Psychic energy damage",
      "effect": "attacks the mind"
   ]),
   
   // Chemical damage types
   "corrosive": ([ 
      "category": "chemical", 
      "description": "Acid and chemical damage",
      "effect": "corrodes and dissolves"
   ]),
   "poison": ([ 
      "category": "chemical", 
      "description": "Toxic poison damage",
      "effect": "poisons and sickens"
   ]),
   "disease": ([ 
      "category": "chemical", 
      "description": "Disease and infection damage",
      "effect": "infects and weakens"
   ]),
   
   // Dark damage types
   "necrotic": ([ 
      "category": "dark", 
      "description": "Death and decay damage",
      "effect": "decays and withers"
   ]),
   "drain": ([ 
      "category": "dark", 
      "description": "Life force draining damage",
      "effect": "drains life energy"
   ]),
   "void": ([ 
      "category": "dark", 
      "description": "Void energy damage",
      "effect": "corrodes reality"
   ]),
   
   // Physical damage types
   "edged": ([ 
      "category": "physical", 
      "description": "Sharp blade damage",
      "effect": "cuts and slashes"
   ]),
   "piercing": ([ 
      "category": "physical", 
      "description": "Penetrating puncture damage",
      "effect": "pierces and punctures"
   ]),
   "blunt": ([ 
      "category": "physical", 
      "description": "Crushing impact damage",
      "effect": "crushes and breaks"
   ])
]);

//: FUNCTION query_damage_types
// string *query_damage_types();
// Returns an array of all valid damage type names
string *query_damage_types()
{
   return keys(damage_types);
}

//: FUNCTION is_valid_damage_type
// int is_valid_damage_type(string type);
// Returns 1 if the given damage type is valid
int is_valid_damage_type(string type)
{
   return member_array(type, keys(damage_types)) >= 0;
}

//: FUNCTION query_damage_type_info
// mapping query_damage_type_info(string type);
// Returns the properties mapping for a damage type
mapping query_damage_type_info(string type)
{
   if (!is_valid_damage_type(type))
      return ([]);
   
   return copy(damage_types[type]);
}

//: FUNCTION query_damage_category
// string query_damage_category(string type);
// Returns the category of a damage type (physical, elemental, mystical)
string query_damage_category(string type)
{
   mapping info = query_damage_type_info(type);
   return info["category"] || "unknown";
}

//: FUNCTION query_damage_description
// string query_damage_description(string type);
// Returns the description of a damage type
string query_damage_description(string type)
{
   mapping info = query_damage_type_info(type);
   return info["description"] || "Unknown damage";
}

//: FUNCTION query_damage_effect
// string query_damage_effect(string type);
// Returns the effect description of a damage type
string query_damage_effect(string type)
{
   mapping info = query_damage_type_info(type);
   return info["effect"] || "damages";
}

//: FUNCTION query_damage_types_by_category
// string *query_damage_types_by_category(string category);
// Returns all damage types in a specific category
string *query_damage_types_by_category(string category)
{
   string *types = ({});
   
   foreach (string type in keys(damage_types))
   {
      if (damage_types[type]["category"] == category)
         types += ({type});
   }
   
   return types;
} 