/* Do not remove the headers from this file! see /USAGE for more info. */

// mod_config.c - Module config checker. Make sure that modules are
//    defined correctly and that incompatible modules aren't defined
//    together. This module is not necessary, and if you write your own
//    adversary modules it would be easier to remove this than to maintain
//    it. HOWEVER, if you remove this module and screw up your setup,
//    don't go whining to us about it. You're on your own.
// Iizuka: Created May 7, 1998.

void check_combat_config()
{
#if DEATH_STYLE != DEATH_CORPSES && DEATH_STYLE != DEATH_VAPORIZE
   error("No valid death style set.\n");
#endif

#if HEALTH_STYLE != HEALTH_RACE_BODY
   error("Only HEALTH_RACE_BODY is supported.\n");
#endif

#if WIELD_STYLE != WIELD_LIMBS && WIELD_STYLE != WIELD_SINGLE && WIELD_STYLE != WIELD_MULTIPLE
   error("No valid wield style set.\n");
#endif

#if ARMOUR_STYLE != ARMOUR_SIMPLE_SLOTS && ARMOUR_STYLE != ARMOUR_COMPLEX_SLOTS && ARMOUR_STYLE != ARMOUR_LIMBS
   error("No valid armour style set.\n");
#endif

#if PULSE_STYLE != PULSE_NON_HEART_BEAT && PULSE_STYLE != PULSE_HEART_BEAT
   error("No valid pulse style set.\n");
#endif

#if BLOW_STYLE != BLOW_SIMPLE && BLOW_STYLE != BLOW_TYPES
   error("No valid blows module set.\n");
#endif

#if FORMULA_STYLE != FORMULA_SIMPLE && FORMULA_STYLE != FORMULA_STATS
#if FORMULA_STYLE != FORMULA_SKILLS
   error("No valid formula style set.\n");
#endif
#endif

   /* We don't really care whether an advancement module is set or
      not. It's not an essential module, and skill-based muds may not
      want to use levels and experience.
   */
}
