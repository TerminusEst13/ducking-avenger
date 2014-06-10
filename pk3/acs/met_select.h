int Select_AvailableBeams[PLAYERMAX][BEAMCOUNT];
int Select_InMenu[PLAYERMAX];
int Select_Client_InMenu;
int Select_Client_TurnOff;

int SM_BeamIcons[BEAMCOUNT] = 
{
    "WEAPPWBM",
    "WEAPSPBM",
    "WEAPPLBM",
    "WEAPWVBM",
    "WEAPICBM",
    "WEAPLNBM",
    "WEAPCSTR",
    "WEAPDMCN",
};

#define SM_HUDX             800
#define SM_HUDY             600
#define SM_BASEMOUSERES     1337        // the best part is that this actually is a good value.
#define SM_MINCENTERDIST    0.05
#define SM_ID               5700

script METROID_SELECT (int onOff) net
{
    int pln = PlayerNumber();
    int i, beams;

    for (i = 0; i < BEAMCOUNT; i++)
    {
        if (CheckInventory(KnownBeams[i]))
        {
            beams++;
        }
    }

    if (!onOff && CheckInventory("BorphMallAcquired")) { terminate; }

    if (!(Select_InMenu[pln] || onOff) && (beams > 1))
    {
        Select_InMenu[pln] = 1;
        SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);
    }

    if (ConsolePlayerNumber() != -1)
    {
        ACS_ExecuteWithResult(METROID_SELECT_CLIENT, !!onOff);
    }
    else
    {
        ACS_ExecuteAlways(METROID_SELECT_CLIENT, 0, !!onOff);
    }
}

script METROID_SELECT_CLIENT (int onOff) clientside
{
    int beams = 0;          // How many beams are on the menu
    int actualBeams = 0;    // How many beams you actually have
    int pln  = PlayerNumber();
    int cpln = ConsolePlayerNumber();
    int allspokes = GetCvar("metroid_cl_showallbeams");
    int time = 0;
    int i;

    if (pln != cpln) { terminate; }

    if (onOff)
    {
        if (Select_Client_InMenu)
        {
            Select_Client_TurnOff = 1;
        }

        terminate;
    }

    if (Select_Client_InMenu) { terminate; }

    for (i = 0; i < BEAMCOUNT; i++)
    {
        if (CheckInventory(KnownBeams[i]) || allspokes)
        {
            Select_AvailableBeams[pln][beams++] = i;
        }

        if (CheckInventory(KnownBeams[i])) { actualBeams++; }
    }

    int spokeangle = 1.0 / beams;
    int spokeoffset = spokeangle >> 1;
    int spokelength = 16;

    int maxlen = FixedMul(min(SM_HUDX, SM_HUDY), 0.35);

    int cx = (SM_HUDX << 15); // raise to fixed, divide by two
    int cy = (SM_HUDY << 15);
    
    int mouse_x = cx;
    int mouse_y = cy;

    int choice_beam1 = -1;
    int choice_beam2 = -1;

    int highlight_beam2 = 0;

    int beamIndex;
    int beamItem;
    int beamIcon;
    int hasBeam;

    if (actualBeams > 1)
    {
        Select_Client_InMenu  = 1;
        Select_Client_Turnoff = 0;

        while (PlayerInGame(pln) && !isDead(0) && !CheckInventory("BorphMallAcquired"))
        {
            FadeTo(0, 0, 0, min(0.35, 0.05 * (time + 1)), 0);

            int vx  = -GetPlayerInput(-1, INPUT_YAW) * SM_BASEMOUSERES;
            int vy  = -GetPlayerInput(-1, INPUT_PITCH) * SM_BASEMOUSERES;

            int sens, xsens, ysens;

            mouse_x += vx;
            mouse_y += vy;

            mouse_x = middle(0, mouse_x, itof(SM_HUDX));
            mouse_y = middle(0, mouse_y, itof(SM_HUDY));

            int hx = (mouse_x >> 16) << 16;
            int hy = (mouse_y >> 16) << 16;

            int dx = (mouse_x - cx) / SM_HUDX;
            int dy = (mouse_y - cy) / SM_HUDX;  // not a typo
            int centerdist = magnitudeTwo_f(dx, dy);

            // starts at top, goes clockwise
            // top:   (0, -1) becomes (1, 0) which is 0  degrres
            // right: (1, 0)  becomes (0, 1) which is 90 degrees
            int mouse_ang = VectorAngle(-dy, dx);

            int whichSpoke;
            int spokeBeam;
            
            if (centerdist >= SM_MINCENTERDIST)
            {
                whichSpoke = (mouse_ang - spokeoffset) * beams;

                if (whichSpoke < 0 || whichSpoke > itof(beams - 1)) { whichSpoke = 0; }
                else { whichSpoke = ceil(whichSpoke); }

                spokeBeam = Select_AvailableBeams[pln][whichSpoke];
            }
            else
            {
                whichSpoke = -1;
                spokeBeam = -1;
            }

            if (Select_Client_TurnOff)
            {
                if (whichSpoke != -1)
                {
                    choice_beam1 = Select_AvailableBeams[pln][whichSpoke];
                }
                break;
            }

            if (CanBeamStack())
            {
                if (keyPressed(BT_ATTACK) && spokeBeam != -1)
                {
                    beamItem  = KnownBeams[spokeBeam];
                    hasBeam   = CheckInventory(beamItem);

                    if (hasBeam)
                    {
                        choice_beam2 = spokeBeam;
                        highlight_beam2 = 1;
                        LocalAmbientSound("select/choose", 127);
                    }
                    else
                    {
                        LocalAmbientSound("select/badchoose", 127);
                    }
                }
            }
            else
            {
                choice_beam2 = -1;
            }

            for (i = 0; i < beams; i++)
            {
                beamIndex = Select_AvailableBeams[pln][i];
                beamItem  = KnownBeams[beamIndex];
                beamIcon  = SM_BeamIcons[beamIndex];
                hasBeam   = CheckInventory(beamItem);

                if (!hasBeam) { continue; }

                // Go clockwise, start at top
                int ang = -spokeAngle * i;
                ang += 0.25;

                int sx = cx + (spokelength * cos(ang));
                int sy = cy - (spokelength * sin(ang));

                sx = (sx >> 16) << 16;
                sy = (sy >> 16) << 16;

                SetFont(beamIcon);
                HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.05, 0.25);

                if (whichSpoke == i)
                {
                    SetFont("HI1_BEAM");
                    HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + (beams * 1) + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.05, 0.25);
                }

                if (choice_beam2 == beamIndex)
                {
                    SetFont("HI2_BEAM");
                    HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + (beams * 3) + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.05, 0.25);
                }
                else if (InBeamStackWith(choice_beam2, beamIndex))
                {
                    SetFont("HI3_BEAM");
                    HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + (beams * 3) + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.05, 0.25);
                }
                else if (InBeamStackWith(spokeBeam, beamIndex))
                {
                    SetFont("HI4_BEAM");
                    HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + (beams * 3) + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.05, 0.25);
                }
            }

            Log(d:choice_beam1, s:", ", d:choice_beam2, s:", ", d:spokeBeam);

            SetHudSize(SM_HUDX, SM_HUDY, 1);
            SetFont("SL_XHAIR");
            HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID, CR_WHITE, hx + 0.4, hy, 0.05, 0.25);

            Delay(1);

            highlight_beam2 = 0;
            spokelength = min(spokelength + 32, maxlen);
            time++;
        }

        FadeTo(0, 0, 0, 0, 0);
        Select_Client_InMenu = 0;
    }

    if (IsServer)
    {
        ACS_ExecuteAlways(METROID_SELECT_SERVER, 0, choice_beam1+1, choice_beam2+1);
    }
}

#define SM_SWITCHTOTIME     35

script METROID_SELECT_SERVER (int beam1_index, int beam2_index) net
{
    int pln = PlayerNumber();

    if (Select_InMenu[pln])
    {
        Select_InMenu[pln] = 0;
        SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);
    }

    beam1_index -= 1;
    beam2_index -= 1;

    int beam1, beam2;
    int hasbeam2;

    if (beam1_index < 0 || beam1_index >= BEAMCOUNT) { terminate; }
    beam1 = KnownBeams[beam1_index];
    if (!CheckInventory(beam1))
    {
        LocalAmbientSound("select/nobeam", 127);
        terminate;
    }

    if ((beam2_index < 0 || beam2_index >= BEAMCOUNT) || (beam1_index == beam2_index))
    {
        hasbeam2 = 0;
    }
    else
    {
        hasbeam2 = 1;
        beam2 = KnownBeams[beam2_index];
    }

    if (!(CanBeamStack() && hasbeam2))
    {
        SetWeapon(beam1);
        terminate;
    }

    // beam stackan here
    
    int i;
    int usingit = 0;
    int stackwep = BeamStackIndex(beam1_index, beam2_index);

    if (stackwep != -1)
    {
        stackwep = BeamStackWeapons[stackwep];
        Print(s:stackwep);

        if (CheckInventory(stackwep)) { terminate; }

        GiveInventory(stackwep, 1);
        SetWeapon(stackwep);

        for (i = 0; i < SM_SWITCHTOTIME; i++)
        {
            if (CheckWeapon(stackwep))
            {
                usingit = 1;
                break;
            }
            Delay(1);
        }

        if (usingit)
        {
            while (CheckWeapon(stackwep)) { Delay(1); }
        }
        
        TakeInventory(stackwep, 0x7FFFFFFF);
    }
    else
    {
        LocalAmbientSound("select/nobeam", 127);
        SetWeapon(beam2);
    }
}
