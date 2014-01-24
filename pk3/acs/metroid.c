#library "METROID"
#include "zcommon.acs"
#include "commonFuncs.h"

#include "met_const.h"
#include "met_spacejump.h"
#include "met_longbeam.h"

int SamusHealth[PLAYERMAX];
int playerOnFoot[PLAYERMAX];
//int IsServer = 0;




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
        ConsoleCommand("set metroid_nolevellimiter 0");
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

    while (1)
    {
        if (!GetCvar("compat_clientssendfullbuttoninfo")) { ConsoleCommand("set compat_clientssendfullbuttoninfo 1"); }
        if (!GetCvar("sv_chasecam")) { ConsoleCommand("set sv_chasecam 1"); }

        if (GetCVar("metroid_noaircontrol") == 0) { SetAirControl(0.225); }
        else if (GetCVar("metroid_noaircontrol") == 1) { SetAirControl(0.00390625); }

        if (GetCVar("metroid_spacejump") == 1) { if (CheckInventory("CanSpaceJump") == 0) { GiveInventory("CanSpaceJump",1); }}
        else if (GetCVar("metroid_spacejump") == 0) { if (CheckInventory("CanSpaceJump") == 1) { TakeInventory("CanSpaceJump",1); }}

        if (GetCVar("metroid_nomorph") == 1) { if (CheckInventory("DisableMorph") == 0) { GiveInventory("DisableMorph",1); }}
        else if (GetCVar("metroid_nomorph") == 0) { if (CheckInventory("DisableMorph") == 1) { TakeInventory("DisableMorph",1); }}

        if (GetCVar("metroid_loaded") == 1)
        {
            if (CheckInventory("Spazer Beam") == 0) { GiveInventory("Spazer Beam",1); }
            if (CheckInventory("Wave Beam") == 0) { GiveInventory("Wave Beam",1); }
            if (CheckInventory("Long Beam") == 0) { GiveInventory("Long Beam",1); }
            if (CheckInventory("Plasma Beam") == 0) { GiveInventory("Plasma Beam",1); }
            if (CheckInventory("Ice Beam") == 0) { GiveInventory("Ice Beam",1); }
        }

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
        if (GetCvar("metroid_cl_morphcamera") == 1)
        { ConsoleCommand("chase"); TakeInventory("PlayerMorphCamera",1); }
        break;

    case 1:
        if (GetCvar("metroid_cl_morphcamera") == 1)
        { ConsoleCommand("chase"); GiveInventory("PlayerMorphCamera",1); }
        break;

    case 2:
        delay(1);
        if (CheckInventory("PlayerMorphCamera") == 0)
        { if (GetCvar("metroid_cl_morphcamera") == 1)
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
        // Play sound...
        ActivatorSound("morphball/morph", 127);

        // Prepare for health transferring from old player to morphed player...
        SamusHealth[pNum] = GetActorProperty(0, APROP_HEALTH);
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
        Thing_ChangeTID(0, myTID);
        SetActivator(newTID);
        Thing_SetTranslation(myTID, -1);
        SetActivator(myTID);
        Thing_Remove(newTID);
        SetActorVelocity(0, velx,vely,velz, 0,0);

        if (isSinglePlayer() || isCoop()) { SetActorState(0,"CoopModeOn"); }

        // Transfer health and give inventory.
        SetActorProperty(0, APROP_HEALTH, SamusHealth[pNum]);
        GiveInventory("BorphMallAcquired", 1);
        TakeInventory("MorphBallActivate", 1);
        TakeInventory("IceBeamChilled",999);
        GiveInventory("MorphBallDeactivate", 1);

        TakeInventory("PowerBeamChargeLevel",999);
        //TakeInventory("PowerBeamCharged",999);
        TakeInventory("PowerBeamIdle",999);
        playerOnFoot[pNum] = 1;

        ACS_ExecuteAlways(METROID_MORPHCAMERA,0,0);
        ACS_ExecuteAlways(METROID_BWEEBWEEBWEEBWEE,0);
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
        GiveInventory("MorphBallActivate", 1);
        TakeInventory("MorphBallDeactivate", 1);
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
    TakeInventory("NotInThisLevel",999);
    TakeInventory("IceBeamChilled",999);

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

    if (CheckInventory("MorphBallDeactivate") == 1) { GiveInventory("MorphBallActivate", 1); TakeInventory("MorphBallDeactivate", 1); }
    ACS_ExecuteAlways(METROID_MORPHCAMERA,0,2);

    if (GameType () == GAME_NET_DEATHMATCH) { SetAmmoCapacity("MissileAmmo",10); GiveInventory("MissileAmmo",5); }
    if (isSinglePlayer() || isCoop()) { if (CheckInventory("CoopModeOn") == 0) { GiveInventory("CoopModeOn",1); SetActorState(0,"CoopModeOn"); }}

    ACS_ExecuteAlways(352,0,0,0); // Activates Space Jump mode.
    ACS_ExecuteAlways(351,0,0,0);

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

    while (1)
    {
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

        delay(1);
    }
}

script METROID_BWEEBWEEBWEEBWEE ENTER clientside
{
    if (PlayerNumber() != ConsolePlayerNumber()) { terminate; }
    while (1)
    {
    if (GetActorProperty(0,APROP_HEALTH) > 0) {
        if (GetActorProperty(0,APROP_HEALTH) <= 30) {
            if (GetCvar("metroid_cl_nosiren") == 0) {
                LocalAmbientSound("system/healthsiren",64); }}}
    delay(17);
    }
}

script METROID_RESPAWN RESPAWN
{
    ACS_ExecuteAlways(592,0);
    ACS_ExecuteAlways(METROID_BWEEBWEEBWEEBWEE,0);

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

script METROID_DECORATE (int which)
{
    switch (which)
    {
    case 0:
        if(GetCvar("metroid_cl_noeffects") == 1)
        setresultvalue(1);
        else setresultvalue(0);
        break;

    case 1:
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*3/4)); // 75% of the current player speed. Stackable.
        GiveInventory("IceBeamChilled",1);
        delay(105);
        TakeInventory("IceBeamChilled",1);
        delay(70);
        SetActorProperty(0,APROP_SPEED,1.00);
        break;

    case 2:
        SetPlayerProperty(0,1,PROP_TOTALLYFROZEN);
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*1/2)); // 50% of the current player speed. Stackable.
        GiveInventory("IceBeamChilled",1);
        delay(17);
        SetPlayerProperty(0,0,PROP_TOTALLYFROZEN);
        delay(123);
        TakeInventory("IceBeamChilled",1);
        delay(70);
        SetActorProperty(0,APROP_SPEED,1.00);
        break;

    case 3:
        if(GetCvar("sv_weaponstay") == 1)
        setresultvalue(1);
        else setresultvalue(0);
        break;

    case 4:
        if(GetCvar("sv_noweaponspawn") == 1)
        setresultvalue(1);
        else setresultvalue(0);
        break;

    case 5:
        //SetActorProperty(0,APROP_SPAWNHEALTH,(GetActorProperty(0,APROP_SPAWNHEALTH))+100);
        //delay(1);
        //SetActorProperty(0,APROP_HEALTH,(GetActorProperty(0,APROP_SPAWNHEALTH)));

        // Well that doesn't work. Guess we'll have to do this the hacky way.

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
        break;

    case 6:
        SetAmmoCapacity("MissileAmmo",GetAmmoCapacity("MissileAmmo")+5);
        delay(1);
        GiveInventory("MissileAmmo",5);
        break;

    case 7:
        if(GetCvar("metroid_nolevellimiter") == 0) { GiveInventory("NotInThisLevel",1); }
        break;

    case 8:
        SetActorProperty(0,APROP_SPEED,(GetActorProperty(0,APROP_SPEED)*1/2)); // 50% of the current monster's speed. Stackable.
        break;
    }
}

/*script METROID_DECORATE_CLIENT (int which) clientside
{
    switch (which)
    {
    case 0:
        if(GetCvar("metroid_cl_noeffects") == 1)
        setresultvalue(1);
        else setresultvalue(0);
        break;
    }
}*/
