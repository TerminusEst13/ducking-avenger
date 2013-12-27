// I have no idea what any of those code does
// so I guess I'll just copy/paste it all in here and hope nothing breaks.

// LONG BEAM SHIT
// ==================

#define TRAILCOUNT 3
int TrailItems[TRAILCOUNT]     = {"LasgunBurstItem", "LascannonBurstItem", "SuperNoclipBurstItem"};
int TrailStarts[TRAILCOUNT]    = {24, 24, 16};
int TrailDensities[TRAILCOUNT] = {24, 24, 16};
int TrailModes[PLAYERMAX];
int TempCoords[PLAYERMAX][3];

script 454 (int scaleI, int scaleF, int speedF)
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

script 455 (int which, int mode)
{
    int x, y, z;
    x = GetActorX(0); y = GetActorY(0); z = GetActorZ(0);
    SetActivatorToTarget(0);
    int pln = PlayerNumber();

    which = condFalse(which, 456);
    TrailModes[pln] = middle(0, mode, TRAILCOUNT-1);

    ACS_ExecuteAlways(which, 0, x,y,z);
    ACS_ExecuteWithResult(469, x,y,z); // For explosive trails
}

script 456 (int tx, int ty, int tz) clientside
{
    if (ConsolePlayerNumber() == -1) { terminate; }

    int i, k = 0, l;
    int ii, p;
    int x, y, z;
    int vx, vy, vz, mag, magI;
    int angle;

    angle = GetActorAngle(0);
    x  = GetActorX(0); y =  GetActorY(0);  z = GetActorZ(0) + 25.0;
    x += (12 * sin(angle));
    y -= (12 * cos(angle));

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = ftoi(mag);

    for (i = 48; i < magI; i += 24)
    {
        Spawn("LascannonBurstEffect", x+(vx*i), y+(vy*i), z+(vz*i));
    }
}

script 459 (int tx, int ty, int tz) clientside
{
    if (ConsolePlayerNumber() == -1) { terminate; }

    int i, k = 0, l;
    int x, y, z;
    int ii, p;
    int vx, vy, vz, mag, magI;
    int angle;

    angle = GetActorAngle(0);
    x  = GetActorX(0); y =  GetActorY(0);  z = GetActorZ(0) + 25.0;
    x += (12 * sin(angle));
    y -= (12 * cos(angle));

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = ftoi(mag);

    for (i = 48; i < magI; i += 16)
    {
        Spawn("LasgunTrail", x+(vx*i), y+(vy*i), z+(vz*i));
    }
}

script 469 (int tx, int ty, int tz)
{
    if (!IsServer) { terminate; }

    int pln = PlayerNumber();
    int item    = TrailItems     [TrailModes[pln]];
    int start   = TrailStarts    [TrailModes[pln]];
    int density = TrailDensities [TrailModes[pln]];
    int i, k = 0, l;
    int ii;
    int x, y, z;
    int vx, vy, vz, mag, magI;
    int angle;

    angle = GetActorAngle(0);
    x  = GetActorX(0); y =  GetActorY(0);  z = GetActorZ(0) + 25.0;
    x += (12 * sin(angle));
    y -= (12 * cos(angle));

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = ftoi(mag);

    for (i = start; i < magI; i += density)
    {
        TempCoords[pln][0] = (vx * i);
        TempCoords[pln][1] = (vy * i);
        TempCoords[pln][2] = (vz * i);
        GiveInventory(item, 1);
    }
}

script 468 (int which)
{
    int pln = PlayerNumber();
    int ret = 0;

    switch (which)
    {
        case 0: ret = TempCoords[pln][0]; break;
        case 1: ret = TempCoords[pln][1]; break;
        case 2: ret = TempCoords[pln][2]; break;
    }

    SetResultValue(ftoi(ret));
}