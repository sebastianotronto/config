/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=14";

/* Dark scheme: */
/*
static const char col_gray1[]       = "#141414";
static const char col_gray2[]       = "#504945";
static const char col_gray3[]       = "#bdae93";
static const char col_gray4[]       = "#f9f9f9";
static const char col_cyan[]        = "#3465a4";
*/

/* Light theme: */
static const char col_gray1[]       = "#fffff8";
static const char col_gray2[]       = "#000000";
static const char col_gray3[]       = "#000000";
static const char col_gray4[]       = "#000000";
static const char col_cyan[]        = "#f0f0f4";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_cyan  },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class | instance | title | tagsmask | isfloating | monitor */
	{ "TelegramDesktop", NULL, NULL,                 1 << 3,  0, -1 },
	{ NULL,              NULL, "stfloat",            0,       1, -1 },
	{ NULL,              NULL, "Picture-in-Picture", TAGMASK, 1, -1 },
};

/* layout(s) */
static const float mfact        = 0.5;  /* factor of master area */
static const int nmaster        = 1;    /* number of clients in master area */
static const int resizehints    = 0;    /* 1 means respect size hints */
static const int refreshrate    = 120;  /* refresh rate (per second) for client move/resize */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview, {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,        {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "st", NULL };
static char dmenumon[2] = "0"; /* used in dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon,
                                               "-fn", dmenufont,
                                               "-nb", col_gray1,
                                               "-nf", col_gray3,
                                               "-sb", col_cyan,
                                               "-sf", col_gray4, NULL };

/* Necessary library for some multimedia key names */
#include <X11/XF86keysym.h>

/* My zoomstack function; passing .i=1 means always focus stack */
void
zoomstack(const Arg *arg)
{
	int s = 0;
	unsigned int j;
	Monitor *m = selmon;
	Client *c = selmon->sel, *f, **p;

	if (!m->lt[m->sellt]->arrange)
		return;

	if (!c || (c && c->isfloating)) {
		c = nexttiled(m->clients);
		s = 1;
	}

	for (j = 0, f = nexttiled(m->clients); f && j < m->nmaster; 
	     f = nexttiled(f->next), j++) {
		if (f == c) {
			c = nexttiled(c->next);
			s = 1;
		}
	}
	if (c && f == c)
		c = nexttiled(c->next);
	for (p = &m->clients; *p && (*p)->next != f; p = &(*p)->next);
	if (!*p || !c)
		return;
	detach(c);
	c->next = (*p)->next;
	(*p)->next = c;
	arrange(m);
	if (!s || arg->i)
		focus(c);
}

/* Focusmater by Mateus Auler - <mateusauler at protonmail dot com>
 * Taken from https://dwm.suckless.org/patches/focusmaster           */
void
focusmaster(const Arg *arg)
{
	Client *c;

	if (selmon->nmaster < 1)
		return;

	c = nexttiled(selmon->clients);

	if (c)
		focus(c);
}

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_o,      spawn,          SHCMD("open-file") },
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd} },
	{ MODKEY,                       XK_n,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_d,      incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      togglefloating, {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	/*
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	*/

	/* Session */
	{ MODKEY|ShiftMask,  XK_q,  spawn, SHCMD("dmenu-dwm-sessionmanager") },
	{ MODKEY|ShiftMask,  XK_z,  spawn, SHCMD("slock") },

	/* Programs */
	{ MODKEY|ShiftMask,  XK_Return,    spawn, SHCMD("terminal") },
	{ MODKEY|ShiftMask,  XK_backslash, spawn, SHCMD("popup-terminal") },
	{ MODKEY|ShiftMask,  XK_f,         spawn, SHCMD("firefox") },
	{ MODKEY|ShiftMask,  XK_g,         spawn, SHCMD("xedit") },
	{ MODKEY|ShiftMask,  XK_j,         spawn, SHCMD("terminal python") },
	{ MODKEY,            XK_y,         spawn, SHCMD("clip") },
	{ MODKEY,            XK_d,         spawn, SHCMD("popup-cal12") },
	{ MODKEY,            XK_q,         spawn, SHCMD("dmenu-unmount") },
	{ MODKEY,            XK_e,         spawn, SHCMD("dmenu-bookmarks") },

	/* System settings */
	{ MODKEY|ShiftMask,  XK_w,  spawn, SHCMD("popup-terminal iwctl") },
	{ MODKEY|ShiftMask,  XK_v,  spawn, SHCMD("popup-terminal pulsemixer") },
	{ MODKEY|ShiftMask,  XK_b,  spawn, SHCMD("popup-terminal bluetoothctl") },
	{ MODKEY|ShiftMask,  XK_p,  spawn, SHCMD("popup-terminal top") },
	{ MODKEY,            XK_space, spawn, SHCMD("xkfix") },

  	/* Multimedia */
	{ 0,   XK_Print,                 spawn, SHCMD("dmenu-screenshot") },
	{ 0,   XF86XK_MonBrightnessUp,   spawn, SHCMD("brightnessctl set 5%+") },
	{ 0,   XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 5%-") },
	{ 0,   XF86XK_AudioRaiseVolume,  spawn, SHCMD("pulsemixer --change-volume +5 && xroot-update") },
	{ 0,   XF86XK_AudioLowerVolume,  spawn, SHCMD("pulsemixer --change-volume -5 && xroot-update") },
	{ 0,   XF86XK_AudioMute,         spawn, SHCMD("pulsemixer --toggle-mute && xroot-update") },

	/* zoomstack (.i = 1: always focus) and others */
	{ MODKEY,              XK_backslash,  zoomstack,       {.i = 0} },
	{ MODKEY|ControlMask,  XK_Return,     focusmaster,     {0} },

	/* Notify */
	{ ControlMask,           XK_space, spawn, SHCMD("notify clean") },
	{ ControlMask|ShiftMask, XK_space, spawn, SHCMD("popup-terminal 'notify show | less'") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click         event mask  button   function        argument */
	{ ClkLtSymbol,   0,          Button1, setlayout,      {0} },
	{ ClkLtSymbol,   0,          Button3, setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,   0,          Button2, zoom,           {0} },
	{ ClkStatusText, 0,          Button2, spawn,          {.v = termcmd } },
	{ ClkClientWin,  MODKEY,     Button1, movemouse,      {0} },
	{ ClkClientWin,  MODKEY,     Button2, togglefloating, {0} },
	{ ClkClientWin,  MODKEY,     Button3, resizemouse,    {0} },
	{ ClkTagBar,     0,          Button1, view,           {0} },
	{ ClkTagBar,     0,          Button3, toggleview,     {0} },
	{ ClkTagBar,     MODKEY,     Button1, tag,            {0} },
	{ ClkTagBar,     MODKEY,     Button3, toggletag,      {0} },
};

