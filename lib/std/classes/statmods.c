/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** statmods.c -- statistics modifiers for 6-core system
**
** These classes are used by the race system to supply modifiers for the
** initial statistics generation, and adjusting statistics.
**
** 950813, Deathblade: Created.
** 2024: Updated for new 6-core stat system
*/

/*
** The adjustment modifiers should add up to zero (some will be negative)
**
** The range values, if non-zero, specify the width of the range of the
**    random statistic.  Half is on one-side of the base value, half is
**    on the other side.  Fractions are rounded down towards a stat value
**    of zero.  For example, if the base of 20 is adjusted by +2 and the
**    range is five, then the random values will be 22 +/- 2.5, which is
**    equal to [19, 24].
*/
class stat_roll_mods
{
   int str_adjust;
   int str_range;

   int dex_adjust;
   int dex_range;

   int con_adjust;
   int con_range;

   int int_adjust;
   int int_range;

   int wil_adjust;
   int wil_range;

   int cha_adjust;
   int cha_range;
}

/*
** These values specify what percentage of the derived stat modifier
** is a bonus.  The rest of the modifier is proportional to the other
** portion of the statistic.
**
** For example, let us say "hp" is 30% racially biased.  Here are a
** couple values for a couple races:
**
** RACE A: hp_bonus = 75  (75% of the bonus is "free")
**    racial bonus = 75% of 30% = 22.5%
**    racial proportion = (25% of 30%) of X = 7.5% of X
**
**    hp statistic (less racial mod) = 20%
**      racial modifier = 7.5% * 20% = 1.5%
**      racial bonus = 22.5%
**      total = 44%
**
**    hp statistic = 50%
**      racial modifier = 7.5% * 50% = 3.5%
**      racial bonus = 22.5%
**      total = 76%
**
** RACE B: hp_bonus = 10
**    racial bonus = 10% of 30% = 3%
**    racial proportion = (90% of 30%) of X = 27% of X
**
**    hp statistic = 20%
**      racial modifier = 27% * 20% = 5.4%
**      racial bonus = 3%
**      total = 28.4%
**
**    hp statistic = 50%
**      racial modifier = 27% * 50% = 13.5%
**      racial bonus = 3%
**      total = 66.5%
**
** The point here is that when any player builds his non-racial stats up
** to 70%, their total will hit 100%.  Races with a high bonus factor
** may start out higher, but will even out in the long run.
**
** NOTE: this value is cached -- it really can't change unless a player
** changes races or something.
*/
class stat_extra_mods
{
   int hp_bonus;
   int fatigue_bonus;
   int mana_bonus;
}
