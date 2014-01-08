#define METROID_BASE                         354

#define METROID_SPACEJUMP_ENTER2             351
#define METROID_SPACEJUMP_ENTER              352
#define METROID_OPEN                         353

#define METROID_WALLBOUNCE                   364
#define METROID_REQUESTDODGE                 368

#define METROID_TURN                         385

#define METROID_LONG_XYZ                     455
#define METROID_LONG_BURST                   456
#define METROID_LONG_TRAIL                   459
#define METROID_LONG_COORDSHIT               468

#define METROID_OPEN_CLIENT                  551
#define METROID_MORPHCAMERA                  587
#define METROID_MORPHBALL                    588
#define METROID_UNLOADING                    589
#define METROID_DEATH                        590
#define METROID_DISCONNECT                   591
#define METROID_ENTER                        592
#define METROID_RESPAWN                      593
#define METROID_DECORATE                     594
#define METROID_BOOSTBALL                    595
#define METROID_DECORATECLIENT               596
#define METROID_BWEEBWEEBWEEBWEE             597

#define CLOCKWISE           0
#define COUNTERCLOCKWISE    1

#define PITCH_UP    0
#define PITCH_DOWN  1

#define DASH_VEL 40

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



#define C_TID        1000  //Default camera tid
#define MAX_R        128      //Maximum radius (or distance from the player)
#define ADJUST_R    8      //Amount to adjust the camera by
#define VIEW_HEIGHT    31.0  //The approximate hight of the player's view