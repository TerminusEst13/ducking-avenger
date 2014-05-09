script 474 (int which, int transferVelocity, int count) clientside
{
    if (M_GetCVar("metroid_cl_noeffects")) { terminate; } // ramon pls
    if (which < 0 || which >= PARTICLECOUNT) { terminate; }
    int particle = DakkaBeamParticles[which];
    int fadeRate = DakkaBeamFadeRates[which];
    int velFactor = percFloat(0, transferVelocity);
    count = max(1, count);

    int nx, ny, nz;
    int i, a;

    while (!CheckInventory("QuitParticlan") && !(ClassifyActor(0) & ACTOR_WORLD))
    {
        int partTID = unusedTID(12000, 14000);
        int velX = GetActorVelX(0);
        int velY = GetActorVelY(0);
        int velZ = GetActorVelZ(0);
        int mag  = magnitudeThree_f(velX, velY, velZ);
        int x = GetActorX(0);
        int y = GetActorY(0);
        int z = GetActorZ(0);

        for (i = 0; i < count; i++)
        {
            nx = x - ((velX * i) / count);
            ny = y - ((velY * i) / count);
            nz = z - ((velZ * i) / count);

            if (!Spawn(particle, nx,ny,nz, partTID, GetActorAngle(0))) { continue; }

            SetActorPitch(partTID, GetActorPitch(0));
            Thing_SetTranslation(partTID, -1);

            a = GetActorProperty(partTID, APROP_Alpha);
            a -= (fadeRate * i) / count;
            SetActorProperty(partTID, APROP_Alpha, a);

            SetActorVelocity(partTID, FixedMul(velx, velFactor),
                                      FixedMul(vely, velFactor),
                                      FixedMul(velz, velFactor), 0, 0);
            
            Thing_ChangeTID(partTID, 0);
        }

        Delay(1);
    }
}

int SoundSlotIndex[PLAYERMAX];

script 475 (int which, int a1, int a2)
{
    int ret = 0;
    int pln = PlayerNumber();
    int x, y, z, vx, vy, vz, mag, nx, ny, nz;
    int color;
    int i, tid;
    int radius, offset;

    switch (which)
    {
      case 0:
        if (!a2) { a2 = BT_ATTACK; }
        if (!keyDown(a2)) { break; }

        int level = CheckInventory("DakkaBeamChargeLevel");
        int firetime = DakkaBeamFireTimes[middle(0, level, CHARGELEVELS-1)];
        if (CheckInventory("IceBeamChilled")) { firetime = ceil(firetime * 1.25); }

        if (a1 >= firetime) { ret = 1; }
        break;

      case 1:
        if (a1 == 0) { a1 = 4; }
        if (a2 == 0) { a2 = 7; }
        i = (a2 - a1) + 1;
        ret = (SoundSlotIndex[pln]++ % i) + a1;
        break;
      
      // HACK TIME!
      //  Clip is used to indicate this has run already.
      //  Shell is used to store starting X position.
      //  RocketAmmo, Y position.
      //  Cell, Z position.
      //  This is used for the FUCKING LASERS.
      case 2:
        if (GetAmmoCapacity("Clip") == -1) { break; }

        x = GetActorX(0);
        y = GetActorY(0);
        z = GetActorZ(0);

        x -= ACS_ExecuteWithResult(475, 6, 0);
        y -= ACS_ExecuteWithResult(475, 6, 1);
        z -= ACS_ExecuteWithResult(475, 6, 2);

        vx = GetActorVelX(0);
        vy = GetActorVelY(0);
        vz = GetActorVelZ(0);

        mag = magnitudeThree_f(vx, vy, vz);
        nx  = FixedDiv(vx, mag);
        ny  = FixedDiv(vy, mag);
        nz  = FixedDiv(vz, mag);

        SetAmmoCapacity("Clip", -1);
        SetAmmoCapacity("Shell",        x);
        SetAmmoCapacity("RocketAmmo",   y);
        SetAmmoCapacity("Cell",         z);

        //PrintBold(s:"Tic ", d:Timer(), s:" - (", f:nx, s:", ", f:ny, s:", ", f:nz, s:")");
        
        SetActorVelocity(0, nx * 32767, ny * 32767, nz * 32767, 0, 0);
        break;

      case 3:
        //PrintBold(s:"help0");
        if (GetAmmoCapacity("Clip") != -1) { break; }
        //PrintBold(s:"help1");

        color = a1;

        //PrintBold(s:"help2");

        x = ACS_ExecuteWithResult(475, 6, 0);
        y = ACS_ExecuteWithResult(475, 6, 1);
        z = ACS_ExecuteWithResult(475, 6, 2);

        x += GetAmmoCapacity("Shell");
        y += GetAmmoCapacity("RocketAmmo");
        z += GetAmmoCapacity("Cell");

        nx = GetActorX(0);
        ny = GetActorY(0);
        nz = GetActorZ(0);

        vx = ACS_ExecuteWithResult(475, 6, 3);
        vy = ACS_ExecuteWithResult(475, 6, 4);
        vz = ACS_ExecuteWithResult(475, 6, 5);

        ACS_ExecuteAlways(484, 0, packShorts(round(x), round(nx)),
                                  packShorts(round(y), round(ny)),
                                  packShorts(round(z), round(nz)));

        ACS_ExecuteAlways(486, 0, vx, vy, vz);

        ACS_ExecuteWithResult(489, x,y,z);
        break;
       
      case 6:
        SetActivatorToTarget(0);

        switch (a1)
        {
            case 0: ret = GetActorX(0); break;
            case 1: ret = GetActorY(0); break;
            case 2: ret = GetActorZ(0); break;

            case 3: ret = GetActorVelX(0); break;
            case 4: ret = GetActorVelY(0); break;
            case 5: ret = GetActorVelZ(0); break;
        }
        break;

      case 7:
        i = percFloat(a1, a2);

        x = FixedMul(GetActorVelX(0), i);
        y = FixedMul(GetActorVelY(0), i);
        z = FixedMul(GetActorVelZ(0), i);

        SetActorVelocity(0, x,y,z, 0,0);
        break;

      case 8:
        tid = defaultTID(-1);
        ret = GetActorViewHeight(tid) - 41.0;
    }

    SetResultValue(ret);
}

script 477 (int which, int a1, int a2) clientside
{
    int pln = PlayerNumber();

    switch (which)
    {
      case 0:
        GiveInventory("QuitParticlan", 1);
        break;
    }
}

int MissileCheckStates[PLAYERMAX];

script 476 (int begin, int button, int superThreshold)
{
    begin = !!begin;
    superThreshold = condFalse(superThreshold, 26);
    button = condFalse(button, BT_ALTATTACK);

    int pln = PlayerNumber();
    if (pln == -1) { terminate; }
    int curstate = MissileCheckStates[pln];
    if (curstate == begin) { terminate; }

    MissileCheckStates[pln] = begin;

    int chargelevel = 0;
    int couldsuper, cansuper = 0, givesuper = 0, canmissile = 0, isfiring = 0;

    while (MissileCheckStates[pln])
    {
        // There's a dirty joke somewhere in here.
        if (CheckInventory("BorphMallAcquired") || CheckInventory("SpeedBoosterActive"))
        {
            MissileCheckStates[pln] = 0;
            break;
        }

        cansuper    = CheckInventory("SuperMissileAcquired") && (CheckInventory("SuperMissileAmmo") || GetCVar("sv_infiniteammo"));
        canmissile  = CheckInventory("MissileAmmo")      || GetCVar("sv_infiniteammo");
        isfiring    = keyDown(button);

        // Clear out the trigger if we can hold for super missiles
        // - if we only have normal missiles, this could waste bandwidth

        if (cansuper && CheckInventory("Metroid_CanMissile"))
        {
            TakeInventory("Metroid_CanMissile", 0x7FFFFFFF);
        }

        // chargan
        if (isfiring && cansuper) { chargelevel++; }
        else
        {
            // Let go of the trigger before charged? fire a normal missile
            if (chargelevel > 0 && canmissile && cansuper && !couldsuper)
            {
                GiveInventory("Metroid_CanMissile", 1);
            }
                
            chargelevel = 0;
        }

        givesuper   = chargelevel >= superThreshold;
        givesuper  &= cansuper;

        // Handle no-super-missiles here
        if (!cansuper)
        {
            if (isfiring && canmissile && !CheckInventory("Metroid_CanMissile")) { GiveInventory("Metroid_CanMissile", 1); }
            else if (CheckInventory("Metroid_CanMissile")) { TakeInventory("Metroid_CanMissile", 0x7FFFFFFF); }
        }

        // LOCKED AND LOADED
        if (givesuper && !couldsuper)
        {
            ActivatorSound("supermissile/ready", 127);
        }

        // If we let go of the trigger, either send the super missile signal
        //   (if charged), or clear the signal if there (if not)
        if (!givesuper)
        {
            if (couldsuper) { GiveInventory("Metroid_CanSuper", 1); }
            else if (CheckInventory("Metroid_CanSuper")) { TakeInventory("Metroid_CanSuper", 0x7FFFFFFF); }
        }

        couldsuper  = givesuper;
        Delay(1);
    }

    if (CheckInventory("Metroid_CanMissile")) { TakeInventory("Metroid_CanMissile", 0x7FFFFFFF); }
    if (CheckInventory("Metroid_CanSuper")) { TakeInventory("Metroid_CanSuper", 0x7FFFFFFF); }
}

script 478 enter
{
    int infinite;

    while (!(ClassifyActor(0) & ACTOR_WORLD))
    {
        infinite = GetCVar("sv_infiniteammo");

        if (infinite && !CheckInventory("HasInfiniteAmmo")) { GiveInventory("HasInfiniteAmmo", 1); }
        else if (!infinite && CheckInventory("HasInfiniteAmmo")) { TakeInventory("HasInfiniteAmmo", 0x7FFFFFFF); }

        Delay(1);
    }
}


// LASERS MOTHERFUCKER

int CB_TempCoords[PLAYERMAX][3];
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

script 484 (int x, int y, int z) clientside
{
    int nx = itof(rightShort(x));
    int ny = itof(rightShort(y));
    int nz = itof(rightShort(z));
    x = itof(leftShort(x));
    y = itof(leftShort(y));
    z = itof(leftShort(z));

    Hack_StartX = x;
    Hack_StartY = y;
    Hack_StartZ = z;
    Hack_ArgsSet[0] = 1;
    Hack_ArgsSet[1] = 1;
    Hack_ArgsSet[2] = 1;
    Hack_EndX = nx;
    Hack_EndY = ny;
    Hack_EndZ = nz;
    Hack_ArgsSet[3] = 1;
    Hack_ArgsSet[4] = 1;
    Hack_ArgsSet[5] = 1;

    ACS_ExecuteWithResult(487);
}

script 486 (int x, int y, int z) clientside
{
    int i;

    // this case gets called first because ACS_ExecuteAlways is LIFO, so reset here
    for (i = 0; i < 9; i++) { Hack_ArgsSet[i] = 0; }

    Hack_VelX = x;
    Hack_VelY = y;
    Hack_VelZ = z;
    Hack_ArgsSet[6] = 1;
    Hack_ArgsSet[7] = 1;
    Hack_ArgsSet[8] = 1;

    ACS_ExecuteWithResult(487);
}

script 487 (void) clientside
{
    if (ConsolePlayerNumber() == -1) { terminate; }

    int i, run = 1;

    for (i = 0; i < 9; i++)
    {
        if (!Hack_ArgsSet[i])
        {
            run = 0;
            break;
        }
    }

    if (!run) { terminate; }
    for (i = 0; i < 9; i++) { Hack_ArgsSet[i] = 0; }

    int pln = PlayerNumber();
    int noeffects = M_GetCVar("metroid_cl_noeffects");
    int density   = middle(1, M_GetCVar("metroid_cl_chromabeamdensity"), 32);

    int  x  = Hack_StartX,  y  = Hack_StartY,  z  = Hack_StartZ;
    int tx  = Hack_EndX,   ty  = Hack_EndY,   tz  = Hack_EndZ;
    int vx1 = Hack_VelX,   vy1 = Hack_VelY,   vz1 = Hack_VelZ;
    int vx  = tx-x,         vy = ty-y,         vz = tz-z;
    int mag = magnitudeThree_f(vx, vy, vz);
    int magI = min(ftoi(mag), 1024);

    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);

    // At this point, v[xyz] is normalized
    int angle = VectorAngle(vx, vy);
    int pitch = VectorAngle(1.0, -vz);

    // Precalculate matrix multipliers
    // look at helix.txt in Dakka for how I got to what I did
    
    int mx_x = FixedMul(cos(angle), cos(pitch));
    int mx_y = -sin(angle);
    int mx_z = FixedMul(cos(angle), sin(pitch));

    int my_x = FixedMul(sin(angle), cos(pitch));
    int my_y = cos(angle);
    int my_z = FixedMul(sin(angle), sin(pitch));

    int mz_x = -sin(pitch);
    int mz_z = cos(pitch);

    x += vx1;
    y += vy1;

    int tid       = unusedTID(8000, 13000);
    int tid_start = unusedTID(8001, 13000);
    int hx, hy, hz, ex, ey, ez;

    int c, ticOffset, offset, alpha;

    // We do this here so that calculations don't get fucked up,
    //   and so that velocity doesn't send the beam flying,
    //   which GZDoom does apparently? IUNO
    // If it starts happening in Zandronum too, just remove the if part.
    if (met_cvarinfo()) { x -= vx1; y -= vy1; z -= vz1; }

    ticOffset =  Timer() % LASER_ROTATETICS;
    ticOffset *= 1.0;
    ticOffset /= LASER_ROTATETICS;

    for (i = 0; i < magI; i += density)
    {
        hx = itof(i);

        alpha = max(0, LASER_NOEFFECTS_FADELENGTH - i)
              + max(0, (i + LASER_NOEFFECTS_FADELENGTH) - magI);

        alpha = min(alpha, LASER_NOEFFECTS_FADELENGTH);

        alpha = (alpha * alpha) / LASER_NOEFFECTS_FADELENGTH;
        alpha = itof(alpha) / LASER_NOEFFECTS_FADELENGTH;

        alpha = FixedMul(min(1.0, 0.125 * density), alpha);

        for (c = 0; c < COLORCOUNT; c++)
        {
            int start = LaserStarts[c];
            int trail = LaserMids  [c];
            int end   = LaserEnds  [c];

            offset = ticOffset + (c * (1.0 / COLORCOUNT));

            hy = 4 * cos(offset);
            hz = 4 * sin(offset);

            ex = FixedMul(hx, mx_x) + FixedMul(hy, mx_y) + FixedMul(hz, mx_z);
            ey = FixedMul(hx, my_x) + FixedMul(hy, my_y) + FixedMul(hz, my_z);
            ez = FixedMul(hx, mz_x)                      + FixedMul(hz, mz_z);

            if (i == 0 && !noeffects)
            {
                Spawn(start, x+ex, y+ey, z+ez, tid_start);
                SetActorVelocity(tid_start, vx1, vy1, vz1, 0, 0);
                Thing_ChangeTID(tid_start, 0);
            }

            if (!(noeffects && alpha <= 0))
            {
                Spawn(trail, x+ex, y+ey, z+ez, tid);
            }

            if (!noeffects && (i + density >= magI)) { Spawn(end, x+ex, y+ey, z+ez); }
        }
        
        if (noeffects)
        {
            SetActorVelocity(tid, vx1, vy1, vz1, 0, 0);
            SetActorProperty(tid, APROP_Alpha, FixedMul(LASER_BASEALPHA, alpha));
            Thing_ChangeTID(tid, 0);
        }
    }

    if (!noeffects)
    {
        SetActorVelocity(tid, vx1, vy1, vz1, 0, 0);
        SetActorProperty(tid, APROP_Alpha, min(1.0, FixedMul(LASER_BASEALPHA, 0.125 * density)));
        Thing_ChangeTID(tid, 0);
    }
}

script 488 (int which)
{
    int pln = PlayerNumber();
    int ret = 0;

    switch (which)
    {
        case 0: ret = CB_TempCoords[pln][0]; break;
        case 1: ret = CB_TempCoords[pln][1]; break;
        case 2: ret = CB_TempCoords[pln][2]; break;
    }

    SetResultValue(ret);
}

script 489 (int x, int y, int z)
{
    int tx = GetActorX(0);
    int ty = GetActorY(0);
    int tz = GetActorZ(0);

    int i;
    int vx, vy, vz, mag, magI;

    SetActivatorToTarget(0);
    int pln = PlayerNumber();

    vx = tx-x; vy = ty-y; vz = tz-z; mag = magnitudeThree_f(vx, vy, vz);
    vx = FixedDiv(vx, mag); vy = FixedDiv(vy, mag); vz = FixedDiv(vz, mag);
    magI = min(ftoi(mag), 1024);

    for (i = 0; i < magI; i += 16)
    {
        CB_TempCoords[pln][0] = (vx * i);
        CB_TempCoords[pln][1] = (vy * i);
        CB_TempCoords[pln][2] = (vz * i);
        GiveInventory("DBeamLaserItem", 1);
    }
}
