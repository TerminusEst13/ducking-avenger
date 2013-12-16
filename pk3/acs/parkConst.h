#define PARKMORE_BASE 354

#define PARKMORE_OPEN               (PARKMORE_BASE-1)
#define PARKMORE_ENTER              (PARKMORE_BASE-2)
#define PARKMORE_ENTER2             (PARKMORE_BASE-3)
#define PARKMORE_UNLOADING          (PARKMORE_BASE-4)

#define PARKMORE_TURN               (PARKMORE_BASE+31)
#define PARKMORE_PITCH              (PARKMORE_BASE+32)
#define PARKMORE_SETTURN            (PARKMORE_BASE+33)
#define PARKMORE_SETPITCH           (PARKMORE_BASE+34)

#define PARKMORE_WALLBOUNCE         (PARKMORE_BASE+10)
#define PARKMORE_LEDGEWALL          (PARKMORE_BASE+11)
#define PARKMORE_LEDGEHOLD          (PARKMORE_BASE+12)
#define PARKMORE_WALLHOLD           (PARKMORE_BASE+13)
#define PARKMORE_REQUESTDODGE       (PARKMORE_BASE+14)

#define PARKMORE_TOGGLE             (PARKMORE_BASE+21)

#define PARKMORE_ASSORTED           (PARKMORE_BASE+41)

#define CLOCKWISE           0
#define COUNTERCLOCKWISE    1

#define PITCH_UP    0
#define PITCH_DOWN  1

#define WB_XYBASE   35.0
#define WB_ZBASE    1.25

#define LW_LEDGE    0
#define LW_WALL     1

#define WB_DODGE    0
#define WB_KICK     1
#define WB_KICKUP   2

#define WD_FORWARD  1
#define WD_FORWRITE 2
#define WD_RIGHT    3
#define WD_BACKRITE 4
#define WD_BACK     5
#define WD_BACKLEFT 6
#define WD_LEFT     7
#define WD_FORWLEFT 8
#define WD_KICK     9

/*int wallCheckers[9] =
{
    "ParkmoreCheckS",
    "ParkmoreCheckSE",
    "ParkmoreCheckE",
    "ParkmoreCheckNE",
    "ParkmoreCheckN",
    "ParkmoreCheckNW",
    "ParkmoreCheckW",
    "ParkmoreCheckSW",
    "ParkmoreCheckKick",
};*/

int AngleOffsets[10] = { 0.5, 0.5, 0.375, 0.25, 0.125, 0, 0.875, 0.75, 0.625, 0 };

int noLedgeGrab[PLAYERMAX];


#define JUMP_FORWARD 15.0
#define JUMP_BACK    5.0
#define JUMP_SIDE    15.0

#define LUNGE_FORWARD 20.0
#define LUNGE_ZMULT   0.8

#define HIJUMP_BACK   5.0
#define HIJUMP_ZMULT  1.6

#define MJUMP_DELAY     8

#define TIMER_COUNT     7

#define TIMER_CFORWARD   0
#define TIMER_CRIGHT     1
#define TIMER_CBACK      2
#define TIMER_CLEFT      3
#define TIMER_BOUNCED    4
#define TIMER_DIDDODGE   5
#define TIMER_HBACK      6

#define CACOUNT         15

#define HMSG_BASE       42666
