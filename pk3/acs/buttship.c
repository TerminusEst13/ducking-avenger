#include "zcommon.acs"
#library "buttship"

#include "commonFuncs.h"

// Pretty much this entire thing was coded by ijon tichy.
// Yes, including the comments.

// Clearly this work is wasted on me. The project should be given to him.

int CL_ShipTIDs[PLAYERMAX];     // BUTT SHIP TIDS SON
int CL_ShipScripts[PLAYERMAX];  // WE TWERKING THE SEVEN SEAS YET
int CL_PlayerTIDS[PLAYERMAX];   // duh

script 173 (void)//enter
{
    int pln = PlayerNumber();
    int tid;
    int time = 0;

    while (1)
    {
        if (!isDead(0)) { tid = defaultTID(-1); }

        if (time % 18 == 0)
        {
            ACS_ExecuteAlways(174, 0, 0, tid);
            time %= 18;
        }

        time++;
        Delay(1);
    }
}

script 174 (int which, int a1, int a2) clientside
{
    int pln = PlayerNumber();
    int testtid = unusedTID(38000, 48000);

    switch (which)
    {
      case 0:
        CL_PlayerTIDs[pln] = a1;
        Thing_ChangeTID(0, a1);

        // GET THAT BUTT SHIP ROLLING
        ACS_ExecuteWithResult(175, pln, 0);
        break;
    }
}

script 175 (int pln, int nocountdeath) clientside
{
    if (CL_ShipScripts[pln]) { terminate; }
    CL_ShipScripts[pln] = 1;

    SetActivator(-1);
    int shipTID = -1;
    int mytid;
    int i, x, y, z, a, vx, vy, vz;
    int reloop;
    int nx, ny, nz;
    int height;

    while (PlayerInGame(pln))
    {
        mytid = CL_PlayerTIDs[pln];

        if (isDead(mytid) && Timer() >= nocountdeath)
        {
            // BUTT SHIP'S DOWN, WHAT THE HELL
            Thing_Remove(CL_ShipTIDs[pln]);
            // PREPARE YOUR ANUS FOR MOURNING
            CL_PlayerTIDs[pln] = 0;
            // RIP BUTT SHIP ;_;7
            CL_ShipTIDs[pln] = 0;
            // YOU DIED AS YOU LIVED
            terminate;
            // THE OPPOSITE SIDE OF A PENIS
        }
        
        x = GetActorX(mytid);
        y = GetActorY(mytid);
        z = GetActorZ(mytid);
        a = GetActorAngle(mytid);
        vx = GetActorVelX(mytid);
        vy = GetActorVelY(mytid);
        vz = GetActorVelZ(mytid);

        // WE GOT NO BUTT SHIP, WE GOTTA FIX THAT SHIT
        if (ThingCount(0, shipTID) <= 0)
        {
            if (ThingCount(0, shipTID) > 0 && shipTID != 0) { Thing_Remove(shipTID); }

            shipTID = unusedTID(12000, 15000);
            while (1)
            {
                reloop = 0;

                // WAIT HOLD THE FUCK UP WE CAN'T BE CONFUSING BUTT SHIPS
                for (i = 0; i < PLAYERMAX; i++)
                {
                    if (CL_PlayerTIDs[i] == shipTID || CL_ShipTIDs[i] == shipTID || ThingCount(0, shipTID) > 0)
                    {
                        shipTID++;
                        reloop = 1;
                        break;
                    }
                }
                
                if (!reloop) { break; }
            }

            // TOOT TOOT THE BUTT SHIP HAS ARRIVED
            Spawn("GunshipHat", x,y,z, shipTID, a);
        }

        // YOUR COLORS ARE NEAT, AND THE BUTT SHIP'S STEALIN' EM
        if (1)
        {
            SetActivator(mytid);
            Thing_SetTranslation(shipTID, -1);
            SetActivator(-1);
        }

        CL_ShipTIDs[pln] = shipTID;

        // THE BUTT SHIP IS STEALTHY
        SetActorProperty(shipTID, APROP_Alpha, GetActorProperty(mytid, APROP_Alpha));

        // WOOP WOOP PULL OVER DAT ASS TOO FAT
        // (angle's factored in; pitch isn't)
        int xOffset = 0, yOffset = 0, zOffset = 0;

        nx = x + FixedMul(xOffset, cos(a)) + FixedMul(yOffset, sin(a));
        ny = y + FixedMul(xOffset, sin(a)) - FixedMul(yOffset, cos(a));
        nz = z + zOffset;

        if (pln != ConsolePlayerNumber())
        {
            // HOLD ON BACK THAT BUTT SHIP ON UP
            nx -= vx; ny -= vy; nz -= vz;
        }
        else
        {
            // NOT FAR ENOUGH, KEEP BACKIN IT UP
            // (players have weird camera interpolation re. Z movement)
            nx -= vx; ny -= vy; nz -= 2*vz;
        }

        //SetActorAngle(shipTID, a);
        if (GetActorAngle(mytid) != GetActorAngle(shipTID))
        { //Print(s:"Yo, shit's uneven!");
          if (GetActorAngle(mytid) < GetActorAngle(shipTID)) { SetActorAngle(shipTID, GetActorAngle(shipTID) - 0.005); }
          if (GetActorAngle(mytid) > GetActorAngle(shipTID)) { SetActorAngle(shipTID, GetActorAngle(shipTID) + 0.005); }}
        SetActorPosition(shipTID, nx, ny, nz, 0);
        SetActorVelocity(shipTID, vx, vy, vz, 0, 0);
        
        Delay(1);
    }

    CL_ShipScripts[pln] = 0;
}

script 176 (int pln) disconnect clientside
{
    Thing_Remove(CL_ShipTIDs[pln]);
    CL_PlayerTIDs[pln] = 0;
    CL_ShipTIDs[pln] = 0;
}