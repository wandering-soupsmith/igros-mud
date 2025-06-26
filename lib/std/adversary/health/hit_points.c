/* Do not remove the headers from this file! see /USAGE for more info. */

/* Note: we calculate healing on a "need to know" basis.  This allows us
 * to pretend you've been healing ever N seconds, without wasting CPU time
 * actually incrementing something every N seconds.
 * With a large number of wounded players/monsters on a MUD, this can be
 * a huge CPU gain.
 */

inherit M_BODY_STATS;
inherit __DIR__ "diagnose_msg";

void die();
varargs int query_health(string unused);
int query_asleep();
int query_stunned();

// Forward declarations for M_BODY_STATS functions
int hurt_hp(int);
int query_hp();
int query_hp_pure();
void heal_all();
void set_hp(int hp);
void heal_hp(int healing);
void heal_all_resources();

private
nosave int health_time = time();
private
int heal_rate = 15;
private
int dead = 0;

void set_heal_rate(int x)
{
   if (x < 0)
      x = 0; /* a number of things depend upon this */

   heal_rate = x;
}

int query_heal_rate()
{
   return heal_rate;
}

//: FUNCTION set_max_health
// void set_max_health(int x);
// Set the maximum number of hit points of a monster, and also set it's
// hit points to the new max.
void set_max_health(int x)
{
   // In the new system, max health is calculated from stats
   // This function is kept for compatibility but doesn't actually set max health
   // The max health is determined by the stats system
   health_time = time(); /* old healing irrelevant */
}

//: FUNCTION kill_us
// void kill_us()
// Call this function to kill an adversary completely.
void kill_us()
{
   dead = 1;
   die();
}

//: FUNCTION set_health
// void set_health(int x);
// Set the hit points of a monster.  Usually, you want to use heal_us() or
// set_max_health instead.
// see also: set_max_health
void set_health(int x)
{
   if (x <= 0)
      kill_us();
   else
      health_time = time(); /* old healing irrelevant */
   
   set_hp(x);
}

//: FUNCTION hurt_us
// varargs int hurt_us(int x, string unused);
// Hurt us a specified amount.
varargs int hurt_us(int x, string unused)
{
   int damage_dealt;

   query_health(); /* must update healing first */
   damage_dealt = hurt_hp(x);
   if (query_health() <= 0)
      kill_us();
   return damage_dealt;
}

//: FUNCTION heal_us
// void heal_us(int x);
// Heal us a specified amount, truncating at max_health.
void heal_us(int x)
{
   /* DO NOT cause health to be updated in this routine, since this routine
    * is called from query_health().  The fact that the health isn't updated
    * doesn't matter anyway, since the effects of adding are cumulative,
    * and hitting the top has no side effects.  Only subtractions from
    * health need to make sure health is updated (since not-added-yet healing
    * may make a difference in whether you die or not).
    */
   
   heal_hp(x);
}

//: FUNCTION reincarnate
// void reincarnate();
// Makes us alive again!
void reincarnate()
{
   dead = 0;
   heal_all();
   health_time = time();
}

//: FUNCTION update_health
// Correct the health if necessary
void update_health()
{
   if (!dead && time() != health_time)
   {
      /* pretend we've been healing ... */
      heal_us(fuzzy_divide((time() - health_time) * heal_rate, 1000));
      health_time = time();
   }
}

//: FUNCTION query_health
// varargs int query_health(string unused);
// Find the current number of hitpoints of a monster
varargs int query_health(string unused)
{
   update_health();
   return query_hp();
}

//: FUNCTION query_max_health
// int query_max_health();
// Find the maximum number of hitpoints of a monster.
int query_max_health()
{
   return query_hp_pure();
}

//: FUNCTION heal_all
// void heal_all();
// Heal us completely.
void heal_all()
{
   heal_all_resources();
}

int query_ghost()
{
   return dead;
}

// Returns a simple choice of limb (target struck) for combat messages
// Overload it for specific npcs as appropriate, so you don't hit the
// hands of a bird, legs of a snake, etc
string query_random_limb()
{
   return choice(({"head", "torso", "arm", "leg"}));
}

//: FUNCTION badly_wounded
// Returns 1 if we're nearing death.
string badly_wounded()
{
   return query_health() < query_max_health() / 5 ? "badly" : 0;
}

string diagnose()
{
   string ret;

   if (query_ghost())
      return "$N $vare dead. Other than that, things are going pretty well for $n.\n";

   if (query_asleep())
      ret = "$N $vare asleep.\n";
   else if (query_stunned())
      ret = "$N $vare stunned.\n";
   else
      ret = "";

   ret += diagnose_msg(query_health() * 100 / query_max_health());
   return ret;
}
