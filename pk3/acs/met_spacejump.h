int playerTimers[PLAYERMAX][TIMER_COUNT];

/*  :TURNING
 * Turning scripts
 */

function int getTimer(int pln, int which)
{
    return playerTimers[pln][which];
}

function void addTimer(int pln, int which, int add)
{
    if (add) { playerTimers[pln][which] = add; }
}

function int tickTimer(int pln, int timerNum)
{
    int i = max(playerTimers[pln][timerNum]-1, 0);
    playerTimers[pln][timerNum] = i;
    return i;
}

function void tickTimers(int pln)
{
    int i;
    for (i = 0; i < TIMER_COUNT; i++)
    {
        tickTimer(pln, i);
    }
}

function void printTimers(int pln)
{
    int i, j, printstr;
    for (i = 0; i < TIMER_COUNT; i++)
    {
        j = playerTimers[pln][i];
        printStr = StrParam(s:printStr, d:i, s:":", d:!!j, s:"  ");
    }

    Print(s:printStr);
}

script METROID_TURN (int degrees, int factor, int direction) net clientside
{
    if (degrees < 0)
    {
        degrees *= -1;
        direction = cond(direction == CLOCKWISE, COUNTERCLOCKWISE, CLOCKWISE);
    }
    
    factor = cond(factor, factor, 4);

    int prevDegrees, addDegrees, curAngle;
    int curDegrees = 0;
    int floatDegrees = itof(degrees);
    int dirMult = cond(direction == CLOCKWISE, -1, 1);

    while (curDegrees < (floatDegrees - 0.1))
    {
        prevDegrees = curDegrees;
        addDegrees = (floatDegrees - curDegrees) / factor;
        curDegrees += addDegrees;

        SetActorAngle(0, GetActorAngle(0) + ((addDegrees * dirMult) / 360)); //}
        Delay(1);
    }

    addDegrees = floatDegrees - curDegrees;

    SetActorAngle(0, GetActorAngle(0) + ((addDegrees * dirMult) / 360));
}

/*  :MOVEMENT
 * Wall-jumping, dodging scripts
 */

script METROID_WALLBOUNCE (int type, int direction, int mask)
{
    int newDir = -1;
    int justCheck;
    int angle, x,y,z, x2,y2, i, j;
    int tid, canBounce;
    int sideMove, forwMove;
    int x3,y3;

    if (isDead(0)) { terminate; }

    if (direction < 0) { direction = -direction; justCheck = 1; }

    if (parkmoreOnGround(0))
    {
        if (type == WB_DODGE) { wallBounce(type, direction); }
        terminate;
    }

    if (mask == 0) { mask = 1; }

    angle = GetActorAngle(0) + AngleOffsets[direction];
    angle = mod(angle, 1.0);

    x  = 20 * cos(angle); y  = 20 * sin(angle);
    x2 = 8  * cos(angle); y2 = 8  * sin(angle);

    if (abs(x) > abs(y))
    {
        y = FixedMul(y, FixedDiv(20.0, abs(x)));
        x = 20.0 * sign(x);
    }
    else
    {
        x = FixedMul(x, FixedDiv(20.0, abs(y)));
        y = 20.0 * sign(y);
    }

    canBounce = 0;
    tid = unusedTID(25000, 30000);

    for (i = 0; i < 10; i++)
    {
        x3 = GetActorX(0) + x + (x2*i);
        y3 = GetActorY(0) + y + (y2*i);
        z  = GetActorZ(0) + 16.0;

        j = Spawn("ParkmoreChecker2", x3, y3, z, tid);
        if (!j) { canBounce = 1; }
        if (parkmoreIn3DFloor(tid)) { canBounce = 1; }

        Thing_Remove(tid);

        //PrintBold(s:"(", f:x3, s:", ", f:y3, s:") type ", d:type, s:" - ", d:j);
        
        if (canBounce) { break; }
    }
    
    if (canBounce && !justCheck)
    {
        wallBounce(type, direction);
    }

    //Print(s:"wallbounce (func): ", d:canBounce, s:" - angle is ", f:angle, s:", direction is ", d:direction, s:" (", f:x, s:", ", f:y, s:")");
    SetResultValue(canBounce);
}

script METROID_SPACEJUMP_ENTER enter
{
    int pln = PlayerNumber();
    int ground, wasGround, didSpecial;
    int inWater, wasInWater;
    int i;
    int direction, dDirection;

    while (PlayerInGame(pln))
    {
        if (isDead(0))
        {
            playerJumps[pln] = 0;
            hasKicked[pln] = 0;
            grabbing[pln] = 0;
            dontGrab[pln] = 0;
            TakeInventory("HasKicked", 1);
            Delay(1);
            continue;
        }

        if (hasKicked[pln] && !CheckInventory("HasKicked")) { GiveInventory("HasKicked", 1); }
        if (!hasKicked[pln] && CheckInventory("HasKicked")) { TakeInventory("HasKicked", 0x7FFFFFFF); }

        wasGround = ground;
        ground = parkmoreOnGround(0);

        PlayerGrounds[pln][0] = ground;
        PlayerGrounds[pln][1] = wasGround;

        wasInWater = inWater;
        inWater = CheckInventory("WaterIndicator");

        direction = getDirection();

        //if (CheckInventory("NoParkour") && !CheckInventory("ForceParkourOff"))
        if (!CheckInventory("CanSpaceJump"))
        {
            if (ground)
            {
                playerJumps[pln] = 0;
                hasKicked[pln] = 0;
                grabbing[pln] = 0;
                dontGrab[pln] = 0;
            }
            else
            {
                playerJumps[pln] = max(1, playerJumps[pln]);
            }
            Delay(1);
            continue;
        }

        didSpecial = 0;

        if (ground || inWater)
        {
            playerJumps[pln] = 0;
            hasKicked[pln] = 0;
            grabbing[pln] = 0;
            dontGrab[pln] = 0;
        }
        else
        {
            playerJumps[pln] = max(1, playerJumps[pln]);
        }

        tickTimer(pln, TIMER_BOUNCED);
        DidSpecials[pln] = max(0, DidSpecials[pln]-1);

        Delay(1);
    }
}

script METROID_SPACEJUMP_ENTER2 enter clientside
{
    int pln = PlayerNumber();
    int dodgeDir, pukeStr;
    int ground, wasGround, direction, dDirection;
    int inWater, wasInWater;
    int myLock = ClientEnterLocks[pln] + 1;
    int i, j, k;

    if (ConsolePlayerNumber() != pln && !IsServer) { terminate; }

    ClientEnterLocks[pln] = myLock;

    while (ClientEnterLocks[pln] == myLock)
    {
        dodgeDir = -1;
        dDirection = -1;

        ground = parkmoreOnGround(0);
        direction = getDirection();

        if (ground) { wasGround = MJUMP_DELAY; }
        else { wasGround = max(0, wasGround-1); }

        wasInWater = inWater;
        inWater = CheckInventory("WaterIndicator");

        CPlayerGrounds[pln][0] = ground;
        CPlayerGrounds[pln][1] = wasGround;

        if (!(GetActorVelX(0) || GetActorVelY(0)))
        {
            playerTimers[pln][TIMER_CLEFT]      = 0;
            playerTimers[pln][TIMER_CFORWARD]   = 0;
            playerTimers[pln][TIMER_CRIGHT]     = 0;
            playerTimers[pln][TIMER_CBACK]      = 0;
        }

        if (!(getTimer(pln, TIMER_BOUNCED) || wasGround) && keyPressed(BT_JUMP) && direction != 0 && CheckInventory("CanSpaceJump") && !CheckInventory("BorphMallAcquired"))
        {
            switch (direction)
            {
              case DIR_NW: dDirection = WD_FORWLEFT;    break;
              case DIR_N:  dDirection = WD_FORWARD;     break;
              case DIR_NE: dDirection = WD_FORWRITE;    break;
              case DIR_SW: dDirection = WD_BACKLEFT;    break;
              case DIR_S:  dDirection = WD_BACK;        break;
              case DIR_SE: dDirection = WD_BACKRITE;    break;
              case DIR_W:  dDirection = WD_LEFT;        break;
              case DIR_E:  dDirection = WD_RIGHT;       break;
            }

            i = 0;
            switch (dDirection)
            {
              case -1: break;

              case WD_FORWARD:
                if (!CheckInventory("HasKicked")
                    && ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICKUP, -WD_KICK)) 
                {
                    i = 2; break;
                }
            
              default:
                if (ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICK, -dDirection)) { i = 1; }
                break;
            }

            if (i == 2)
            {
                if (!IsServer)
                {
                    pukeStr = StrParam(s:"puke -", d:METROID_REQUESTDODGE, s:" ", d:-256);
                    if (GetCVar("cyber_cl_debug")) { Print(s:"should be wall kick: ", s:pukeStr); }
                    ConsoleCommand(pukeStr);
                }
                else
                {
                    ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICKUP, WD_KICK);
                }
            }
            else if (i == 1)
            {
                if (!IsServer)
                {
                    pukeStr = StrParam(s:"puke -", d:METROID_REQUESTDODGE, s:" ", d:-dDirection);
                    if (GetCVar("cyber_cl_debug")) { Print(s:"should be wall jump: ", s:pukeStr); }
                    ConsoleCommand(pukeStr);
                }
                else
                {
                    ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICK, dDirection);
                }
            }
            else { dDirection = -1; }

            
            if (i) { addTimer(pln, TIMER_BOUNCED, 2); }
            //Print(s:"walljump: ", d:i, s:" (", d:dDirection, s:")");
        }

        if (keyPressed(BT_JUMP) && CheckInventory("CanSpaceJump") && !CheckInventory("BorphMallAcquired"))
        {
            if (!(ground || (GetActorVelZ(0) < 0 && wasGround) || wasGround >= (MJUMP_DELAY-2) || inWater || dDirection != -1))
            {
                if (!IsServer)
                {
                    pukeStr = StrParam(s:"puke -", d:METROID_REQUESTDODGE, s:" 0 0 1");
                    if (GetCVar("cyber_cl_debug")) { Print(s:"should be multijump: ", s:pukeStr); }
                    ConsoleCommand(pukeStr);
                }
                else if (!DidSpecials[pln] && !grabbing[pln])
                {
                    MultiJump(1, 0);
                }
            }
        }

        tickTimer(pln, TIMER_CFORWARD);
        tickTimer(pln, TIMER_CRIGHT);
        tickTimer(pln, TIMER_CBACK);
        tickTimer(pln, TIMER_CLEFT);
        tickTimer(pln, TIMER_HBACK);

        tickTimer(pln, TIMER_DIDDODGE);
        tickTimer(pln, TIMER_BOUNCED);

        Delay(1);
    }

}

script METROID_REQUESTDODGE (int direction, int hijump, int mjump) net
{
    int pln = PlayerNumber();

    if (GetCVar("cyber_cl_debug"))
    {
        PrintBold(s:"Activator is ", n:0, s:"\c- (idx ", d:pln, s:")\nExecuted on tic ", d:Timer(), s:"\nArgs: ", d:direction, s:", ", d:hijump, s:", ", d:mjump);
    }

    if (isDead(0)) { terminate; }

    if (mjump && !DidSpecials[pln] && !grabbing[pln])
    {
        //Print(d:playerJumps[pln]);
        MultiJump(1, 0);
    }
    else if (direction < 0)
    {
        direction = -direction;
        if (direction == 256) { ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICKUP, WD_KICK); }
        else { ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_KICK, direction); }
    }
    else
    {
        ACS_ExecuteWithResult(METROID_WALLBOUNCE, WB_DODGE, direction, 0);
    }
}