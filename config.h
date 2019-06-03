#include "push.c"
#include "moveresize.c"

/* See LICENSE file for copyright and license details. */
// https://dwm.suckless.org/customisation

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int minwsz    = 20;       /* Minimal heigt of a client for smfact */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "monospace:size=10" };
static const char *fonts[]          = { "Noto Color Emoji:size=10", "UbuntuMono Nerd Font:size=10"};
//static const char dmenufont[]       = "monospace:size=10";
static const char dmenufont[]       = "UbuntuMono Nerd Font:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_blue[]        = "#58CCED";
static const char col_black[]       = "#000000";
static const char col_red[]         = "#ff0000";
static const char col_yellow[]      = "#ffff00";
static const char col_green[]       = "#8BE2AF";
static const char col_white[]       = "#ffffff";

/* AO ADICIONAR MAIS ESTADOS ALTERAR EM ENUMS NO dwm.c */
// Exige mais proxessamento!
// Usage: echo -e "\x01 \x02 \x03 \x04 \x05"
static const char *colors[][3]      = {
	/*                      fg         bg         border   */
	[SchemeNorm]    = { col_gray3,  col_gray1,  col_gray2   },
	[SchemeSel]     = { col_gray4,  col_gray1,  col_blue    },
	[SchemeSuc]     = { col_black,  col_green,  col_cyan    },
	[SchemeWarn]    = { col_black,  col_yellow, col_red     },
	[SchemeUrgent]  = { col_white,  col_red,    col_red     },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title           tags mask     isfloating   monitor */
	{ "Gimp",           NULL,       NULL,           1<<6,         0,           -1 },
	//{ NULL,             NULL,       "neomutt",      1<<8,         0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact     = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static Key keys[] = {
	/* modifier                         key        function        argument */
    // Close
	{ MODKEY|ShiftMask,                 XK_e,       quit,           {0} },
    // Restart
	{ MODKEY|ShiftMask,                 XK_r,       quit,           {1} }, 
	{ MODKEY|ShiftMask,                 XK_q,       killclient,     {0} },
    { MODKEY|ShiftMask,                 XK_s,       spawn,          SHCMD("st -e sudo shutdown -h 0") },
    { MODKEY,                           XK_w,       spawn,          SHCMD("st -e . ~/dev/dotfiles/scripts/dwm/lock.sh && read") },
    { MODKEY|ShiftMask,                 XK_w,       spawn,          SHCMD("st -e . ~/dev/dotfiles/scripts/dwm/lock.sh && read") },

    // Spawn softwares
	{ MODKEY,                           XK_d,       spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,                 XK_Return,  spawn,          {.v = termcmd } },

    // Soud volume
    { MODKEY,                           XK_v,       spawn,          SHCMD("amixer -q -D pulse sset Master 10%-") },
    { MODKEY|ShiftMask,                 XK_v,       spawn,          SHCMD("amixer -q -D pulse sset Master 10%+") },
    { MODKEY|ControlMask,               XK_v,       spawn,          SHCMD("amixer -q -D pulse sset Master toggle") },

    // Resize
	{ MODKEY,                           XK_h,       setmfact,       {.f = -0.05} },
	{ MODKEY,                           XK_l,       setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,                 XK_l,       setsmfact,      {.f = +0.05} },
	{ MODKEY|ShiftMask,                 XK_h,       setsmfact,      {.f = -0.05} },

    // Master
    // Move to master area
	{ MODKEY,                           XK_Return,  zoom,           {0} },
    // Increase/decrease num            ber of windows in master area
	{ MODKEY|ShiftMask,                 XK_i,       incnmaster,     {.i = +1 } },
	{ MODKEY,                           XK_i,       incnmaster,     {.i = -1 } },

    //layouts
	{ MODKEY,                           XK_t,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                           XK_f,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                           XK_m,       setlayout,      {.v = &layouts[2]} },
    // Toggle mono
	{ MODKEY,                           XK_space,   setlayout,      {0} },
    // One window floating
	{ MODKEY|ShiftMask,                 XK_space,   togglefloating, {0} },
    // Full Screen
	{ MODKEY,                           XK_b,       togglebar,      {0} },

    // Show window in every             tag
	{ MODKEY,                           XK_0,       tag,            {.ui = ~0 } },

    // View all clients in c            urrent tag
	{ MODKEY|ShiftMask,                 XK_0,       view,           {.ui = ~0 } },

    // ?????
	{ MODKEY,                           XK_comma,   focusmon,       {.i = -1 } },
	{ MODKEY,                           XK_period,  focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,                 XK_comma,   tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,                 XK_period,  tagmon,         {.i = +1 } },

    // Movement per window
	{ MODKEY,                           XK_j,       focusstack,     {.i = +1 } },
	{ MODKEY,                           XK_k,       focusstack,     {.i = -1 } },
    { MODKEY|ControlMask,               XK_j,       pushdown,       {0} },
	{ MODKEY|ControlMask,               XK_k,       pushup,         {0} },
    { MODKEY|ShiftMask,                 XK_u,       focusurgent,    {0} },
    // Transform, move and resize
    { MODKEY|ControlMask,				XK_Up,		moveresize,     {.v = "0x -25y 0w 0h"} },
	{ MODKEY|ControlMask,				XK_Down,	moveresize,     {.v = "0x 25y 0w 0h"} },
	{ MODKEY|ControlMask,				XK_Left,	moveresize,     {.v = "-25x 0y 0w 0h"} },
	{ MODKEY|ControlMask,				XK_Right,	moveresize,     {.v = "25x 0y 0w 0h"} },
	{ MODKEY|ControlMask|ShiftMask,		XK_Up,		moveresize,     {.v = "0x 0y 0w -25h"} },
	{ MODKEY|ControlMask|ShiftMask,		XK_Down,	moveresize,     {.v = "0x 0y 0w 25h"} },
	{ MODKEY|ControlMask|ShiftMask,		XK_Left,	moveresize,     {.v = "0x 0y -25w 0h"} },
    { MODKEY|ControlMask|ShiftMask,     XK_Right,   moveresize,     {.v = "0x 0y 25w 0h"} },

    // Movement per tag
    //{ MODKEY,                           XK_Tab,     shiftview,      {.i = +1 } },
	//{ MODKEY|ShiftMask,                 XK_Tab,     shiftview,      {.i = -1 } },
    { MODKEY,              XK_Tab,           view_adjacent,  { .i = +1 } },
	{ MODKEY|ShiftMask,              XK_Tab,           view_adjacent,  { .i = -1 } },
	TAGKEYS(                            XK_1,                           0)
	TAGKEYS(                            XK_2,                           1)
	TAGKEYS(                            XK_3,                           2)
	TAGKEYS(                            XK_4,                           3)
	TAGKEYS(                            XK_5,                           4)
	//TAGKEYS(                            XK_6,                           5)
	//TAGKEYS(                            XK_7,                           6)
	//TAGKEYS(                            XK_8,                           7)
	//TAGKEYS(                            XK_9,                           8)
    // back to previous tag
	{ MODKEY|ControlMask,               XK_Tab,     view,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

