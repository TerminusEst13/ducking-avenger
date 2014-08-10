#define METROID_SV_CVAR_VERSION    2
#define METROID_CL_CVAR_VERSION    5

#define METROID_BASE                        354

#define METROID_SPACEJUMP_ENTER2            351
#define METROID_SPACEJUMP_ENTER             352
#define METROID_OPEN                        353

#define METROID_WALLBOUNCE                  364
#define METROID_REQUESTDODGE                368

#define METROID_TURN                        385

#define METROID_POWERBOMB                   454
#define METROID_LONG_XYZ                    555
#define METROID_LONG_BURST                  556
#define METROID_LONG_TRAIL                  559
#define METROID_LONG_COORDSHIT              568

#define METROID_OPEN_CLIENT                 551
#define METROID_SELECT                      552
#define METROID_SELECT_CLIENT               553
#define METROID_SELECT_SERVER               554
#define METROID_CHANGELOG                   583
#define METROID_SHIPCONTROLS                584
#define METROID_PUKE                        585
#define METROID_SPEEDBOOSTER                586
#define METROID_MORPHCAMERA                 587
#define METROID_MORPHBALL                   588
#define METROID_UNLOADING                   589
#define METROID_DEATH                       590
#define METROID_DISCONNECT                  591
#define METROID_ENTER                       592
#define METROID_RESPAWN                     593
#define METROID_DECORATE                    594
#define METROID_BOOSTBALL                   595
#define METROID_DECORATECLIENT              596
#define METROID_BWEEBWEEBWEEBWEE            597
#define METROID_BWEEBWEEMORPH               598
#define METROID_ENTER_CLIENTSIDE            599

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


#define BEAMCOUNT   8

int KnownBeams[BEAMCOUNT] = 
{
    "Power Beam",
    "Spazer Beam",
    "Plasma Beam",
    "Wave Beam",
    "Ice Beam",
    "Long Beam",
    "Chroma Storm",
    "Doom Cannon",
};

#define STACKTYPES 3

int BeamStackWeapons[STACKTYPES] = 
{
    "SuperShotgun",
    "RocketLauncher",
    "PlasmaRifle",
};

#define STACKPERMUTATIONS   3

// index 0 is beam 1's index (KnownBeams)
// index 1 is beam 2's index
// index 2 is the stacked weapon index (BeamStackWeapons)
// no need to duplicate combos flipped around

int BeamStackCombinations[STACKTYPES][3] =
{
    {2, 7, 0},
    {4, 7, 1},
    {2, 3, 2},
};


#define DROPCOUNT 5

#define D_DROPITEM      0
#define D_CHECKITEM     1
#define D_CHECKAMMO     2

#define DN_NOSPAWNCHANCE 0
#define DN_PICKCHANCE    1

int MonsterDropItems[DROPCOUNT][3] =
{
    {"Energy10_Dropped",            "",                     "Health"},
    {"Energy25_Dropped",            "",                     "Health"},
    {"MissileAmmo_Dropped",         "",                     "MissileAmmo"},
    {"SuperMissileAmmo_Dropped",    "SuperMissileAcquired", "SuperMissileAmmo"},
    {"PowerBombAmmo",               "PowerBombAcquired",    "PowerBombAmmo"},
};

int MonsterDropChances[DROPCOUNT][2] =
{
    {80,  15},
    {155, 10},
    {65,  20},
    {175, 5},
    {155, 1},
};

int TempDropState[DROPCOUNT];


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
"From v1 to v1.1:\n\
==========================\n\
- There is now a menu for selecting beams. With the right item, it allows for...\n\
- Beam stacking! Stick two compatible beams together, get a new beam! Endless possibilities! Or at least more then you'll ever need! Exclamation points!\n\
- Players are not allowed to superball unless cheats are explicitly on.\n\
- If the boost ball is stopped, it now plays a fizzle sound effect.\n\
- Spectating and rejoining no longer breaks missile firing.\n\
- No longer does ball-to-body impact have to wait for time to keep flowing.\n\
- IDKFA now gives raw energy armor.\n\
- Improved ALTHUD support by adding health icons and new STTNUM numbers.\n\
- There is now \"Press Y to Accept\" on the difficulty description text.\n\
- Tanks and weapons can no longer be crushed by, well, crushers.\n\
- Raw Energy Deposit now has a corrected pickup message.\n\
- metroid_cl_doomhealth was not being properly archived. This has been fixed.\n\
- No longer does firing a charged Wave beam into a monster's head do 800 damage to that monster. Similarly, wave combos can no longer oneshot Cyberdemons this way.\n\
- Enemies no longer drop items you have no use for in single player. In multiplayer, any item can drop.\n\
- The Long Beam no longer conflicts with the Lasgun from Lelweps.\n\
- Power Bombs now have an obituary.\n\
- Pickup messages have a lot more to say now, if you inquire.";
