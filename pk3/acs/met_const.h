#define METROID_SV_CVAR_VERSION    2
#define METROID_CL_CVAR_VERSION    2

#define METROID_BASE                         354

#define METROID_SPACEJUMP_ENTER2             351
#define METROID_SPACEJUMP_ENTER              352
#define METROID_OPEN                         353

#define METROID_WALLBOUNCE                   364
#define METROID_REQUESTDODGE                 368

#define METROID_TURN                         385

#define METROID_POWERBOMB                    454
#define METROID_LONG_XYZ                     455
#define METROID_LONG_BURST                   456
#define METROID_LONG_TRAIL                   459
#define METROID_LONG_COORDSHIT               468

#define METROID_OPEN_CLIENT                  551
#define METROID_CHANGELOG                    583
#define METROID_SHIPCONTROLS                 584
#define METROID_PUKE                         585
#define METROID_SPEEDBOOSTER                 586
#define METROID_MORPHCAMERA                  587
#define METROID_MORPHBALL                    588
#define METROID_UNLOADING                    589
#define METROID_DEATH                        590
#define METROID_DISCONNECT                   591
#define METROID_ENTER                        592
#define METROID_RESPAWN                      593
#define METROID_DECORATE                     594
#define METROID_BOOSTBALL                    595
#define METROID_DECORATECLIENT               596
#define METROID_BWEEBWEEBWEEBWEE             597
#define METROID_BWEEBWEEMORPH                598
#define METROID_ENTER_CLIENTSIDE             599

#define CLOCKWISE           0
#define COUNTERCLOCKWISE    1

#define DASH_VEL    40

#define WB_XYBASE   35.0
#define WB_ZBASE    1.25

#define WB_DODGE    0
#define WB_KICK     1
#define WB_KICKUP   2

#define WD_FORWARD  1
#define WD_FORWRITE 2
#define WD_RIGHT    3
#define WD_BACKRITE 4
#define WD_BACK     5
#define WD_BACKLEFT 6
#define WD_LEFT     7
#define WD_FORWLEFT 8
#define WD_KICK     9

int AngleOffsets[10] = { 0.5, 0.5, 0.375, 0.25, 0.125, 0, 0.875, 0.75, 0.625, 0 };

#define LUNGE_FORWARD 20.0
#define LUNGE_ZMULT   0.8

#define MJUMP_DELAY     8

#define TIMER_COUNT     7

#define TIMER_CFORWARD   0
#define TIMER_CRIGHT     1
#define TIMER_CBACK      2
#define TIMER_CLEFT      3
#define TIMER_BOUNCED    4
#define TIMER_DIDDODGE   5
#define TIMER_HBACK      6

#define TRAILCOUNT 3

#define PICKUPTYPES 14

int GotBigPickup[PLAYERMAX][PICKUPTYPES];

int BigPickupItems[PICKUPTYPES] = 
{
    "SuperMissileAcquired",     // 0
    "MissileTankAcquired",
    "PowerBombAcquired",
    "EnergyTankAcquired",
    "SpeedBoosterAcquired",
    "SpaceJumpAcquired",        // 5

    "SpazerBeamAcquired",
    "PlasmaBeamAcquired",
    "WaveBeamAcquired",
    "IceBeamAcquired",
    "LongBeamAcquired",         // 10

    "ChargeComboAcquired",
    "DoomCannonAcquired",
    "ChromaStormAcquired",
};

int BigPickupMsgFonts[PICKUPTYPES] =
{
    "BNRSTANK",
    "BNRMTANK",
    "BNRPTANK",
    "BNRETANK",
    "BNRSBOOS",
    "BNRSJUMP",

    "BNRSPBEM",
    "BNRPLBEM",
    "BNRWVBEM",
    "BNRICBEM",
    "BNRLNBEM",

    "BNRECONV",
    "BNRUNOWN",
    "BNRCHRST",
};

int BigPickupSounds[PICKUPTYPES][3] =
{
    {"M_ITMGET", "GrossSoundHack1", "arsenal/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "arsenal/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "arsenal/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "nrgtank/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "movement/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "movement/get"}, 

    {"M_ITMGET", "GrossSoundHack1", "weapon/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "weapon/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "weapon/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "weapon/get"}, 
    {"M_ITMGET", "GrossSoundHack1", "weapon/get"}, 

    {"M_ITMGE2", "GrossSoundHack2", "charge/get"}, 
    {"M_ITMGE3", "GrossSoundHack3", "cannon/get"}, 
    {"M_ITMGE4", "GrossSoundHack4", "chroma/get"}, 
};

int BigPickupMessages[PICKUPTYPES] = 
{
    "SUPER MISSILE TANK: ACQUIRED",
    "MISSILE TANK: ACQUIRED",
    "POWER BOMB TANK: ACQUIRED",
    "ENERGY TANK: ACQUIRED",
    "SPEED BOOSTER: ACQUIRED",
    "SPACE JUMP: ACQUIRED",

    "SPAZER BEAM: ACQUIRED",
    "PLASMA BEAM: ACQUIRED",
    "WAVE BEAM: ACQUIRED",
    "ICE BEAM: ACQUIRED",
    "LONG BEAM: ACQUIRED",

    "ENERGY CONVERTER UNIT: ACQUIRED",
    "\cjdX000FFFsF \cferror",
    "CHROMA STORM: ACQUIRED",
};

#define PARTICLECOUNT 4

int DakkaBeamParticles[PARTICLECOUNT] =
{
    "DakkaBeamTrail",
    "DakkaBeamParticle",
    "DakkaBeamComboTrail",
    "DakkaBeamComboParticle",
};

int DakkaBeamFadeRates[PARTICLECOUNT] = 
{
    0.01,
    0.00,
    0.01,
    0.00,
};

#define CHARGELEVELS 19
int DakkaBeamFireTimes[CHARGELEVELS] = 
{
    9, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 3, 2,
};

// LASERS MOTHERFUCKER

#define LASER_ROTATETICS 72
#define LASER_NOEFFECTS_FADELENGTH 192
#define LASER_BASEALPHA 0.5

#define COLORCOUNT 3

int LaserStarts[COLORCOUNT] =
{
    "DBeamLaserPuff_Red",
    "DBeamLaserPuff_Green",
    "DBeamLaserPuff_Blue",
};

int LaserMids[COLORCOUNT] =
{
    "DBeamLaserParticle_Red",
    "DBeamLaserParticle_Green",
    "DBeamLaserParticle_Blue",
};

int LaserEnds[COLORCOUNT] =
{
    "DBeamLaserDeath_Red",
    "DBeamLaserDeath_Green",
    "DBeamLaserDeath_Blue",
};

int ChangelogString =
"From alpha-1.777b to v1:\n\
==========================\n\
GAMEPLAY:\n\
- Metroid: Dreadnought is now playable in ZDoom instead of just Zandronum!\n\
- The Morph Ball no longer allows the player to, well, morph when the flag is held.\n\
- Speed Booster has a much longer delay after stopping before restarting.\n\
- Plasma Combo does slightly more damage.\n\
- Long Combo speeds up by 10% every tic and has double the explosive radius.\n\
- Plasma and Ice Uncharged and Charged have had their radius and height slightly reduced.\n\
- Wall jump checks no longer get hung-up on lines that block monsters.\n\
- After Boost Balling in mid-air, it is no longer possible to boost ball again until you have hit the ground.\n\
- The Power Bomb's damage arc has been reversed--instead of dealing most of its damage in the center and weakening as it spreads out, it deals most of its damage the farther out it spreads. Due to the sheer amount of actors clustered up in the center, this is barely a noticable damage nerf for the Power Bomb. All it means is that one-shotting Cybers is a bit more difficult, and that enemies two feet out no longer get tickled.\n\
- The Missiles' rate of fire has been shortened by eight tics.\n\
- The Speed Booster now replaces the Partial Invisibility sphere rather than the Radsuit.\n\
- An optional Metroid-style system has been implemented--on picking up a major item, the world freezes, a banner pops up, and the fanfare plays. For those who want nothing to do with this, hit the Use button to break out, or type 'metroid_cl_nometroidpickups 1' in the console.\n\
- Picking up the Space Jump will give the Morph Ball the ability to jump while morphed. This does not work for metroid_spacejump 1.\n\
- The Doom Cannon now has its Charge Combo.\n\
- The Ice Beam's blast radius in co-op is expanded, allowing the player to catch more enemies with one shot. The Ice Beam's blast radius in PvP is ever-so-slightly expanded.\n\
- All Charged beams aside from Ice have had their damage dramatically increased in co-op, to make charging up more viable.\n\
- metroid_loaded has changed from activated-whenever to activated-on-first-spawn, and has been expanded on--1 gives you all weapons, 2 gives you more missiles, 3 gives you even more missiles, 4 gives you all weapons and more missiles, 5 gives you weapons/missiles/super missiles/power bombs, 6 gives you weapons/missiles/super missiles/power bombs/charge combo/speed booster.\n\
- Skulltag weapons now have their replacers, handled by metroid_skulltagweapons. 1 to spawn unique weapons, 2 to spawn normal weapons, 0 to not spawn anything at all.\n\
- New weapon designed by ijon tichy has been implemented; the Chroma Storm, a rainbow herald of pure dakka!\n\
- Being chilled now reduces your jump height as well.\n\
- Charging up for Boost Ball now gives you a 2x weakness to everything, due to diverting energy to motion rather than shields.\n\
- The center bolt of the Spazer is slightly more powerful than the outer bolts.\n\
- Samus now recoils back slightly on being hit, like in Metroid.\n\
- If a player picks up an Energy Converter without having picked up a missile tank before, one will be provided for free.\n\
- metroid_dropgravity now controls whether items dropped from enemies have gravity or not.\n\
- Mook enemies no longer have a chance to drop Power Bombs on death.\n\
- Energy Armor now only protects for 125 damage instead of 150 damage.\n\
\n\
BUGFIXES:\n\
- Player death no longer freezes the life bar.\n\
- Items should no longer become invisible when metroid_dropstay is on.\n\
- Charging up the beams while Time Freeze is active no longer causes a massive amount of frozen trails.\n\
- Put DoomEd numbers on all relevant actors, as per mapper request.\n\
- The Speed Booster script no longer overflows with copious respawning.\n\
- The Ice Combo now checks for ten missiles instead of five.\n\
- The Bomb ammo regeneration has been changed to be work via ACS rather than Decorate, which means that Spread should no longer give infinite bombs and Time Freeze should no longer stop bombs.\n\
- MaxArmorBonus and MaxHealthBonus now no longer appear.\n\
- Floatbob removed from items to keep them from floating into space.\n\
- Cacodemon and Lost Soul no longer momentarily turn into a zombieman on freeze.\n\
- Morph Ball can no longer be triggered on death to leave a dummy Samus.\n\
- Plasma combo can no longer activate doors or lines, and can no longer teleport.\n\
- Morph Ball bombs can no longer teleport.\n\
- Samus' freeze sprite is no longer invisible.\n\
- Frozen Samus is no longer breakable via ice explosions.\n\
- Morph Ball is now freezable.\n\
- Energy armor is no longer constantly pickuppable as long as your bar is entirely blue.\n\
- The morph ball should no longer have troubles with the player's translation.\n\
- In rare instances, people were starting the game with infinite Power Bombs from the get-go. This has been fixed.\n\
\n\
POLISH:\n\
- Implemented changelog. Neat.\n\
- GLDefs! Fuckin' everywhere! Thanks to PresidentPeople for help.\n\
- 15/30/100 Energy now have different pickup sprites.\n\
- ACLO E sprite has been replaced with NOPE A.\n\
- Rephrased a few lines in the difficulty settings for more clarification.\n\
- By modder request, the HUD is now based upon whether or not you have PowerBeamAcquired in your inventory.\n\
- Dying now properly lowers your arm cannon on death.\n\
- Slightly fewer smoke puffs on the Doom Cannon.\n\
- The Chasecam system has been entirely redone from the ground up to be ACS-based rather than consolecommand based, which not only allows it to be used in ZDoom but also fixes the 'exit level while morphed' glitch.\n\
- The Lost Frame's pain frame is now BRIGHT.\n\
- The main menu now has graphical replacements.\n\
- Morph Ball bombs and Power Bombs exploding now alert enemies.\n\
- The beam charge spawns have been changed from an A_FireCustomMissile based system to an A_SpawnItemEx system, which makes them follow the player a lot more closely and doesn't lag behind.\n\
- The visual intensity of the speed booster is slightly reduced.\n\
- ZDoom play has a menu in Options for all the cvars.\n\
- Speed Booster now has a non-SNES-style sound, thanks to Repo Man.\n\
- The Charge Combo now has an indicator on the HUD, when it's in the player's inventory.\n\
- Thanks to BouncyTEM, Samus' sounds have been edited to be less reminiscent of Zero Suit Samus and more reminiscent of Prime in-suit Samus.\n\
- The Morph Ball now has its own set of taunt, usefail, and pain sounds.\n\
- A unique CONFONT has been implemented, thanks to WildWeasel.";