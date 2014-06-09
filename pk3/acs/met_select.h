int Select_AvailableBeams[PLAYERMAX][BEAMCOUNT];
int Select_SP_PlayerShit[PLAYERMAX][4];
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
    int setfreeze = 0;

    if (!(Select_InMenu[pln] || onOff))
    {
        Select_InMenu[pln] = 1;
        SetPlayerProperty(0, 1, PROP_TOTALLYFROZEN);

        if (GameType() == GAME_SINGLE_PLAYER && !GetCvar("metroid_cl_nospselectfreeze"))
        {
            setfreeze = 1;
            GiveInventory("BeamSelectNoGrav", 1);
            
            int vx = GetActorVelX(0);
            int vy = GetActorVelY(0);
            int vz = GetActorVelZ(0);
            Select_SP_PlayerShit[pln][0] = 1;
            Select_SP_PlayerShit[pln][1] = vx;
            Select_SP_PlayerShit[pln][2] = vy;
            Select_SP_PlayerShit[pln][3] = vz;
            SetActorVelocity(0, 0,0,0, 0,1);
        }
    }

    ACS_ExecuteAlways(METROID_SELECT_CLIENT, 0, !!onOff);

    if (setfreeze)
    {
        while (Select_InMenu[pln])
        {
            TakeInventory("BeamSelectFreeze", 0x7FFFFFFF);
            GiveInventory("BeamSelectFreeze", 1);
            Delay(35);
        }
    }
}

script METROID_SELECT_CLIENT (int onOff) clientside
{
    int beams = 0;
    int pln  = PlayerNumber();
    int cpln = ConsolePlayerNumber();
    int allspokes = GetCvar("metroid_cl_showallbeams");
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

    if (beams > 1)
    {
        Select_Client_InMenu  = 1;
        Select_Client_Turnoff = 0;

        while (PlayerInGame(pln) && !isDead(0))
        {
            for (i = 0; i < beams; i++)
            {
                int beamIndex = Select_AvailableBeams[pln][i];
                int beamItem  = KnownBeams[beamIndex];
                int beamIcon  = SM_BeamIcons[beamIndex];
                int hasBeam = CheckInventory(beamItem);

                if (!hasBeam) { continue; }

                // Go clockwise, start at top
                int ang = -spokeAngle * i;
                ang += 0.25;

                int sx = cx + (spokelength * cos(ang));
                int sy = cy - (spokelength * sin(ang));

                sx = (sx >> 16) << 16;
                sy = (sy >> 16) << 16;

                SetFont(beamIcon);
                HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID + 1 + i, CR_UNTRANSLATED, sx + 0.4, sy, 0.25, 0.25);
            }

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

            int whichSpoke = (mouse_ang - spokeoffset) * beams;

            if (whichSpoke < 0 || whichSpoke > itof(beams - 1)) { whichSpoke = 0; }
            else { whichSpoke = ceil(whichSpoke); }

            if (Select_Client_TurnOff)
            {
                if (centerdist >= SM_MINCENTERDIST)
                {
                    choice_beam1 = Select_AvailableBeams[pln][whichSpoke];
                }
                break;
            }

            SetHudSize(SM_HUDX, SM_HUDY, 1);
            SetFont("SMALLFONT");
            HudMessage(s:"A"; HUDMSG_FADEOUT, SM_ID, CR_WHITE, hx + 0.4, hy, 0.25, 0.25);

            Delay(1);

            spokelength = min(spokelength + 32, maxlen);
        }

        Select_Client_InMenu = 0;
    }

    if (IsServer)
    {
        ACS_ExecuteAlways(METROID_SELECT_SERVER, 0, choice_beam1+1, choice_beam2+1);
    }
}

script METROID_SELECT_SERVER (int beam1_index, int beam2_index) net
{
    int pln = PlayerNumber();

    if (Select_InMenu[pln])
    {
        Select_InMenu[pln] = 0;
        SetPlayerProperty(0, 0, PROP_TOTALLYFROZEN);

        if (Select_SP_PlayerShit[pln][0])
        {
            TakeInventory("BeamSelectFreeze", 0x7FFFFFFF);
            GiveInventory("BeamSelectYesGrav", 1);
            
            int vx = Select_SP_PlayerShit[pln][1];
            int vy = Select_SP_PlayerShit[pln][2];
            int vz = Select_SP_PlayerShit[pln][3];
            SetActorVelocity(0, vx,vy,vz, 0,1);

            Select_SP_PlayerShit[pln][0] = 0;
        }
    }

    beam1_index -= 1;
    beam2_index -= 1;

    int beam1, beam2;
    int hasbeam2;

    if (beam1_index < 0 || beam1_index >= BEAMCOUNT) { terminate; }
    beam1 = KnownBeams[beam1_index];
    if (!CheckInventory(beam1))
    {
        Delay(1);
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
}
