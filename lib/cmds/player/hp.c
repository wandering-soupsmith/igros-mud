/* Do not remove the headers from this file! see /USAGE for more info. */
/* Tsath 2022 */

//: PLAYERCOMMAND
//$$ see: skills, stats, score, spells
// USAGE hp
//
// Prints your current HP, mana, fatigue, and experience status.

#include <classes.h>
#include <config.h>

inherit CMD;

private
void main(string arg)
{
   object body = this_body();

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

   // Display a single row with HP, Mana, Fatigue, and Experience
   out(sprintf("HP: %3d/%3d   Mana: %3d/%3d   Fatigue: %3d/%3d   Exp: %6d\n",
               body->query_hp(), body->query_hp_pure(),
               body->query_mana(), body->query_mana_pure(),
               body->query_fatigue(), body->query_fatigue_pure(),
               body->query_experience()));
}
