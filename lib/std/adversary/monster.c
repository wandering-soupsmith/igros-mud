/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Monster setup functions. Must be as small as possible
** since players carry this code as well.
*/

#include <config/skills.h>

#define MAX_MONSTER_LEVEL 100

int query_level();
void set_skill(string name, int sp);
void set_str(int);
void set_dex(int);
void set_con(int);
void set_int(int);
void set_wil(int);
void set_cha(int);
void heal_all();
void set_natural_armour(int na);
void set_damage_bonus(int x);
void add_id(string *id...);
void init_stats();
varargs int query_max_capacity(string relation);

private
nosave int max_skill;
private
nosave string monster_race;

int stats_for_Level()
{
   return 4 + (query_level() * 4);
}

void set_race(string r)
{
   add_id(r);
   monster_race = r;
}

string query_race()
{
   return monster_race;
}

// Tricky function! Basically, this scales the monsters
// skills like the player skills. They do not reach the next rank
// right before they can.
private
int skill_for_level()
{
   int below, current;
   if (max_skill)
      return max_skill;
   current = SKILL_D->pts_for_rank(query_level() / 5);
   below = SKILL_D->pts_for_rank((query_level() / 5) - 1);
   max_skill = (((0.0 + query_level()) / 5 - floor(query_level() / 5)) * (current - below)) + below;

   return max_skill || (query_level() * 10);
}

// Overwritten by BODY if a player.
varargs int query_no_move_capacity(string relation)
{
   return to_int(query_max_capacity() * 0.9);
}

void setup_monster_defaults()
{
   string *skills = SKILL_D->query_skills();
   int sp = skill_for_level();
   int level = query_level();
   
   if (this_object()->is_body())
      return;

   init_stats();
   set_natural_armour(level / 2);
   set_damage_bonus(level / 2);

   // Set 6-core stats based on level
   set_str(level);
   set_dex(level);
   set_con(level);
   set_int(level);
   set_wil(level);
   set_cha(level);

   foreach (string skill in skills)
   {
      set_skill(skill, sp);
   }
   
   // Health is now calculated from stats in the new system
   // No need to update_max_health() - it's automatic
   heal_all(); // Heal to full HP
}
