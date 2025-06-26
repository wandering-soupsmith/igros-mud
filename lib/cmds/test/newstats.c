/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** newstats.c -- Test command for the new 6-core stat system
**
** This command allows testing of the new stat and health systems
** from the command line.
**
** 2024: Created for testing new stat system
*/

#include <classes.h>

inherit CMD;

private
void main(string arg)
{
   object body = this_body();
   
   if (!body || !body->test_stats)
   {
      write("This command requires a body with the new stat system.\n");
      return;
   }
   
   if (!arg || arg == "")
   {
      body->test_stats();
      return;
   }
   
   string *parts = explode(arg, " ");
   
   if (sizeof(parts) < 2)
   {
      write("Usage: newstats [stats|modify <stat> <amount>|health <type> <amount>]\n");
      write("Examples:\n");
      write("  newstats stats                    - Show current stats\n");
      write("  newstats modify str 5             - Increase STR by 5\n");
      write("  newstats health hurt 10           - Take 10 damage\n");
      write("  newstats health heal 20           - Heal 20 HP\n");
      write("  newstats health fatigue 5         - Fatigue by 5\n");
      write("  newstats health drain_mana 10     - Drain 10 mana\n");
      write("  newstats health heal_all          - Full heal\n");
      return;
   }
   
   switch(parts[0])
   {
      case "stats":
         body->test_stats();
         break;
         
      case "modify":
         if (sizeof(parts) < 3)
         {
            write("Usage: newstats modify <stat> <amount>\n");
            write("Valid stats: str, dex, con, int, wil, cha\n");
            return;
         }
         body->test_modify(parts[1], to_int(parts[2]));
         break;
         
      case "health":
         if (sizeof(parts) < 2)
         {
            write("Usage: newstats health <type> [amount]\n");
            write("Valid types: hurt, heal, fatigue, restore_fatigue, drain_mana, restore_mana, heal_all\n");
            return;
         }
         if (parts[1] == "heal_all")
         {
            body->test_health(parts[1], 0);
         }
         else if (sizeof(parts) < 3)
         {
            write("Usage: newstats health <type> <amount>\n");
            return;
         }
         else
         {
            body->test_health(parts[1], to_int(parts[2]));
         }
         break;
         
      default:
         write("Unknown subcommand. Use 'newstats' for help.\n");
         break;
   }
} 