/* Do not remove the headers from this file! see /USAGE for more info. */

inherit LIGHT_OBJECT;

int is_flame()
{
   return 1;
}

void mudlib_setup()
{
   light_object::mudlib_setup();
   add_save(({"num_decays"}));
   set_decay_auto_remove(1);
   set_id("torch");
   set_fuel(600);
}
