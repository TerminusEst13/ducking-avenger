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

int SoundSlotIndex[PLAYERMAX];

// LASERS MOTHERFUCKER

#define LASER_ROTATETICS 72
#define LASER_NOEFFECTS_FADELENGTH 192
#define LASER_BASEALPHA 0.5

int TempCoords[PLAYERMAX][3];
int TempTrail;
int TempPuff;
int TempEnd;

int Hack_ArgsSet[10];

int Hack_StartX;
int Hack_StartY;
int Hack_StartZ;

int Hack_EndX;
int Hack_EndY;
int Hack_EndZ;

int Hack_VelX;
int Hack_VelY;
int Hack_VelZ;

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