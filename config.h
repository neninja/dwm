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
static const char col_gray3[]       = "#777777";
static const char col_gray4[]       = "#bbbbbb";
static const char col_gray5[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_cyan2[]       = "#80ffdc";
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
	/*                      fg          bg       border   */
	[SchemeNorm]    = { col_gray3,  col_gray1,  col_gray2   },
	[SchemeSel]     = { col_cyan2,  col_gray1,  col_blue    },
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
#define SUPER Mod4Mask
#define SHIFT ShiftMask
#define CTRL ControlMask
#define MODKEY SUPER
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

// Custom scripts
static const char *dmenu_app[]  = { "dwm_app", NULL };
static const char *dmenu_search[]  = { "dwm_search", NULL };
static const char *dmenu_exit[]  = { "dwm_exit", NULL };
static const char *dmenu_volume[]  = { "dwm_volume", NULL };
static const char *dmenu_bright[]  = { "dwm_bright", NULL };
static const char *dmenu_copypaste[]  = { "dwm_copypaste", NULL };

static Key keys[] = {
	/* modifier         key         function         argument */
    // Restart DWM
	{ SUPER,            XK_r,       quit,           {1} }, 
    // Close client
	{ SUPER|SHIFT,      XK_q,       killclient,     {0} },

    // Custom scripts/spawn softwares
	{ SUPER|SHIFT,      XK_Return,  spawn,          {.v = termcmd } },
    { SUPER|SHIFT,      XK_e,       spawn,          {.v = dmenu_exit } },
	{ SUPER|SHIFT,      XK_a,       spawn,          {.v = dmenu_app } }, // antigo {.v = dmenucmd }
    { SUPER|SHIFT,      XK_v,       spawn,          {.v = dmenu_volume } },
    { SUPER|SHIFT,      XK_b,       spawn,          {.v = dmenu_bright } },
    { SUPER|SHIFT,      XK_c,       spawn,          {.v = dmenu_copypaste } },
    { SUPER|SHIFT,      XK_s,       spawn,          {.v = dmenu_search } },

    // Resize
	{ SUPER,            XK_h,       setmfact,       {.f = -0.05} },
	{ SUPER,            XK_l,       setmfact,       {.f = +0.05} },
	{ SUPER|SHIFT,      XK_l,       setsmfact,      {.f = +0.05} },
	{ SUPER|SHIFT,      XK_h,       setsmfact,      {.f = -0.05} },

    // Master
    // Move to master area
	{ SUPER,            XK_Return,  zoom,           {0} },
    // Increase/decrease number of windows in master area
	{ SUPER|SHIFT,      XK_i,       incnmaster,     {.i = +1 } },
	{ SUPER,            XK_i,       incnmaster,     {.i = -1 } },

    //layouts
	{ SUPER,            XK_t,       setlayout,      {.v = &layouts[0]} },
	{ SUPER,            XK_f,       setlayout,      {.v = &layouts[1]} },
	{ SUPER,            XK_m,       setlayout,      {.v = &layouts[2]} },
    // Toggle previous layout
	{ SUPER,            XK_space,   setlayout,      {0} },
    // Toggle one client floating
	{ SUPER|SHIFT,      XK_space,   togglefloating, {0} },
    // Toggle full Screen
	{ SUPER,            XK_b,       togglebar,      {0} },

    // Show window in every tag
	{ SUPER,            XK_0,       tag,            {.ui = ~0 } },

    // View all clients in current tag
	{ SUPER|SHIFT,      XK_0,       view,           {.ui = ~0 } },

    // Multiplos monitores
    // Change selected monitor
	{ SUPER,            XK_comma,   focusmon,       {.i = -1 } },
	{ SUPER,            XK_period,  focusmon,       {.i = +1 } },
    // Send client to other monitor monitor
	{ SUPER|SHIFT,      XK_comma,   tagmon,         {.i = -1 } },
	{ SUPER|SHIFT,      XK_period,  tagmon,         {.i = +1 } },

    // Movement per window
	{ SUPER,            XK_j,       focusstack,     {.i = +1 } },
	{ SUPER,            XK_k,       focusstack,     {.i = -1 } },
    { SUPER|CTRL,       XK_j,       pushdown,       {0} },
	{ SUPER|CTRL,       XK_k,       pushup,         {0} },
    { SUPER|SHIFT,      XK_u,       focusurgent,    {0} },
    // Transform, move and resize
    { SUPER|CTRL,       XK_Up,		moveresize,     {.v = "0x -25y 0w 0h"} },
	{ SUPER|CTRL,       XK_Down,	moveresize,     {.v = "0x 25y 0w 0h"} },
	{ SUPER|CTRL,       XK_Left,	moveresize,     {.v = "-25x 0y 0w 0h"} },
	{ SUPER|CTRL,       XK_Right,	moveresize,     {.v = "25x 0y 0w 0h"} },
	{ SUPER|CTRL|SHIFT, XK_Up,		moveresize,     {.v = "0x 0y 0w -25h"} },
	{ SUPER|CTRL|SHIFT,	XK_Down,	moveresize,     {.v = "0x 0y 0w 25h"} },
	{ SUPER|CTRL|SHIFT,	XK_Left,	moveresize,     {.v = "0x 0y -25w 0h"} },
    { SUPER|CTRL|SHIFT, XK_Right,   moveresize,     {.v = "0x 0y 25w 0h"} },

    // Movement per tag
    //{ SUPER,             XK_Tab,     shiftview,      {.i = +1 } },
	//{ SUPER|SHIFT,       XK_Tab,     shiftview,      {.i = -1 } },
    { SUPER,              XK_Tab,   view_adjacent,  { .i = +1 } },
	{ SUPER|SHIFT,        XK_Tab,   view_adjacent,  { .i = -1 } },
	TAGKEYS(                            XK_1,                           0)
	TAGKEYS(                            XK_2,                           1)
	TAGKEYS(                            XK_3,                           2)
	TAGKEYS(                            XK_4,                           3)
	TAGKEYS(                            XK_5,                           4)
    // Back to previous tag
	{ SUPER|CTRL,         XK_Tab,     view,           {0} },
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

