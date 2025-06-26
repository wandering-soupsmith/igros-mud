/* Do not remove the headers from this file! see /USAGE for more info. */

inherit CLASS_EVENT_INFO;
inherit CLASS_WEAR_INFO;

int is_limb(string);
string *query_limbs();
string query_random_limb();
string *query_non_limbs();
varargs int query_max_health(string);

private
mapping armours = ([]);
private
object whole_body_item;  // New: for whole-body items like cloaks

mapping query_armour_map()
{
   return armours;
}

class wear_info find_wi(string s)
{
   class wear_info wi;

   wi = armours[s];

   // Upgrade class with secondary if needed
   if (sizeof(wi) == 2)
   {
      class wear_info wi_upgrade = new (class wear_info);
      wi_upgrade.primary = wi.primary;
      wi_upgrade.others = wi.others;
      wi = wi_upgrade;
      armours[s] = wi;
   }

   if (!wi)
   {
      if (!is_limb(s))
         return 0;
      wi = armours[s] = new (class wear_info);
   }
   return wi;
}

//: FUNCTION query_armours
// object *query_armours(string s);
// Returns the armours that are covering limb 's'.
object *query_armours(string s)
{
   class wear_info wi;
   object *armours = ({});

   if (query_max_health(s) == -1)
      return 0;

   wi = find_wi(s);
   if (!wi)
      return 0;

   armours += ({wi.primary}) + (arrayp(wi.others) ? wi.others : ({0})) + ({wi.secondary});
   armours -= ({0});
   return sizeof(armours) ? armours : 0;
}

//: FUNCTION update_cover
// Update the cover of a piece of equipment using this function should the cover change between wearing it and it being
// removed and worn again.
void update_cover(object what)
{
   mixed *also;
   class wear_info wi;

   also = what->also_covers();
   if (also)
      foreach (string limb in also)
         if (wi = find_wi(limb))
         {
            if (wi.others && member_array(what, wi.others) == -1)
               wi.others += ({what});
            else
               wi.others = ({what});
            wi.others -= ({0});
         }
}

//: FUNCTION wear_item
// nomask int wear_item(object what, string where);
// Forces the adversary to wear 'what' on its 'where' limb.
nomask int wear_item(object what, string where)
{
   class wear_info wi;
   string orig_w = where;
   mixed *also;

   // Handle whole-body items
   if (what->query_whole_body())
   {
      if (whole_body_item)
         return 0;  // Already wearing a whole-body item
      whole_body_item = what;
      return 1;
   }

   // Handle body parts
   if (what->query_body_parts() && sizeof(what->query_body_parts()))
   {
      string *parts = what->query_body_parts();
      
      // Check if all parts are available
      foreach (string part in parts)
      {
         if (!is_limb(part))
            return 0;
         wi = find_wi(part);
         if (!wi || wi.primary)
            return 0;  // Part already occupied
      }

      // Wear on all parts
      foreach (string part in parts)
      {
         wi = find_wi(part);
         wi.primary = what;
      }

      // Handle also_covers for compatibility
      also = what->also_covers();
      if (also)
         update_cover(what);

      return 1;
   }

   // Find the limb we want to use.
   if (is_limb(where))
      wi = find_wi(where);
   else
   {
      if (is_limb("right " + where))
      {
         where = "right " + where;
         wi = find_wi(where);
         // If this is a primary slot item and we're already
         // wearing something there, try left limb instead.
         if (wi && wi.primary && !what->query_worn_under())
         {
            where = "left " + orig_w;
            wi = find_wi(where);
         }
      }
   }

   // Check if this is worn under other things.
   if (what->query_worn_under())
   {
      if (!wi || wi.secondary)
         return 0;

      wi.secondary = what;
      update_cover(what);
      return 1;
   }

   if (!wi || wi.primary)
      return 0;

   wi.primary = what;

   also = what->also_covers();
   update_cover(what);
   return 1;
}

//: FUNCTION remove_item
// nomask int remove_item(object what, string where);
// Removes armour 'what' from the 'where' limb.
nomask int remove_item(object what, string where)
{
   class wear_info wi;
   string orig_w = where;
   string *also;

   // Handle whole-body items
   if (what->query_whole_body())
   {
      if (whole_body_item == what)
      {
         whole_body_item = 0;
         return 1;
      }
      return 0;
   }

   // Handle body parts
   if (what->query_body_parts() && sizeof(what->query_body_parts()))
   {
      string *parts = what->query_body_parts();
      int removed = 0;

      foreach (string part in parts)
      {
         if (wi = find_wi(part))
         {
            if (wi.primary == what)
            {
               wi.primary = 0;
               removed = 1;
            }
         }
      }

      // Clean up empty wear_info entries
      foreach (string part in parts)
      {
         if (wi = find_wi(part))
         {
            if (!wi.primary && !wi.secondary && (!wi.others || !sizeof(wi.others)))
               map_delete(armours, part);
         }
      }

      // Handle also_covers for compatibility
      also = what->also_covers();
      if (also)
         foreach (string limb in also)
            if (wi = find_wi(limb))
            {
               wi.others -= ({what});
               wi.others -= ({0});

               if (sizeof(wi.others) == 0)
               {
                  if (wi.primary == 0)
                     map_delete(armours, limb);
                  else
                     wi.others = 0;
               }
            }

      return removed;
   }

   if (!(wi = armours[where]) || (wi.primary != what && wi.secondary != what))
   {
      where = "left " + where;
      if (!(wi = armours[where]) || (wi.primary != what && wi.secondary != what))
      {
         where = "right " + orig_w;
         if (!(wi = armours[where]) || (wi.primary != what && wi.secondary != what))
         {
            return 0;
         }
      }
   }

   wi.primary = 0;
   if (wi.others == 0)
      map_delete(armours, where);

   also = what->also_covers();
   if (also)
      foreach (string limb in also)
         if (wi = find_wi(limb))
         {
            wi.others -= ({what});
            wi.others -= ({0});

            if (sizeof(wi.others) == 0)
            {
               if (wi.primary == 0)
                  map_delete(armours, limb);
               else
                  wi.others = 0;
            }
         }
   return 1;
}

//: FUNCTION has_body_slot
// int has_body_slot(string slot);
// Returns 1 if the body slot is a valid one.
int has_body_slot(string slot)
{
   return is_limb(slot);
}

//: FUNCTION query_armour_slots
// string *query_armour_slots()
// Returns all valid armour slots on an adversary.
string *query_armour_slots()
{
   return query_limbs() - query_non_limbs();
}

string query_random_armour_slot()
{
   return query_random_limb();
}

object *event_get_armours(class event_info evt)
{
   // Following ifdef added so this would update nicely always
   return query_armours(evt.target_extra);
}

//: FUNCTION query_worn_on
// object query_worn_on(string body_part);
// Returns the item worn on a specific body part.
object query_worn_on(string body_part)
{
   class wear_info wi = find_wi(body_part);
   return wi ? wi.primary : 0;
}

//: FUNCTION query_whole_body_item
// object query_whole_body_item();
// Returns the whole-body item being worn (cloak, robe, etc.)
object query_whole_body_item()
{
   return whole_body_item;
}
