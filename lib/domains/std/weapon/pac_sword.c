/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_modules.h>

inherit SWORD;

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
   string *system_limbs;

   if (evt.data == "miss" || evt.data == "disarm")
      return evt;

#ifdef HEALTH_USES_LIMBS
   opp_health = evt->target->query_health(evt.target_extra);
#else
   opp_health = evt->target->query_health();
#endif
   damage = evt->data[sizeof(evt.data) - 1];

   if (opp_health <= damage)
   {
      excess = damage - opp_health;
#if HEALTH_STYLE == HEALTH_HITPOINTS
      for (int i = 0; i <= excess; i++)
         decrement_damage(evt);
#else  /* HEALTH_STYLE == HEALTH_LIMBS || HEALTH_STYLE == HEALTH_WOUNDS || HEALTH_STYLE == HEALTH_RACE_BODY */
      // In the new race-based system, all limbs are considered vital/system
      // So we apply excess damage regardless of limb type
      for (int i = 0; i < excess; i++)
         decrement_damage(evt);
#endif /* HEALTH_STYLE */
   }
   return evt;
}

void setup()
{
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
