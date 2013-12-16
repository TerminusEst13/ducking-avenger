#library "METROID"
#include "zcommon.acs"
#include "commonFuncs.h"

#define C_TID		1000  //Default camera tid
#define MAX_R		128	  //Maximum radius (or distance from the player)
#define ADJUST_R	8	  //Amount to adjust the camera by
#define VIEW_HEIGHT	31.0  //The approximate hight of the player's view

bool cam_mode[8];         //Variable for turning the camera on or off.

int SamusHealth[PLAYERMAX];
int playerOnFoot[PLAYERMAX];
int IsServer = 0;




// CVAR HANDLING SHIT
// ==================

script 550 OPEN
{
    IsServer = 1;

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

    while (1)
    {
        if (GetCVar("metroid_noaircontrol") == 0) { SetAirControl(0.225); }
        else if (GetCVar("metroid_noaircontrol") == 1) { SetAirControl(0.00390625); }
        Delay(1);

        if (GetCVar("metroid_spacejump") == 1) { if (CheckInventory("CanSpaceJump") == 0) { GiveInventory("CanSpaceJump",1); }}
        else if (GetCVar("metroid_spacejump") == 0) { if (CheckInventory("CanSpaceJump") == 1) { TakeInventory("CanSpaceJump",1); }}
    }
}

script 551 OPEN clientside
{
    if (!GetCVar("metroid_cl_noeffects"))
    {
        ConsoleCommand("set metroid_cl_noeffects 0");
        ConsoleCommand("archivecvar metroid_cl_noeffects");
    }

    if (!GetCVar("metroid_cl_nocamerajerk"))
    {
        ConsoleCommand("set metroid_cl_nocamerajerk 0");
        ConsoleCommand("archivecvar metroid_cl_nocamerajerk");
    }
}


// MORPH BALL CAMERA SHIT
// ========================

Script 587 (int p_num)
{
	int r = MAX_R;
	
	while (cam_mode[p_num] == ON)
	{	
		int a = GetActorAngle (0);
		int p = GetActorPitch (0);
		int x = GetActorX (0);
		int y = GetActorY (0);
		int z = GetActorZ (0) + VIEW_HEIGHT;
		int xyr = r * cos (p) >> 16;
		
		if (!ThingCountName ("ChaseCam", C_TID+p_num))
		{
			while (!Spawn ("ChaseCam", x-cos(a)*xyr, y-sin(a)*xyr, z+sin(p)*r, C_TID+p_num, a >> 8) && r > 0)
			{
				r -= ADJUST_R;
				xyr = cos (p) * r >> 16;
			}
			
			if (ThingCountName ("ChaseCam", C_TID + p_num))
				ChangeCamera (C_TID + p_num, 0, 0);
			else
			{
				cam_mode[p_num] = OFF;
				print (s:"Camera script failed to initialize.");
			}
		}
	        else
		{
			while (!SetActorPosition (C_TID+p_num, x-cos(a)*xyr, y-sin(a)*xyr, z+sin(p)*r, 0) && r > 0)
			{
				r -= ADJUST_R;
				xyr = cos (p) * r >> 16;
			}
			
			SetActorAngle (C_TID + p_num, a);
			SetActorPitch (C_TID + p_num, p);
			
			if (r < MAX_R) 
                              r += ADJUST_R;
		}
		
		delay (1);
	}
}


// MORPH BALL SHIT
// =================

script 588 (int morphshit)
{
    int pNum = PlayerNumber();
    int CheckerTID = 1500+pNum;	

    switch (morphshit)
    {
    case 0:
        // Play sound...
        ActivatorSound("morphball/morph", 127);

        // Prepare for health transferring from old player to morphed player...
        SamusHealth[pNum] = GetActorProperty(0, APROP_HEALTH);
		int newTID = unusedTID(23000, 25000);
		int myTID  = defaultTID(-1);

        // Transfer the player's translation from the old body to the new morph.
		Spawn("TranslationHolder", GetActorX(0), GetActorY(0), GetActorZ(0)+8.0, newTID);
		Thing_SetTranslation(newTID, -1);
		MorphActor(0, "MorphBallPlayer", "", 0x7FFFFFFF, 194, "emptytelefog", "emptytelefog");
		Thing_ChangeTID(0, myTID);
		SetActivator(newTID);
		Thing_SetTranslation(myTID, -1);
		SetActivator(myTID);
		Thing_Remove(newTID);

        // Transfer health and give inventory.
        SetActorProperty(0, APROP_HEALTH, SamusHealth[pNum]);
        GiveInventory("BorphMallAcquired", 1);
        TakeInventory("MorphBallActivate", 1);
        GiveInventory("MorphBallDeactivate", 1);

        TakeInventory("PowerBeamChargeLevel",999);
        //TakeInventory("PowerBeamCharged",999);
        TakeInventory("PowerBeamIdle",999);
        playerOnFoot[pNum] = 1;

        // And set the camera
        cam_mode[PlayerNumber ()] = ON;
        ACS_ExecuteAlways (587, 0, PlayerNumber ());
        break;

    case 1:
        if (Spawn("SpaceChecker", GetActorX(0), GetActorY(0), GetActorZ(0), CheckerTID, GetActorAngle(0) >> 8 ) > 0)
        {
        ActivatorSound("morphball/unmorph", 127);
        SamusHealth[pNum] = GetActorProperty(0, APROP_HEALTH);
        //Thing_Remove(CheckerTID);
        UnmorphActor(0, 1);
        SetActorProperty(0, APROP_HEALTH, SamusHealth[pNum]);
        TakeInventory("BorphMallAcquired", 1);
        GiveInventory("MorphBallActivate", 1);
        TakeInventory("MorphBallDeactivate", 1);
        playerOnFoot[pNum] = 0;

        cam_mode[PlayerNumber ()] = OFF;
        Thing_Remove (C_TID + PlayerNumber ());
        Thing_Remove(CheckerTID);
        }
        else
        { ActivatorSound("morphball/denied", 127); }
        break;
    }
}

// ENTER EXIT DEATH RESPAWN SHIT
// ========================

script 589 UNLOADING
{
    // Removes Morph Ball camera
	cam_mode[PlayerNumber ()] = OFF;
	Thing_Remove (C_TID + PlayerNumber ());

    // Removes Morph Ball Inventory
    TakeInventory("BorphMallAcquired",999);
    TakeInventory("PowerBeamChargeLevel",999);
    TakeInventory("PowerBeamCharged",999);
    TakeInventory("PowerBeamIdle",999);
}

script 590 DEATH { ACS_ExecuteAlways(589,0); }

Script 591 (int p_num) DISCONNECT // Somehow this is different from just the Death script.
{
       cam_mode[p_num] = OFF;
       Thing_Remove (C_TID + p_num);
}

script 592 ENTER
{
    if (CheckInventory("MorphBallDeactivate") == 1) { 
        GiveInventory("MorphBallActivate", 1);
        TakeInventory("MorphBallDeactivate", 1); }
}

script 593 RESPAWN { ACS_ExecuteAlways(589,0); }


// DECORATE CHECKS
// =====================

script 594 (int which)
{
    switch (which)
    {
    case 0:
        if(GetCvar("metroid_cl_noeffects") == 1)
		setresultvalue(1);
		else setresultvalue(0);
		break;
    }
}

/*script 595 (int which) clientside
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