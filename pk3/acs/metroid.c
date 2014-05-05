#library "METROID"
#include "zcommon.acs"
#include "commonFuncs.h"

world int 0:MaxJumpCount;

int SamusHealth[PLAYERMAX];
int SamusArmor[PLAYERMAX];
int playerOnFoot[PLAYERMAX];
int IsServer;

int playerJumps[PLAYERMAX] = {0};
int hasKicked[PLAYERMAX]   = {0};
int grabbing[PLAYERMAX]    = {0};
int dontGrab[PLAYERMAX]    = {0};
int oldLedgeVals[PLAYERMAX][2] = {{0}};

int CPlayerGrounds[PLAYERMAX][2];
int PlayerGrounds[PLAYERMAX][2];
int DidSpecials[PLAYERMAX];

int ClientEnterLocks[PLAYERMAX];

int array_runrunruu[PLAYERMAX];
int array_doomHealth[PLAYERMAX];
int array_metpick[PLAYERMAX];

#include "met_const.h"
#include "met_funcs.h"
#include "met_spacejump.h"
#include "met_longbeam.h"




// CVAR HANDLING SHIT
// ==================

script METROID_OPEN OPEN
{
    IsServer = 1;
    int cjumps, oldcjumps;

    if (!GetCVar("metroid_noaircontrol"))
    {
        ConsoleCommand("set metroid_noaircontrol 0");
        ConsoleCommand("archivecvar metroid_noaircontrol");
    }

    if (!GetCVar("metroid_spacejump"))
    {
        ConsoleCommand("set metroid_spacejump 0");
        ConsoleCommand("archivecvar metroid_spacejump");
    }

    if (!GetCVar("metroid_loaded"))
    {
        ConsoleCommand("set metroid_loaded 0");
        ConsoleCommand("archivecvar metroid_loaded");
    }

    if (!GetCVar("metroid_startingtanks"))
    {
        ConsoleCommand("set metroid_startingtanks 0");
        ConsoleCommand("archivecvar metroid_startingtanks");
    }

    if (!GetCVar("metroid_nolevellimiter"))
    {
        ConsoleCommand("set metroid_nolevellimiter 1");
        ConsoleCommand("archivecvar metroid_nolevellimiter");
    }

    if (!GetCVar("metroid_jumpcount"))
    {
        ConsoleCommand("set metroid_jumpcount 2");
        ConsoleCommand("archivecvar metroid_jumpcount");
    }

    if (!GetCVar("metroid_nomorph"))
    {
        ConsoleCommand("set metroid_nomorph 0");
        ConsoleCommand("archivecvar metroid_nomorph");
    }

    if (!GetCVar("metroid_noenemydrops"))
    {
        ConsoleCommand("set metroid_noenemydrops 0");
        ConsoleCommand("archivecvar metroid_noenemydrops");
    }

    if (!GetCVar("metroid_doomcannon"))
    {
        ConsoleCommand("set metroid_doomcannon 0");
        ConsoleCommand("archivecvar metroid_doomcannon");
    }

    if (!GetCVar("metroid_dropstay"))
    {
        ConsoleCommand("set metroid_dropstay 0");
        ConsoleCommand("archivecvar metroid_dropstay");
    }

    if (!GetCVar("metroid_nobfg"))
    {
        ConsoleCommand("set metroid_nobfg 0");
        ConsoleCommand("archivecvar metroid_nobfg");
    }

    if (!GetCVar("metroid_permabfg"))
    {
        ConsoleCommand("set metroid_permabfg 0");
        ConsoleCommand("archivecvar metroid_permabfg");
    }

    if (!GetCVar("metroid_soultanks"))
    {
        ConsoleCommand("set metroid_soultanks 0");
        ConsoleCommand("archivecvar metroid_soultanks");
    }

    if (!GetCVar("metroid_skulltagweapons"))
    {
        ConsoleCommand("set metroid_skulltagweapons 0");
        ConsoleCommand("archivecvar metroid_skulltagweapons");
    }

    while (1)
    {
        if (!GetCvar("compat_clientssendfullbuttoninfo")) { ConsoleCommand("set compat_clientssendfullbuttoninfo 1"); }
        if (!GetCvar("sv_chasecam")) { ConsoleCommand("set sv_chasecam 1"); }

        if (GetCVar("metroid_noaircontrol") == 0) { SetAirControl(0.225); }
        else if (GetCVar("metroid_noaircontrol") == 1) { SetAirControl(0.00390625); }

        oldcjumps = cjumps;
        cjumps = GetCVar("metroid_jumpcount");
        if (cjumps != oldcjumps) { MaxJumpCount = cjumps; }

        delay(1);
    }
}

script METROID_OPEN_CLIENT OPEN clientside
{
    if (!GetCVar("metroid_cl_noeffects"))
    {
        ConsoleCommand("set metroid_cl_noeffects 0");
        ConsoleCommand("archivecvar metroid_cl_noeffects");
    }

    if (!GetCVar("metroid_cl_nosiren"))
    {
        ConsoleCommand("set metroid_cl_nosiren 0");
        ConsoleCommand("archivecvar metroid_cl_nosiren");
    }

    if (!GetCVar("metroid_cl_morphcamera"))
    {
        ConsoleCommand("set metroid_cl_morphcamera 1");
        ConsoleCommand("archivecvar metroid_cl_morphcamera");
    }

    if (!GetCVar("metroid_cl_doomhealth"))
    {
        ConsoleCommand("set metroid_cl_doomhealth 0");
        ConsoleCommand("archivecvar metroid_cl_doomhealth");
    }

    if (!GetCVar("metroid_cl_nometroidpickups"))
    {
        ConsoleCommand("set metroid_cl_nometroidpickups 0");
        ConsoleCommand("archivecvar metroid_cl_nometroidpickups");
    }

    if (!GetCVar("metroid_cl_pickupmusic"))
    {
        ConsoleCommand("set metroid_cl_pickupmusic 0");
        ConsoleCommand("archivecvar metroid_cl_pickupmusic");
    }
}


// MORPH BALL CAMERA SHIT
// ========================

Script METROID_MORPHCAMERA (int bitches) CLIENTSIDE
{
    if (PlayerNumber() != ConsolePlayerNumber()) { terminate; }
    int pNum = PlayerNumber();

    switch (bitches)
    {
    case 0:
        if (GetCvar("metroid_cl_morphcamera") == 1 && !CheckInventory("IsAFuckingSpaceshipBoyeeee"))
        { ConsoleCommand("chase"); TakeInventory("PlayerMorphCamera",1); }
        break;

    case 1:
        if (GetCvar("metroid_cl_morphcamera") == 1 && !CheckInventory("IsAFuckingSpaceshipBoyeeee"))
        { ConsoleCommand("chase"); GiveInventory("PlayerMorphCamera",1); }
        break;

    case 2:
        delay(1);
        if (CheckInventory("PlayerMorphCamera") == 0)
        { if (GetCvar("metroid_cl_morphcamera") == 1 && !CheckInventory("IsAFuckingSpaceshipBoyeeee"))
        { ConsoleCommand("chase"); GiveInventory("PlayerMorphCamera",1); } }
        break;
    }
}


// MORPH BALL SHIT
// =================

script METROID_MORPHBALL (int morphshit)
{
    int pNum = PlayerNumber();
    int CheckerTID = 1500+pNum;
    int velx, vely, velz;

    switch (morphshit)
    {
    case 0:
        if(CheckInventory("PowerInvulnerable") == 1) { ActivatorSound("morphball/denied", 127); Print(s:"Morphing while invulnerable is temporarily disabled due to a bug.\n\nSorry."); terminate; }
        if(CheckInventory("SpeedBoosterActive") == 1) { ActivatorSound("morphball/denied", 127); terminate; }
        if(CheckInventory("RedFlag") == 1) { ActivatorSound("morphball/denied", 127); terminate; }
        if(CheckInventory("BlueFlag") == 1) { ActivatorSound("morphball/denied", 127); terminate; }
        // Play sound...
        ActivatorSound("morphball/morph", 127);

        // Prepare for health transferring from old player to morphed player...
        SamusHealth[pNum] = GetActorProperty(0, APROP_HEALTH);
        SamusArmor[pNum] = CheckInventory("Armor");
        int newTID = unusedTID(23000, 25000);
        int myTID  = defaultTID(-1);

        // Prepare velocities...
        velx = GetActorVelX(0);
        vely = GetActorVelY(0);
        velz = GetActorVelZ(0);

        // Transfer the player's translation from the old body to the new morph.
        Spawn("TranslationHolder", GetActorX(0), GetActorY(0), GetActorZ(0)+8.0, newTID);
        Thing_SetTranslation(newTID, -1);
        if (isSinglePlayer() || isCoop()) { MorphActor(0, "MorphBallPlayer", "", 0x7FFFFFFF, 194, "emptytelefog", "emptytelefog"); }
        else { MorphActor(0, "MorphBallPlayerDM", "", 0x7FFFFFFF, 194, "emptytelefog", "emptytelefog"); }
        if (CheckInventory("SpaceJumpAcquired") == 1) { SetActorProperty(0,APROP_JumpZ,6.0); }
        Thing_ChangeTID(0, myTID);
        SetActivator(newTID);
        Thing_SetTranslation(myTID, -1);
        SetActivator(myTID);
        Thing_Remove(newTID);
        SetActorVelocity(0, velx,vely,velz, 0,0);

        if (isSinglePlayer() || isCoop()) { SetActorState(0,"CoopModeOn"); }

        // Transfer health and give inventory.
        SetActorProperty(0, APROP_HEALTH, SamusHealth[pNum]);
        GiveInventory("RawEnergyShield2",SamusArmor[pNum]);
        GiveInventory("BorphMallAcquired", 1);
        TakeInventory("MorphBallActivate", 1);
        TakeInventory("IceBeamChilled",999);
        GiveInventory("MorphBallDeactivate", 1);

        TakeInventory("PowerBeamChargeLevel",999);
        //TakeInventory("PowerBeamCharged",999);
        TakeInventory("PowerBeamIdle",999);
        TakeInventory("TheReturnOfSamusReturns",999);
        TakeInventory("MissileChargeLevel",999);
        TakeInventory("MissileCharged",999);
        playerOnFoot[pNum] = 1;

        ACS_ExecuteAlways(METROID_MORPHCAMERA,0,0);
        ACS_ExecuteAlways(METROID_BWEEBWEEMORPH,0);
        break;

    case 1:
        if (Spawn("SpaceChecker", GetActorX(0), GetActorY(0), GetActorZ(0), CheckerTID, GetActorAngle(0) >> 8 ) > 0)
        {
        ActivatorSound("morphball/unmorph", 127);
        SamusHealth[pNum] = GetActorProperty(0, APROP_HEALTH);
        velx = GetActorVelX(0);
        vely = GetActorVelY(0);
        velz = GetActorVelZ(0);
        //Thing_Remove(CheckerTID);
        UnmorphActor(0, 1);
        SetActorProperty(0, APROP_HEALTH, SamusHealth[pNum]);
        SetActorProperty(0,APROP_SPEED,1.00);
        TakeInventory("BorphMallAcquired", 1);
        TakeInventory("BoostBallCount", 99);
        TakeInventory("MissileChargeLevel",999);
        TakeInventory("MissileCharged",999);
        GiveInventory("MorphBallActivate", 1);
        TakeInventory("MorphBallDeactivate", 1);
        TakeInventory("BallBoosting", 1);
        SetActorVelocity(0, velx,vely,velz, 0,0);
        playerOnFoot[pNum] = 0;

        ACS_ExecuteAlways(METROID_MORPHCAMERA,0,1);

        ACS_ExecuteAlways(352,0,0,0);
        ACS_ExecuteAlways(351,0,0,0);
        ACS_ExecuteAlways(METROID_BWEEBWEEBWEEBWEE,0);
        }
        else { ActivatorSound("morphball/denied", 127); }
        break;
    }
}

script METROID_BOOSTBALL (void) NET
{

    int vx, vy, vz,  mag, angle, pitch;
    int nx, ny, nz, nmag;
    int rx, ry, rz, rmag;

        vx = GetActorVelX(0); // Checks for the player's X velocity and assigns it to vx
        vy = GetActorVelY(0); // Same for Y
        vz = GetActorVelZ(0); // Same for Z
        angle = GetActorAngle(0); // Checks for the player's angle and assigns it to angle
        pitch = middle(-0.027, GetActorPitch(0), 0.25); // This checks for the middle ground of the player's pitch, and will register up to a certain height
        mag = magnitudeThree_f(vx, vy, vz);

        nx = FixedMul(DASH_VEL * cos(angle), cos(pitch));
        ny = FixedMul(DASH_VEL * sin(angle), cos(pitch));
        nz = DASH_VEL * -sin(pitch);
        nmag = magnitudeThree_f(nx, ny, nz);
        
        rx = vx + nx; ry = vy + ny; rz = vz + nz;
        rmag = magnitudeThree_f(rx, ry, rz);

        ActivatorSound("boostball/launch",255);

        if (rmag < mag)  // if we're making a sharp turn
        {
            SetActorVelocity(0, nx, ny, nz, 0,0);
        }
        else
        {
            mag += (DASH_VEL<<16);
            SetActorVelocity(0,
                    FixedMul(FixedMul(mag, cos(angle)), cos(pitch)),
                    FixedMul(FixedMul(mag, sin(angle)), cos(pitch)),
                    FixedMul(mag, -sin(pitch)), 0, 0);
        }
        SetActorState(0,"BoostBall");
}

// ENTER EXIT DEATH RESPAWN SHIT
// ========================

script METROID_UNLOADING UNLOADING
{
    SetPlayerProperty(0,0,PROP_TOTALLYFROZEN);
    SetActorProperty(0,APROP_SPEED,1.00);

    // Adjusts inventory
    TakeInventory("BorphMallAcquired",999);
    TakeInventory("PowerBeamChargeLevel",999);
    TakeInventory("PowerBeamCharged",999);
    TakeInventory("PowerBeamIdle",999);
    TakeInventory("Bombs",999);
    TakeInventory("BoostBallCount",999);
    TakeInventory("ETNotInThisLevel",999);
    TakeInventory("PBNotInThisLevel",999);
    TakeInventory("SMNotInThisLevel",999);
    TakeInventory("IceBeamChilled",999);
    TakeInventory("MissileChargeLevel",999);
    TakeInventory("MissileCharged",999);
    TakeInventory("SynthFireRight",999);
    TakeInventory("SpeedBooster",999);
    TakeInventory("SpeedBoosterPrepare",999);
    TakeInventory("SpeedBoostCounter",999);
    TakeInventory("SpeedBoosterActive",999);
    TakeInventory("SpeedWalker",999);
    TakeInventory("SpeedWalkerPrepare",999);
    TakeInventory("TheReturnOfSamusReturns",999);
    TakeInventory("BallBoosting",1);

    GiveInventory("BombCount",999);
}

script METROID_DEATH DEATH
{
    if (CheckInventory("BorphMallAcquired")) { ACS_ExecuteAlways(METROID_MORPHCAMERA,0,1); }
    ACS_ExecuteAlways(589,0);
}

Script METROID_DISCONNECT (int p_num) DISCONNECT // Guess this isn't really needed anymore, but...
{
       //cam_mode[p_num] = OFF;
       //Thing_Remove (C_TID + p_num);
}

script METROID_ENTER ENTER
{
    int barhp;
    int oarmor;
    int armor;
    int pln = PlayerNumber();
    int frozen = 0, wasfrozen;
    int jumpz = GetActorProperty(0, APROP_JumpZ);
    int frozenjumpz = FixedDiv(jumpz, sqrt(2.0));
    int i;

    SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
    SetActorProperty(0, APROP_INVULNERABLE, 0);

    if (CheckInventory("MorphBallDeactivate") == 1) { GiveInventory("MorphBallActivate", 1); TakeInventory("MorphBallDeactivate", 1); }
    ACS_ExecuteAlways(METROID_MORPHCAMERA,0,2);

    if (isFreeForAll() || isTeamgame()) { SetAmmoCapacity("MissileAmmo",10); GiveInventory("MissileAmmo",5); GiveInventory("MissileTankAcquired",1); }//if (GetCvar("metroid_startingtanks") == 0) { GiveInventory("EnergyTankAcquired",1); SetActorProperty(0,APROP_SPAWNHEALTH,200); SetActorProperty(0,APROP_HEALTH,200); } }
    if (isSinglePlayer() || isCoop()) { if (CheckInventory("CoopModeOn") == 0) { GiveInventory("CoopModeOn",1); SetActorState(0,"CoopModeOn"); }}

    ACS_ExecuteAlways(352,0,0,0); // Activates Space Jump mode.
    ACS_ExecuteAlways(351,0,0,0);

    if (!CheckInventory("InGame")) {

        if (GetCVar("metroid_loaded") == 1)
        {
            if (GetCvar("metroid_doomcannon") == 1) { GiveInventory("Doom Cannon",1); GiveInventory("DoomCannonAcquired",1); }
            if (GetCvar("metroid_skulltagweapons") == 1) { GiveInventory("Chroma Storm",1); GiveInventory("ChromaStormAcquired",1); }
            GiveInventory("Spazer Beam",1); GiveInventory("SpazerBeamAcquired",1);
            GiveInventory("Plasma Beam",1); GiveInventory("PlasmaBeamAcquired",1);
            GiveInventory("Wave Beam",1); GiveInventory("WaveBeamAcquired",1);
            GiveInventory("Ice Beam",1); GiveInventory("IceBeamAcquired",1);
            GiveInventory("Long Beam",1); GiveInventory("LongBeamAcquired",1);
        }

        if (GetCvar("metroid_loaded") == 2) { SetAmmoCapacity("MissileAmmo",10); GiveInventory("MissileAmmo",5); GiveInventory("MissileTankAcquired",1); }

        if (GetCvar("metroid_loaded") == 3) { SetAmmoCapacity("MissileAmmo",20); GiveInventory("MissileAmmo",15); GiveInventory("MissileTankAcquired",1); }

        if (GetCVar("metroid_loaded") == 4)
        {
            if (GetCvar("metroid_doomcannon") == 1) { GiveInventory("Doom Cannon",1); GiveInventory("DoomCannonAcquired",1); }
            if (GetCvar("metroid_skulltagweapons") == 1) { GiveInventory("Chroma Storm",1); GiveInventory("ChromaStormAcquired",1); }
            GiveInventory("Spazer Beam",1); GiveInventory("SpazerBeamAcquired",1);
            GiveInventory("Plasma Beam",1); GiveInventory("PlasmaBeamAcquired",1);
            GiveInventory("Wave Beam",1); GiveInventory("WaveBeamAcquired",1);
            GiveInventory("Ice Beam",1); GiveInventory("IceBeamAcquired",1);
            GiveInventory("Long Beam",1); GiveInventory("LongBeamAcquired",1);
            SetAmmoCapacity("MissileAmmo",20); GiveInventory("MissileAmmo",15); GiveInventory("MissileTankAcquired",1); 
        }

        if (GetCVar("metroid_loaded") == 5)
        {
            if (GetCvar("metroid_doomcannon") == 1) { GiveInventory("Doom Cannon",1); GiveInventory("DoomCannonAcquired",1); }
            if (GetCvar("metroid_skulltagweapons") == 1) { GiveInventory("Chroma Storm",1); GiveInventory("ChromaStormAcquired",1); }
            GiveInventory("Spazer Beam",1); GiveInventory("SpazerBeamAcquired",1);
            GiveInventory("Plasma Beam",1); GiveInventory("PlasmaBeamAcquired",1);
            GiveInventory("Wave Beam",1); GiveInventory("WaveBeamAcquired",1);
            GiveInventory("Ice Beam",1); GiveInventory("IceBeamAcquired",1);
            GiveInventory("Long Beam",1); GiveInventory("LongBeamAcquired",1);
            SetAmmoCapacity("MissileAmmo",20); GiveInventory("MissileAmmo",15); GiveInventory("MissileTankAcquired",1); 
            SetAmmoCapacity("SuperMissileAmmo",4); GiveInventory("SuperMissileAmmo",4); GiveInventory("SuperMissileAcquired",1); 
            SetAmmoCapacity("PowerBombAmmo",2); GiveInventory("PowerBombAmmo",2); GiveInventory("PowerBombAcquired",1); 
        }

        if (GetCVar("metroid_loaded") == 6)
        {
            if (GetCvar("metroid_doomcannon") == 1) { GiveInventory("Doom Cannon",1); GiveInventory("DoomCannonAcquired",1); }
            if (GetCvar("metroid_skulltagweapons") == 1) { GiveInventory("Chroma Storm",1); GiveInventory("ChromaStormAcquired",1); }
            GiveInventory("Spazer Beam",1); GiveInventory("SpazerBeamAcquired",1);
            GiveInventory("Plasma Beam",1); GiveInventory("PlasmaBeamAcquired",1);
            GiveInventory("Wave Beam",1); GiveInventory("WaveBeamAcquired",1);
            GiveInventory("Ice Beam",1); GiveInventory("IceBeamAcquired",1);
            GiveInventory("Long Beam",1); GiveInventory("LongBeamAcquired",1);
            SetAmmoCapacity("MissileAmmo",20); GiveInventory("MissileAmmo",15); GiveInventory("MissileTankAcquired",1); 
            SetAmmoCapacity("SuperMissileAmmo",4); GiveInventory("SuperMissileAmmo",4); GiveInventory("SuperMissileAcquired",1); 
            SetAmmoCapacity("PowerBombAmmo",2); GiveInventory("PowerBombAmmo",2); GiveInventory("PowerBombAcquired",1); 
            GiveInventory("SpeedBoosterAcquired",1);
            GiveInventory("ChargeComboAcquired",1);
            GiveInventory("SpaceJumpAcquired",1);
        }

        if (GetCvar("metroid_startingtanks") == 1) { GiveInventory("EnergyTankAcquired",1); SetActorProperty(0,APROP_SPAWNHEALTH,200); SetActorProperty(0,APROP_HEALTH,200); }
        if (GetCvar("metroid_startingtanks") == 2) { GiveInventory("EnergyTankAcquired",2); SetActorProperty(0,APROP_SPAWNHEALTH,300); SetActorProperty(0,APROP_HEALTH,300); }
        if (GetCvar("metroid_startingtanks") == 3) { GiveInventory("EnergyTankAcquired",3); SetActorProperty(0,APROP_SPAWNHEALTH,400); SetActorProperty(0,APROP_HEALTH,400); }
        if (GetCvar("metroid_startingtanks") == 4) { GiveInventory("EnergyTankAcquired",4); SetActorProperty(0,APROP_SPAWNHEALTH,500); SetActorProperty(0,APROP_HEALTH,500); }
        if (GetCvar("metroid_startingtanks") == 5) { GiveInventory("EnergyTankAcquired",5); SetActorProperty(0,APROP_SPAWNHEALTH,600); SetActorProperty(0,APROP_HEALTH,600); }
        if (GetCvar("metroid_startingtanks") == 6) { GiveInventory("EnergyTankAcquired",6); SetActorProperty(0,APROP_SPAWNHEALTH,700); SetActorProperty(0,APROP_HEALTH,700); }
        if (GetCvar("metroid_startingtanks") == 7) { GiveInventory("EnergyTankAcquired",7); SetActorProperty(0,APROP_SPAWNHEALTH,800); SetActorProperty(0,APROP_HEALTH,800); }
        if (GetCvar("metroid_startingtanks") == 8) { GiveInventory("EnergyTankAcquired",8); SetActorProperty(0,APROP_SPAWNHEALTH,900); SetActorProperty(0,APROP_HEALTH,900); }
        if (GetCvar("metroid_startingtanks") == 9) { GiveInventory("EnergyTankAcquired",9); SetActorProperty(0,APROP_SPAWNHEALTH,1000); SetActorProperty(0,APROP_HEALTH,1000); }
        if (GetCvar("metroid_startingtanks") == 10) { GiveInventory("EnergyTankAcquired",10); SetActorProperty(0,APROP_SPAWNHEALTH,1100); SetActorProperty(0,APROP_HEALTH,1100); }

        /*if (GameType () != GAME_TITLE_MAP)
            {
            SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
            SetActorProperty(0, APROP_INVULNERABLE, 1);
            ACS_ExecuteAlways(METROID_DECORATECLIENT,0,3,0,0);
            LocalAmbientSound("system/samusappear",127);
            delay(210);
            SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
            SetActorProperty(0, APROP_INVULNERABLE, 0);
            ACS_ExecuteAlways(METROID_DECORATECLIENT,0,4,0,0);
            }*/
    }
    GiveInventory("InGame",1);

    while (1)
    {
        // Health bar shit
        if (GetActorProperty(0,APROP_Health) > 100) { GiveInventory("HealthOver100",1); } else { if (CheckInventory("HealthOver100") == 1) { TakeInventory("HealthOver100",1); }}
        if (GetActorProperty(0,APROP_Health) > 200) { GiveInventory("HealthOver200",1); } else { if (CheckInventory("HealthOver200") == 1) { TakeInventory("HealthOver200",1); }}
        if (GetActorProperty(0,APROP_Health) > 300) { GiveInventory("HealthOver300",1); } else { if (CheckInventory("HealthOver300") == 1) { TakeInventory("HealthOver300",1); }}
        if (GetActorProperty(0,APROP_Health) > 400) { GiveInventory("HealthOver400",1); } else { if (CheckInventory("HealthOver400") == 1) { TakeInventory("HealthOver400",1); }}
        if (GetActorProperty(0,APROP_Health) > 500) { GiveInventory("HealthOver500",1); } else { if (CheckInventory("HealthOver500") == 1) { TakeInventory("HealthOver500",1); }}
        if (GetActorProperty(0,APROP_Health) > 600) { GiveInventory("HealthOver600",1); } else { if (CheckInventory("HealthOver600") == 1) { TakeInventory("HealthOver600",1); }}
        if (GetActorProperty(0,APROP_Health) > 700) { GiveInventory("HealthOver700",1); } else { if (CheckInventory("HealthOver700") == 1) { TakeInventory("HealthOver700",1); }}
        if (GetActorProperty(0,APROP_Health) > 800) { GiveInventory("HealthOver800",1); } else { if (CheckInventory("HealthOver800") == 1) { TakeInventory("HealthOver800",1); }}
        if (GetActorProperty(0,APROP_Health) > 900) { GiveInventory("HealthOver900",1); } else { if (CheckInventory("HealthOver900") == 1) { TakeInventory("HealthOver900",1); }}
        if (GetActorProperty(0,APROP_Health) > 1000) { GiveInventory("HealthOver1000",1); } else { if (CheckInventory("HealthOver1000") == 1) { TakeInventory("HealthOver1000",1); }}

        if (GetActorProperty(0, APROP_Health) >= ((CheckInventory("EnergyTankAcquired")+1) * 100)) { barHP = GetActorProperty(0, APROP_Health); }
        else { barhp = GetActorProperty(0, APROP_Health) % 100; }

        TakeInventory("PlayerTotalHealth", 0x7FFFFFFF);
        GiveInventory("PlayerTotalHealth", min(barhp, 99));

        // Spacejump shit
        if (GetCVar("metroid_spacejump") == 1) { if (CheckInventory("CanSpaceJump") == 0) { GiveInventory("CanSpaceJump",1); }}
        else if (GetCVar("metroid_spacejump") == 0) { if (CheckInventory("SpaceJumpAcquired") == 0) { if (CheckInventory("CanSpaceJump") == 1) { TakeInventory("CanSpaceJump",1); }}}

        // Nomorph shit
        if (GetCVar("metroid_nomorph") == 1) { if (CheckInventory("DisableMorph") == 0) { GiveInventory("DisableMorph",1); }}
        else if (GetCVar("metroid_nomorph") == 0) { if (CheckInventory("DisableMorph") == 1) { TakeInventory("DisableMorph",1); }}

        // Clientside shit
        if (array_doomHealth[pln]) { GiveInventory("DoomHealthCounter", 1); }
        else { TakeInventory("DoomHealthCounter", 0x7FFFFFFF); }
        
        if (array_runrunruu[pln]) { GiveInventory("AlwaysRunIsOn", 1); }
        else { TakeInventory("AlwaysRunIsOn", 0x7FFFFFFF); }
        
        if (array_metpick[pln]) { GiveInventory("NoMetroidPickupSystem", 1); }
        else { TakeInventory("NoMetroidPickupSystem", 0x7FFFFFFF); }

        // Armor shit
        oarmor = armor;
        armor = CheckInventory("Armor");

        if (oarmor > armor)
        { ActivatorSound("rawenergy/shieldhit", 127);
            FadeRange(255, 255, 255, min(0.5, (oarmor - armor) * 0.015), 0, 0, 0, 0.0, min(35.0, 1.5 * (oarmor - armor)) / 35); }

        // Charge combo shit
        if (CheckInventory("ChargeComboAcquired") == 1)
        {
        if (keyDown(BT_ALTATTACK)) { GiveInventory("SynthFireRight", 1); }
        else { TakeInventory("SynthFireRight", 0x7FFFFFFF); }
        }

        // FREEZE, MOFUCKA
        wasfrozen = frozen;
        frozen = CheckInventory("IceBeamChilled");

        if (!wasfrozen && frozen) { SetActorProperty(0, APROP_JumpZ, frozenjumpz); }
        if (!frozen && wasfrozen) { SetActorProperty(0, APROP_JumpZ, jumpz); }

        // Clear pickup status
        for (i = 0; i < PICKUPTYPES; i++) { GotBigPickup[pln][i] = 0; }

        if (isDead(0)) { terminate; }
        delay(1);
    }
}

script METROID_ENTER_CLIENTSIDE ENTER clientside
{
    int execInt, oExecInt, execStr;
    int pln = PlayerNumber();

    while(1)
    {

    if (ConsolePlayerNumber() != PlayerNumber()) { Delay(1); continue; }

            oExecInt = execInt;
            execInt = MetroidClientVars();

            if (execInt != oExecInt)
            {
                execStr = StrParam(s:"puke -", d:METROID_PUKE, s:" ", d:execInt, s:" ", d:pln);
                ConsoleCommand(execStr);
            }
        //if (GetCVar("metroid_cl_doomhealth") == 1) { if (CheckInventory("DoomHealthCounter") == 0) { GiveInventory("DoomHealthCounter",1); }}
        //else { if (CheckInventory("DoomHealthCounter") == 1) { TakeInventory("DoomHealthCounter",1); }}

        //if (GetCvar("cl_run") == 1) { if (CheckInventory("AlwaysRunIsOn") == 0) { GiveInventory("AlwaysRunIsOn",1); }}
        //else { if (CheckInventory("AlwaysRunIsOn") == 1) { TakeInventory("AlwaysRunIsOn",1); }}

        delay(1);
    }
}

script METROID_PUKE (int values) net
{
    int pln = PlayerNumber();

    array_runrunruu[pln]     = values & 1;
    array_doomHealth[pln]    = values & 2;
    array_metpick[pln]       = values & 4;
    /*array_weaponBar[pln]    = values & 8;
    array_pickupswitch[pln] = values & 16;*/
}

   // These are stupidly hacky and a wasteful pair of scripts, but I'm
   // really fucking tired of the multi-siren bug.
script METROID_BWEEBWEEBWEEBWEE ENTER clientside
{
    int endloop;
    if (PlayerNumber() != ConsolePlayerNumber()) { terminate; }

    while (!endloop)
    {
    if (isDead(0)) { terminate; }
    delay(2);
    if (CheckInventory("BorphMallAcquired") == 1) { terminate; }
    if (isDead(0)) { terminate; }
    if (GetActorProperty(0,APROP_HEALTH) > 0) {
        if (GetActorProperty(0,APROP_HEALTH) <= 30) {
            if (GetCvar("metroid_cl_nosiren") == 0) {
                LocalAmbientSound("system/healthsiren",96); }}}
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(1);
    }
}

script METROID_BWEEBWEEMORPH (void) clientside
{
    int endloop;
    if (PlayerNumber() != ConsolePlayerNumber()) { terminate; }

    while (!endloop)
    {
    if (isDead(0)) { terminate; }
    delay(2);
    if (CheckInventory("BorphMallAcquired") == 0) { terminate; }
    if (isDead(0)) { endloop = 1; }
    if (GetActorProperty(0,APROP_HEALTH) > 0) {
        if (GetActorProperty(0,APROP_HEALTH) <= 30) {
            if (GetCvar("metroid_cl_nosiren") == 0) {
                LocalAmbientSound("system/healthsiren",96); }}}
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(1);
    }
}

script METROID_RESPAWN RESPAWN
{
    ACS_ExecuteAlways(METROID_ENTER,0);
    ACS_ExecuteAlways(METROID_ENTER_CLIENTSIDE,0);
    ACS_ExecuteAlways(METROID_BWEEBWEEBWEEBWEE,0);
    ACS_ExecuteAlways(METROID_SPEEDBOOSTER,0);

    if (isSinglePlayer() || isCoop()) // This shouldn't ever matter in any PvP modes, since their inventory resets on death, but just in case.
    {
        if (CheckInventory("EnergyTankAcquired") == 1) { SetActorProperty(0,APROP_SPAWNHEALTH,200); SetActorProperty(0,APROP_HEALTH,200); }
        if (CheckInventory("EnergyTankAcquired") == 2) { SetActorProperty(0,APROP_SPAWNHEALTH,300); SetActorProperty(0,APROP_HEALTH,300); }
        if (CheckInventory("EnergyTankAcquired") == 3) { SetActorProperty(0,APROP_SPAWNHEALTH,400); SetActorProperty(0,APROP_HEALTH,400); }
        if (CheckInventory("EnergyTankAcquired") == 4) { SetActorProperty(0,APROP_SPAWNHEALTH,500); SetActorProperty(0,APROP_HEALTH,500); }
        if (CheckInventory("EnergyTankAcquired") == 5) { SetActorProperty(0,APROP_SPAWNHEALTH,600); SetActorProperty(0,APROP_HEALTH,600); }
        if (CheckInventory("EnergyTankAcquired") == 6) { SetActorProperty(0,APROP_SPAWNHEALTH,700); SetActorProperty(0,APROP_HEALTH,700); }
        if (CheckInventory("EnergyTankAcquired") == 7) { SetActorProperty(0,APROP_SPAWNHEALTH,800); SetActorProperty(0,APROP_HEALTH,800); }
        if (CheckInventory("EnergyTankAcquired") == 8) { SetActorProperty(0,APROP_SPAWNHEALTH,900); SetActorProperty(0,APROP_HEALTH,900); }
        if (CheckInventory("EnergyTankAcquired") == 9) { SetActorProperty(0,APROP_SPAWNHEALTH,1000); SetActorProperty(0,APROP_HEALTH,1000); }
        if (CheckInventory("EnergyTankAcquired") == 10) { SetActorProperty(0,APROP_SPAWNHEALTH,1100); SetActorProperty(0,APROP_HEALTH,1100); }
    }
}


// DECORATE CHECKS
// =====================

script METROID_DECORATE (int which, int a1, int a2)
{
    int burrshet;
    int result;
    int pln = PlayerNumber();
    int i;

    switch (which)
    {
    case 0:
        SetResultValue(!!GetCVar("metroid_cl_noeffects"));
        break;

    case 1:
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*4/5)); // 80% of the current player speed. Stackable.
        GiveInventory("IceBeamChilled",1);
        delay(100);
        TakeInventory("IceBeamChilled",1);
        delay(75);
        SetActorProperty(0,APROP_SPEED,1.00);
        break;

    case 2:
        SetPlayerProperty(0,1,PROP_TOTALLYFROZEN);
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*2/3)); // 66% of the current player speed. Stackable.
        GiveInventory("IceBeamChilled",1);
        delay(20);
        SetPlayerProperty(0,0,PROP_TOTALLYFROZEN);
        delay(85);
        TakeInventory("IceBeamChilled",1);
        delay(95);
        SetActorProperty(0,APROP_SPEED,1.00);
        break;

    case 3:
        SetResultValue(!!GetCVar("sv_weaponstay"));
        break;

    case 4:
        SetResultValue(!!GetCVar("sv_noweaponspawn"));
        break;

    case 5:
        SetActorProperty(0, APROP_SpawnHealth,  (1 + CheckInventory("EnergyTankAcquired")) * 100);
        SetActorProperty(0, APROP_Health, max(GetActorProperty(0, APROP_Health), (1 + CheckInventory("EnergyTankAcquired")) * 100));
        break;

    case 6:
        SetAmmoCapacity("MissileAmmo",GetAmmoCapacity("MissileAmmo")+5);
        delay(1);
        GiveInventory("MissileAmmo",5);
        break;

    case 7:
        if(GetCvar("metroid_nolevellimiter") == 0) { GiveInventory("ETNotInThisLevel",1); }
        break;

    case 8:
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*1/2)); // 50% of the current monster's speed. Stackable.
        break;

    case 9:
        SetAmmoCapacity("SuperMissileAmmo",GetAmmoCapacity("SuperMissileAmmo")+2);
        delay(1);
        GiveInventory("SuperMissileAmmo",2);
        break;

    case 10:
        SetResultValue(!!GetCVar("metroid_noenergydrops"));
        break;

    case 11:
        SetResultValue(!!GetCVar("metroid_soultanks"));
        break;

    case 12:
        SetAmmoCapacity("PowerBombAmmo",GetAmmoCapacity("PowerBombAmmo")+1);
        delay(1);
        GiveInventory("PowerBombAmmo",1);
        break;

    case 13:
        if(GetCvar("metroid_doomcannon") == 1)
        SetResultValue(1);
        else SetResultValue(0);
        break;

    case 14:
        if(GetCvar("metroid_nolevellimiter") == 0) { GiveInventory("SMNotInThisLevel",1); }
        break;

    case 15:
        if(GetCvar("metroid_nolevellimiter") == 0) { GiveInventory("PBNotInThisLevel",1); }
        break;

    case 16:
        SetResultValue(!!GetCVar("metroid_dropstay"));
        break;

    case 17:
        SetResultValue(!!GetCVar("metroid_nobfg"));
        break;

    case 18:
        SetResultValue(!!GetCVar("metroid_permabfg"));
        break;

    case 19:
        SetResultValue(!!GetCVar("metroid_skulltagweapons"));
        break;

    // [ijon] IT'S MOTHERFUCKING *ARRAY TIME*
    //      NOW WITH 200% MORE INDEXES FOR THAT ALL-NATURAL ASSKICKING TASTE

      case 20:
        if (a1 < 0 || a1 >= PICKUPTYPES) { break; }

        int forceNoPause    = !!a2;
        int whichPickup     = a1;
        int giveItem        = BigPickupItems[whichPickup];
        int message         = BigPickupMsgFonts[whichPickup];
        int giveSound       = BigPickupSounds[whichPickup][2];
        int hadItem         = !!CheckInventory(giveItem);

        GiveInventory(giveItem, 1);

        if (GotBigPickup[pln][whichPickup]) { break; }
        GotBigPickup[pln][whichPickup] = 1;

        if (!(CheckInventory("NoMetroidPickupSystem") || forceNoPause))
        {
            SetFont(message);

            if (!hadItem && isSinglePlayer())
            {
                Thing_Stop(0);
                GiveInventory("PowerTimeFreezer", 1);
                SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
                SetActorProperty(0, APROP_INVULNERABLE, 1);

                HudMessage(s:"A"; HUDMSG_PLAIN, 1, 0, 0.5, 0.3, 6);
                ACS_ExecuteAlways(METROID_DECORATECLIENT, 0, 3, whichPickup);

                for (i = 0; i < 210; i++)
                {
                    if (keyDown(BT_USE)) { break; }
                    Delay(1);
                }

                SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
                SetActorProperty(0, APROP_INVULNERABLE, 0);
                ACS_ExecuteAlways(METROID_DECORATECLIENT, 0, 4);

                TakeInventory("PowerTimeFreezer",0x7FFFFFFF);
                HudMessage(s:""; HUDMSG_PLAIN, 1, 0, 0.5, 0.5, 0.01);
            }
            else
            {
                HudMessage(s:"A"; HUDMSG_PLAIN, 1, 0, 0.5, 0.25, 3);
                ActivatorSound(giveSound, 127);

                for(i = 0; i < 52; i++)
                {
                    if (keyDown(BT_USE)) { break; }
                    Delay(1);
                }

                HudMessage(s:""; HUDMSG_PLAIN, 1, 0, 0.5, 0.5, 0.01);
            }
        }
        else
        {
            ActivatorSound(giveSound, 127);
            ACS_ExecuteAlways(METROID_DECORATECLIENT, 0, 7, whichPickup);
        }
        break;

    case 32:
        delay(32);
        if (isCoop() || isSinglePlayer()) { GiveInventory("BombCount",1); }
        else { delay(8); GiveInventory("BombCount",1); }
        break;

    }
}

script METROID_DECORATECLIENT (int which, int a1, int a2) clientside
{
    if (ConsolePlayerNumber() != PlayerNumber()) { terminate; }

    switch (which)
    {
      case 0:
        SetResultValue(!!GetCVar("metroid_cl_noeffects"));
        break;

      case 1:
        SetResultValue(!!GetCVar("metroid_cl_doomhealth"));
        break;

      case 2:
        SetResultValue(!!GetCVar("cl_run"));
        break;


      case 3:
        if (a1 < 0 || a1 >= PICKUPTYPES) { break; }
        int pickupMus = BigPickupSounds[a1][0];
        int soundItem = BigPickupSounds[a1][1];

        if(GetCvar("metroid_cl_pickupmusic") == 1) { LocalSetMusic(pickupMus); }
        else
        {
            // [ijon] testmusicvol is safer than snd_musicvolume - it does no lasting damage.
            ConsoleCommand("testmusicvol 0");
            GiveInventory(soundItem, 1);
        }
        break;

      case 4:
        if(GetCvar("metroid_cl_pickupmusic") == 1) { LocalSetMusic("*"); }
        else { ConsoleCommand("testmusicvol 1"); }
        break;

      case 7:
        if (a1 < 0 || a1 >= PICKUPTYPES) { break; }
        int pickupColor = msgColors[GetCVar("msg0color")];
        Log(s:pickupColor, s:strstr(BigPickupMessages[a1], "\c-", pickupColor));
        break;
    }
}

script METROID_POWERBOMB (int scaleI, int scaleF, int speedF)
{
    int scale, x, y, z, mag;
    x = GetActorVelX(0); y = GetActorVelY(0); z = GetActorVelZ(0);

    if (speedF == 0) { scale = itof(scaleI) + (scaleF * 0.01); }
    else
    {
        mag = magnitudeThree_f(x, y, z);

        if (mag == 0)
        {
            x = random(-10.0, 10.0);
            y = random(-10.0, 10.0);
            z = random(-10.0, 10.0);
            mag = magnitudeThree_f(x, y, z);
        }

        scale = FixedDiv(speedF * 1.0, mag);
    }

    SetActorVelocity(0, FixedMul(x, scale), FixedMul(y, scale), FixedMul(z, scale), 0, 0);
    // PrintBold(s:"(", f:x, s:", ", f:y, s:", ", f:z, s:"): ", f:scale, s:" -> (", f:GetActorVelX(0), s:", ", f:GetActorVelY(0), s:", ", f:GetActorVelZ(0), s:")");
    // printf("(%f, %f, %f): %f -> (%f, %f, %f)\n", x, y, z, scale, GetActorVelX(0), GetActorVelY(0), GetActorVelZ(0));
}

script METROID_SPEEDBOOSTER ENTER
{
    int buttons;
    int x;
    int y;
    int xb;
    int yb;
    while (1)
    {
    buttons = GetPlayerInput(-1, INPUT_BUTTONS);
    if (CheckInventory("BorphMallAcquired") == 0 && CheckInventory("IsAFuckingSpaceshipBoyeeee") == 0)
    {
    if (CheckInventory("SpeedBoosterAcquired") == 1)
    {
        xb = x;
        yb = y;
        x = GetActorX(0);
        y = GetActorY(0);

        if ((buttons & (BT_SPEED | BT_FORWARD)) == (BT_SPEED | BT_FORWARD) && (xb != x) && (yb != y))//(buttons & BT_SPEED && buttons & BT_FORWARD)// (buttons & (BT_SPEED | BT_FORWARD) == (BT_SPEED | BT_FORWARD))
        {
        
            if (CheckInventory("SpeedBoostCounter") == 0) { ActivatorSound("speedboost/start",127); }
            if (CheckInventory("AlwaysRunIsOn") == 1 ) { TakeInventory("SpeedWalkerPrepare",1); GiveInventory("SpeedBoosterPrepare",1); }
            else { TakeInventory("SpeedBoosterPrepare",1); GiveInventory("SpeedWalkerPrepare",1); }
            GiveInventory("SpeedBoosterActive",1);
            GiveInventory("SpeedBoostCounter",1);
            
            if (CheckInventory("SpeedBoostCounter") > 8)
            {
                ActivatorSound("speedboost/loop",127);
                if (CheckInventory("AlwaysRunIsOn") == 1 ) { TakeInventory("SpeedWalkerPrepare",1); TakeInventory("SpeedWalker",1); GiveInventory("SpeedBoosterPrepare",1); GiveInventory("SpeedBooster",1); /*Print(s:"AlwaysRun is on.");*/ }
                else { TakeInventory("SpeedBoosterPrepare",1); TakeInventory("SpeedBooster",1); GiveInventory("SpeedWalkerPrepare",1); GiveInventory("SpeedWalker",1); /*Print(s:"AlwaysRun is off.");*/ }
                GiveInventory("SpeedBoosterFlashing",1);
            }
        }
        else
        {
            TakeInventory("SpeedBooster",1);
            TakeInventory("SpeedBoosterPrepare",1);
            TakeInventory("SpeedWalker",1);
            TakeInventory("SpeedWalkerPrepare",1);
            TakeInventory("SpeedBoostCounter",0x7FFFFFFF);
            if (CheckInventory("SpeedBoosterActive") == 1) { TakeInventory("SpeedBoosterActive",1); delay(105); } // If you were successfully speed boosting just before, you can't just spam it again as soon as the last one finished.
        }
    }}
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(2);
    if (isDead(0)) { terminate; }
    delay(1);
    }
}

script METROID_SHIPCONTROLS (void)
{
    int shipbuttons;
    int AnglePre;
    int PitchPre;
    int AnglePost;
    int PitchPost;
    int AngleResult;

    Delay(2);
    if (!CheckInventory("IsAFuckingSpaceshipBoyeeee")) { terminate; }
    ConsoleCommand("chase");
    //if (ConsolePlayerNumber() != PlayerNumber()) { terminate; }
        
    while(1)
    {
        if (isDead(0)) { SetPlayerProperty(0,0,4); terminate; }

        shipbuttons = GetPlayerInput(-1,INPUT_BUTTONS);

        /*int Yaw = GetPlayerInput(-1,INPUT_YAW) * 500;
        int Pitch = GetPlayerInput(-1,INPUT_PITCH) * 500;

        if (Pitch > 0.2)
            { GiveInventory("GunshipLookUp",1); GiveInventory("LookUpCounter",1); TakeInventory("LookDownCounter",1); }
        else if (Pitch < -0.2)
            { GiveInventory("GunshipLookDown",1); GiveInventory("LookDownCounter",1); TakeInventory("LookUpCounter",1); }
        else
            { TakeInventory("LookUpCounter",1); TakeInventory("LookDownCounter",1); }

        if (Yaw > 0.2)
            { GiveInventory("GunshipLookRight",1); GiveInventory("LookRightCounter",1); TakeInventory("LookLeftCounter",2);  }
        else if (Yaw < -0.2)
            { GiveInventory("GunshipLookLeft",1); GiveInventory("LookLeftCounter",1); TakeInventory("LookRightCounter",2);  }
        else
            { TakeInventory("LookLeftCounter",2); TakeInventory("LookRightCounter",2); }

      AnglePre=getactorangle(0);
      PitchPre=getactorpitch(0);
      //Delay(1);
      if(AnglePre!=AnglePost)
      {
         AngleResult=AnglePre-((AnglePre-AnglePost)/5);
         
         if(AnglePre-AnglePost<(-50000)) //if the difference of the two is too big, and reached a big negative number
            SetActorAngle(0,65535);
         else if(AnglePre-AnglePost>50000)  //if the difference of the two is too big
            SetActorAngle(0,1);
         else
            SetActorAngle(0,AngleResult);
      }*/

      AnglePost=getactorangle(0);
      PitchPost=getactorpitch(0);

        if ( ((shipbuttons & BT_MOVELEFT)) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("GunshipLeft",1); }
                
        else if ( ((shipbuttons & BT_MOVERIGHT)) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("GunshipRight",1); }

        /*if( (shipbuttons & BT_ATTACK) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("SynthFireLeft",1); }
        else
            { TakeInventory("SynthFireLeft",0x7FFFFFFF); }
        if( (shipbuttons & BT_ALTATTACK) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("SynthFireRight",1); }
        else
            { TakeInventory("SynthFireRight",0x7FFFFFFF); }*/

        if( (shipbuttons & BT_JUMP) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("GunshipUp",1); }

        if( (shipbuttons & BT_Crouch) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("GunshipDown",1); }

        if( (shipbuttons & BT_FORWARD) && !(shipbuttons & BT_BACK) )
            { if (!(shipbuttons & BT_SPEED)) { GiveInventory("GunshipForward",1); GiveInventory("GoingForwardCounter",1); TakeInventory("GoingBackwardCounter",1); }
            else { GiveInventory("GunshipForwardDash",1); GiveInventory("GoingForwardCounter",1); TakeInventory("GoingBackwardCounter",2); }}
        else if( (shipbuttons & BT_BACK) && !(shipbuttons & BT_FORWARD) && !(shipbuttons & BT_SPEED) )
            { GiveInventory("GunshipBackward",1); GiveInventory("GoingBackwardCounter",1); TakeInventory("GoingForwardCounter",1); }
        else
            { TakeInventory("GoingForwardCounter",1); TakeInventory("GoingBackwardCounter",1); }

        Delay(1);
    }
}
