/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit WEAPON;

#if BLOWS_STYLE == BLOWS_TYPES
#define evt_data(evt) evt.data[0]
#define decrement_damage(evt) evt->data[sizeof(evt.data) - 1]--
#else /* BLOWS_STYLE == BLOWS_SIMPLE */
#define evt_data(evt) evt.data
#define decrement_damage(evt) evt.data--
#endif

class event_info source_modify_event(class event_info evt)
{
   int opp_health;
   int damage;
   int excess;

   if (evt.data == "miss" || evt.data == "disarm")
      return evt;

   opp_health = evt->target->query_health();
   damage = evt->data[sizeof(evt.data) - 1];

   if (opp_health <= damage)
   {
      excess = damage - opp_health;
      // In the race-based system, all damage is applied to the global HP pool
      for (int i = 0; i < excess; i++)
         decrement_damage(evt);
   }
   return evt;
}

void setup()
{
   set_weapon_class("sword");
   set_weapon_damage_class(12);
   set_damage_type("edged");
   set_adj("glowing");
   set_id("sword");
   add_id("mercy");
   add_adj("sword of");
   set_proper_name("the Sword of Mercy");
#ifdef USE_SIZE
   set_size(MEDIUM);
#endif
#ifdef USE_MASS
   set_mass(MEDIUM);
#endif
}
