/* Do not remove the headers from this file! see /USAGE for more info. */

#include <combat_config.h>

void simple_action(string msg, mixed context);
varargs mixed *action(mixed *, mixed, object, object);
void inform(mixed *, mixed, object);
varargs void filtered_inform(object *who, string *msgs, mixed others, function filter, mixed extra);
varargs void filtered_simple_action(mixed msg, function filter, mixed extra, mixed *obs);
string query_combat_message(string);
string query_damage_effect(string damage_type);
string cconfig;

void update_combat_config()
{
   cconfig = get_user_variable("cconfig");
   if (!cconfig)
      cconfig = CC_SIZE;
}

int combat_config(int flag)
{
   if (!cconfig)
      update_combat_config();
   return cconfig[flag] == 'y';
}

string damage_message(int percent)
{
   switch (percent)
   {
   case 0:
      return "!none";
   case 1:
      return "!dam1";
   case 2..3:
      return "!dam2";
   case 4..5:
      return "!dam3";
   case 6..9:
      return "!dam4";
   case 10..14:
      return "!dam5";
   case 15..24:
      return "!dam6";
   case 25..49:
      return "!dam7";
   case 50..79:
      return "!dam8";
   case 80..100:
      return "!dam9";
   default:
      return "!dam10";
   }
}

int message_filter(object who, mixed extra)
{
   // TBUG("Who: " + sprintf("%O", who) + " Extra: " + sprintf("%O", extra));
   if (extra == "miss" && who->is_body())
   {
      return !who->combat_config(CC_HIDE_MISSES);
   }
   if (extra == "none" && who->is_body())
   {
      return !who->combat_config(CC_HIDE_NO_DAMAGE);
   }
   if ((extra == "dam1" || extra == "dam2") && who->is_body())
   {
      return !who->combat_config(CC_HIDE_LOW_DAMAGE);
   }
   if (extra == "disable_limb" && who->is_body())
   {
      return !who->combat_config(CC_HIDE_DISABLE_LIMB);
   }
   if (extra == "simple_stun" && who->is_body())
   {
      return !who->combat_config(CC_HIDE_SIMPLE_STUNS);
   }
   if (extra == "dodge" && who->is_body())
   {
      return !who->combat_config(CC_HIDE_DODGES);
   }

   return 1;
}

void dodge_message()
{
   filtered_simple_action("$N $vdodge an attack.", ( : message_filter:), "dodge");
}

void handle_message(mixed mess, object target, object weapon, string limb)
{
   mixed *combat_who, messages;
   string omess = mess;

  if (arrayp(mess))
    mess=choice(mess);

   if (mess[0] == '!')
   {
      string tmp;

      if (weapon)
         tmp = weapon->query_combat_message(mess[1..]);
      else
         tmp = query_combat_message(mess[1..]);

      if (!tmp)
      {
         simple_action("$N $vare puzzled because $n $vhave no message for '" + mess[1..] + "'.", this_object());
         TBUG(mess);
         TBUG(target);
         TBUG(weapon);
         return;
      }

      mess = tmp;
   }

   combat_who = ({this_object(), target});
   if (!target)
   {
      TBUG("Missing target: this_object(): " + this_object() + " weapon: " + weapon + " limb: " + limb);
      TBUG(mess);
      return;
   }

   // Some weapons self-destruct, don't message about them.
   if (!weapon)
      return;

   // Use provided limb or get random limb from target
   if (!limb)
      limb = target->query_random_limb();

   // Generate more descriptive combat messages with body parts and damage types
   if (mess == "hit" || mess == "!hit")
   {
      string damage_type = weapon->query_damage_type() || "edged";
      string damage_effect = query_damage_effect(damage_type);
      
      // Create descriptive hit message with body part and damage type
      mess = "$N $v" + damage_effect + " $t on the " + limb + " for $o damage.";
   }
   else if (mess == "miss" || mess == "!miss")
   {
      mess = "$N $vmiss $t completely.";
   }
   else if (mess == "dodge" || mess == "!dodge")
   {
      mess = "$N $vdodge $t's attack with agile reflexes.";
   }
   else if (mess == "block" || mess == "!block")
   {
      mess = "$N $vblock $t's attack with $p shield.";
   }

   messages = action(combat_who, mess, weapon->alt_weapon() ? weapon->alt_weapon() : weapon, target->query_weapon(),
                     limb, weapon->secondary_weapon_part());
   filtered_inform(combat_who, messages, environment(), ( : message_filter:), omess[1..]);
}
