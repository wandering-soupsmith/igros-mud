#include "../school.h";
inherit INDOOR_ROOM;

void setup(){
    set_brief("How to make a basic sword");
    set_long(@MAY

Inherit WEAPON directly.
Normal setup for adj, size and value (id not needed).
Set weapon_class to the weapon type name and weapon_damage_class to the damage value.

inherit WEAPON;

void setup() {
    set_adj("dull");
    set_weapon_class("sword");
    set_weapon_damage_class(15);
    set_damage_type("edged");
    set_size(MEDIUM);
    set_value(1000);
}

MAY
    );
    set_exits( ([
        "weaponry" : R + "weaponry",
        "forward" : R "sword02",
      ]) );
    set_objects( ([
      "/domains/std/weapon/sword" : 1,
      ]) );
}
