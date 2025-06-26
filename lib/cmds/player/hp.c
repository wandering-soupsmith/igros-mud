/* Do not remove the headers from this file! see /USAGE for more info. */
/* Tsath 2022 */

//: PLAYERCOMMAND
//$$ see: skills, stats, score, spells
// USAGE hp
//
// Prints your current HP, mana, and fatigue status.

#include <classes.h>
#include <config.h>
#include <limbs.h>

inherit CMD;

#ifdef HEALTH_USES_LIMBS
inherit CLASS_LIMB;
inherit M_WIDGETS;
inherit M_FRAME;

// orders our limbs a bit, if we get more weird limbs extend this function
private
int sortLimbs(string limb)
{
   if (strsrch(limb, "head") >= 0)
      return 10;
   if (strsrch(limb, "torso") >= 0)
      return 15;
   if (strsrch(limb, "arm") >= 0)
      return 20;
   if (strsrch(limb, "leg") >= 0)
      return 25;
   // Everything else at the bottom
   return 50;
}

private
void main(string arg)
{
   int width = this_user()->query_screen_width();
   string *names;
   object body = this_body();
   mapping limbs;
   int hp_bar = width - 48;
   int mgreen;
   int mred;
   int mpercentage;
   string ansi_colour = "GREEN";
   string m_pinkfish_colour = "CYAN";
   string content = "";

   if (strlen(arg) > 0 && wizardp(this_user()))
   {
      body = present(arg, environment(this_body()));
      if (!body)
      {
         out("Cannot find '" + arg + "'.\n");
         return;
      }
      out("Status for " + capitalize(arg) + ":\n");
   }

   limbs = body->get_health();

   if (sizeof(limbs) == 0)
   {
      out("No health data found.\n");
      return;
   }

   names = sort_by_value(keys(limbs), ( : sortLimbs($1) :));

   frame_init_user();
   set_frame_title("HP");
   set_frame_header(
       sprintf("%14s %6-s %5s/%5-s %5-s %s", "Limb", "Type", "HP", "Max", "Armour", i_simplify() ? "" : "Bar"));

   foreach (string name in names)
   {
      mapping limb_data = limbs[name];
      string *parts = explode(name, "/");
      string *type = ({});
      string name2 = repeat_string("   ", sizeof(parts) - 1) + parts[ < 1];
      int ac_total = 0;
      int level = sizeof(parts);
      if (limb_data["max_health"] <= 0)
         continue;

      // In the new system, all limbs are considered vital
      type += ({"vital"});

      if (arrayp(body->query_armours(name)))
         foreach (object armour in body->query_armours(name))
         {
            if (armour)
               ac_total += armour->query_armour_class();
         }

      content += sprintf("%15s %6-s %5s/%5-s %5-s %s\n", capitalize(name),
                         (sizeof(type) ? capitalize(implode(type, ",")) : ""), "" + limb_data["health"], "" + limb_data["max_health"],
                         "" + ac_total, critical_bar(limb_data["health"], limb_data["max_health"], hp_bar));
   }
   content += sprintf("\n%15s %6-s %5s/%5-s %5-s %s\n", "Reflex", "Pool", "" + body->query_reflex(),
                      "" + body->max_reflex(), "-", green_bar(body->query_reflex(), body->max_reflex(), hp_bar));

   content += sprintf("\n%15s %6-s %8.8s    %5-s %s\n", "Intoxication", "%", "" + body->query_drunk_percent() + "%",
                      "-", reverse_critical_bar(body->query_drunk(), body->query_max_drunk(), hp_bar));

   content += sprintf("%15s %6-s %8.8s    %5-s %s\n", "System Abuse", "%", "" + body->query_abuse_percent() + "%", "-",
                      reverse_critical_bar(body->query_abuse(), body->query_max_abuse(), hp_bar));

   set_frame_content(content);
   out(frame_render());
}
#else
private
void main(string arg)
{
   object body = this_body();
   int width = this_user()->query_screen_width();
   int hp_bar = width - 48;
   string content = "";

   if (strlen(arg) > 0 && wizardp(this_user()))
   {
      body = present(arg, environment(this_body()));
      if (!body)
      {
         out("Cannot find '" + arg + "'.\n");
         return;
      }
      out("Status for " + capitalize(arg) + ":\n");
   }

   content += sprintf("%15s %6-s %5s/%5-s %5-s %s\n", "Health", "Pool", "" + body->query_health(),
                      "" + body->query_max_health(), "-", critical_bar(body->query_health(), body->query_max_health(), hp_bar));

   content += sprintf("%15s %6-s %5s/%5-s %5-s %s\n", "Reflex", "Pool", "" + body->query_reflex(),
                      "" + body->max_reflex(), "-", green_bar(body->query_reflex(), body->max_reflex(), hp_bar));

   content += sprintf("\n%15s %6-s %8.8s    %5-s %s\n", "Intoxication", "%", "" + body->query_drunk_percent() + "%",
                      "-", reverse_critical_bar(body->query_drunk(), body->query_max_drunk(), hp_bar));

   content += sprintf("%15s %6-s %8.8s    %5-s %s\n", "System Abuse", "%", "" + body->query_abuse_percent() + "%", "-",
                      reverse_critical_bar(body->query_abuse(), body->query_max_abuse(), hp_bar));

   out(content);
}
#endif
