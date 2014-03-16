function int parkmoreOnGround(int tid)
{
    int ctid = unusedTID(13000, 18000);
    int spawned = Spawn("ParkmoreFloorChecker", GetActorX(tid), GetActorY(tid), GetActorZ(tid)-4.0, ctid);
    if (spawned) { Thing_Remove(ctid); }

    return (onGround(tid) ||
        (GetActorVelZ(tid) == 0 && !spawned));
}

/* A note on 3D floors:
 *
 * The closest 3D floor above you in the sector you're in is considered the ceiling of the sector.
 * The closest 3D floor below you in the sector you're in is considered the floor of the sector.
 * Knowing this, we can determine three relationships:
 *
 * When changing Z height, but keeping X and Y constant:
 *  - If both ceiling and floor height change, we have skipped over a 3D floor entirely.
 *
 * When raising Z height ONLY:
 *  - If only the ceiling height changes, we have entered a 3D floor.
 *  - If only the floor height changes, we have left a 3D floor.
 *
 * When lowering Z height ONLY:
 *  - If only the ceiling height changes, we have left a 3D floor.
 *  - If only the floor height changes, we have entered a 3D floor.
 *
 * There is no way to differentiate between solid and non-solid 3D floors:
 *  at least, no way that I know of.
 */

function int parkmoreIn3DFloor(int tid)
{
    if (tid != 0 && ThingCount(0, tid) == 0) { return 0; }

    int x = GetActorX(tid);
    int y = GetActorY(tid);
    int f = GetActorFloorZ(tid);
    int c = GetActorCeilingZ(tid);
    int i = random(5, 22);
    int newtid = unusedTID(1000 * i, 1000 * (10+i));

    Spawn("ParkmoreChecker3", x, y, f, newtid);

    int ret = GetActorCeilingZ(newtid) < c;
    Thing_Remove(newtid);
    return ret;
}

function int hasHighJump(void)
{
    return CheckInventory("RuneHighJump") || CheckInventory("PowerHighJump");
}

function int getJumpZ(void)
{
    int ret = GetActorProperty(0, APROP_JumpZ);

    ret *= (hasHighJump() + 1);
    
    return ret;
}

function void wallBounce (int type, int direction) 
{
    int forwardMult, sideMult, xyMult, zMult;
    int forward, side, up;
    int forwardx, forwardy, sidex, sidey;
    int velx, vely, velz;

    int pln = PlayerNumber();
    int angle = GetActorAngle(0);

    if (type == WB_KICKUP && hasKicked[pln]) { return; }

    switch (type)
    {
        case WB_KICK:   xyMult =  0.45;  zMult = 0.75; break; // case WB_KICK:   xyMult =  0.8;  zMult = 1.0; break;
        case WB_KICKUP: xyMult =  0.02; zMult = 0.95; break; // case WB_KICKUP: xyMult =  0.02; zMult = 1.3; break;
        default: return;
    }

    xyMult = FixedMul(xyMult, GetActorProperty(0, APROP_Speed));
    zMult  = FixedMul(zMult,  GetActorProperty(0, APROP_JumpZ));

    if (hasHighJump())
    {
        xyMult = FixedMul(xyMult, 1.4142);
        zMult  = FixedMul(zMult, 1.4142);
    }

    switch (direction)
    {
        case WD_FORWARD:  forwardMult =  1.0;   sideMult =  0.0;    break;
        case WD_FORWRITE: forwardMult =  0.707; sideMult =  0.707;  break;
        case WD_RIGHT:    forwardMult =  0.0;   sideMult =  1.0;    break;
        case WD_BACKRITE: forwardMult = -0.707; sideMult =  0.707;  break;
        case WD_BACK:     forwardMult = -1.0;   sideMult =  0.0;    break;
        case WD_BACKLEFT: forwardMult = -0.707; sideMult = -0.707;  break;
        case WD_LEFT:     forwardMult =  0.0;   sideMult = -1.0;    break;
        case WD_FORWLEFT: forwardMult =  0.707; sideMult = -0.707;  break;
        case WD_KICK:     forwardMult = -1.0;   sideMult =  0.0;    break;
        default: return;
    }
    
    forward = FixedMul(WB_XYBASE, forwardMult); 
    side    = FixedMul(WB_XYBASE, sideMult); 

    up      = FixedMul(WB_ZBASE, zMult);

    forwardx = FixedMul(cos(angle), forward);
    forwardy = FixedMul(sin(angle), forward);
    sidex = FixedMul(cos(angle-0.25), side);
    sidey = FixedMul(sin(angle-0.25), side);
    
    velx = FixedMul(forwardx + sidex, xyMult);
    vely = FixedMul(forwardy + sidey, xyMult);
    velz = up;

    switch (type)
    {
        case WB_KICK:   SetActorVelocity(0, velx, vely, velz, 0, 1); break;
        case WB_KICKUP: SetActorVelocity(0, velx + GetActorVelX(0), GetActorVelY(0), velz, 0, 1); break;
    }

    playerJumps[pln] = min(playerJumps[pln]+1, 1);

    if (type == WB_KICK)
    {
        switch (direction)
        {
            case WD_FORWRITE: ACS_ExecuteAlways(METROID_TURN, 0, 45,  2, CLOCKWISE); break;
            case WD_RIGHT:    ACS_ExecuteAlways(METROID_TURN, 0, 90,  2, CLOCKWISE); break;
            case WD_BACKRITE: ACS_ExecuteAlways(METROID_TURN, 0, 135, 2, CLOCKWISE); break;
            case WD_BACK:     ACS_ExecuteAlways(METROID_TURN, 0, 180, 2, CLOCKWISE); break;
            case WD_BACKLEFT: ACS_ExecuteAlways(METROID_TURN, 0, 135, 2, COUNTERCLOCKWISE); break;
            case WD_LEFT:     ACS_ExecuteAlways(METROID_TURN, 0, 90,  2, COUNTERCLOCKWISE); break;
            case WD_FORWLEFT: ACS_ExecuteAlways(METROID_TURN, 0, 45,  2, COUNTERCLOCKWISE); break;
        }
    }

    if (type == WB_KICKUP) { hasKicked[pln] = 1; }
    else { hasKicked[pln] = 0; }

    grabbing[pln] = 0;
    dontGrab[pln] = 0;

    switch (type)
    {
        case WB_KICK: ActivatorSound("metroid/walljump", 127); break;
        case WB_KICKUP: ActivatorSound("metroid/wajumpup", 127); break;
    }
}

function void MultiJump(int countJump, int force)
{
    int pln = PlayerNumber();
    int jumpHeight = getJumpZ();

    if ((force != 1) && (MaxJumpCount >= 0) && (playerJumps[pln] + countJump > MaxJumpCount)) { return; }
    if (playerJumps[pln] == 0) { return; }

    playerJumps[pln] += countJump; 
    ActivatorSound("metroid/spacejump", 127);
    SetActorVelocity(0, GetActorVelX(0), GetActorVelY(0), jumpHeight, 0, 1);
}

function void Lunge(int force)
{
    int pln = PlayerNumber();
    int jumpHeight = FixedMul(getJumpZ(), LUNGE_ZMULT);
    int velx, vely, velz;
    int angle = GetActorAngle(0);

    if ((force != 1) && (playerJumps[pln] != 0)) { return; }

    playerJumps[pln] += 1; 

    velX = FixedMul(cos(angle), LUNGE_FORWARD);
    velY = FixedMul(sin(angle), LUNGE_FORWARD);
    velZ = jumpHeight;

    SetActorVelocity(0, velX, velY, velZ, 0, 1);
}

function int MetroidClientVars(void)
{
    //int switchOnPickup  = !!GetCVar("switchonpickup");
    //int weaponBar       = !!GetCVar("samsara_cl_weaponhud");
    //int ballgag         = !!GetCVar("samsara_cl_ballgag");
    int doomHealth        = !!GetCVar("metroid_cl_doomhealth");
    int runrunruu         = !!GetCVar("cl_run");

    return /*(switchOnPickup << 4) + (weaponBar << 3) + (ballgag << 2) +*/ (doomHealth << 1) + runrunruu;
}