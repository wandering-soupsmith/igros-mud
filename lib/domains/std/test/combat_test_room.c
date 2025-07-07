/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** COMBAT_TEST_ROOM
**
** Test room for validating the new combat system with race-based bodies.
** Contains a test monster to fight.
**
** 2024: Created for combat system testing
*/

inherit INDOOR_ROOM;

void setup()
{
   set_area("wiz_area");
   set_brief("Combat Test Room");
   set_long("This is a simple test room for validating the new combat system. "
            "A test monster stands here, ready for combat. The room is well-lit "
            "and has plenty of space for fighting.\n");

   add_exit("south", "/domains/std/lima/directory");
   
   // Add a test monster
   set_objects((["/domains/std/monster/goblin":1]));
} 