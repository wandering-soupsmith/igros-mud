/* Do not remove the headers from this file! see /USAGE for more info. */

// race_d.c - Daemon for providing race data to objects
// 2024: Created for race-based body system

private
mapping race_data = ([]);

void create()
{
   load_race_data();
}

void load_race_data()
{
   string *race_files;
   object race_obj;
   mapping parts;
   
   // Load standard races
   race_files = get_dir(DIR_RACES + "/*.c");
   foreach (string file in race_files)
   {
      string race_name = file[0..<3]; // remove .c
      race_obj = load_object(DIR_RACES + "/" + race_name);
      
      if (race_obj && function_exists("query_body_parts", race_obj))
      {
         parts = race_obj->query_body_parts();
         if (parts)
            race_data[race_name] = parts;
      }
   }
   
   // Load restricted races
   race_files = get_dir(DIR_RACES + "/restricted/*.c");
   foreach (string file in race_files)
   {
      string race_name = file[0..<3]; // remove .c
      race_obj = load_object(DIR_RACES + "/restricted/" + race_name);
      
      if (race_obj && function_exists("query_body_parts", race_obj))
      {
         parts = race_obj->query_body_parts();
         if (parts)
            race_data[race_name] = parts;
      }
   }
}

mapping get_race_body_parts(string race)
{
   if (!race_data[race])
      return race_data["human"] || ([]);
   return copy(race_data[race]);
}

string *list_races()
{
   return keys(race_data);
} 