/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=14" };
static const char dmenufont[]       = "monospace:size=16";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	/* { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 }, */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define XF86MonBrightnessUp 0x1008ff02
#define XF86MonBrightnessDown 0x1008ff03
#define XF86AudioRaiseVolume 0x1008ff13
#define XF86AudioLowerVolume 0x1008ff11
#define XF86AudioMute 0x1008ff12

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *brightnessupcmd[]  = { "xbacklight", "-inc", "15", NULL };
static const char *brightnessdowncmd[]  = { "xbacklight", "-dec", "15", NULL };
static const char *soundupcmd[]  = { "amixer", "-q", "-D", "pulse", "sset", "Master", "5%+", NULL };
static const char *sounddowncmd[]  = { "amixer", "-q", "-D", "pulse",  "sset", "Master", "5%-", NULL };
static const char *soundtogglecmd[]  = { "amixer", "-q", "-D", "pulse",  "sset", "Master", "toggle", NULL };
static const char *ouputintcmd[]  = { "pacmd", "set-card-profile", "alsa_card.pci-0000_00_1f.3", "output:analog-stereo", NULL };
static const char *ouputextcmd[]  = { "pacmd", "set-card-profile", "alsa_card.pci-0000_00_1f.3", "output:iec958-stereo", NULL };

static const char *browsercmd[]  = { "firefox", NULL };
static const char *networkmanagercmd[]  = { "st", "nmtui", NULL };
static const char *lockcmd[]  = { "slock", NULL };
static const char *showclipcmd[]  = { "showclip", NULL };
static const char *musiccmd[]  = { "com.spotify.Client", NULL };

static const char *mountcmd[]  = { "dmount", NULL };
static const char *umountcmd[]  = { "dumount", NULL };
static const char *sessionmngrcmd[]  = { "dsessionmngr", NULL };
static const char *unicodecmd[]  = { "dunicode", NULL };
static const char *clipmenucmd[]  = { "clipmenu", "-fn", dmenufont, NULL };
static const char *clipmenuurlcmd[]  = { "clipmenuurl", "-fn", dmenufont, NULL };

static const char *filemanagercmd[]  = { "st", "vifm", NULL };
static const char *filemanagerxcmd[]  = { "pcmanfm", NULL };


// static const char *sscmd[]  = { "maim", "$HOME/xx.png", NULL };
// static const char *sswindowcmd[]  = { "maim", "-i", "$(xdotool getactivewindow)", ">", "~/$(date +%F-%s).png", NULL };
// static const char *ssregioncmd[]  = { "maim", "-s", ">", "~/$(date +%F-%s).png", NULL };
// static const char *ssclipcmd[]  = { "maim", "|", "xclip", "-selection", "clipboard", "-t", "image/png", NULL };
// static const char *sswindowclipcmd[]  = { "maim", "-i", "$(xdotool getactivewindow)", "|", "xclip", "-selection", "clipboard", "-t", "image/png", NULL };
// static const char *ssregionclipcmd[]  = { "maim", "-s", "|", "xclip", "-selection", "clipboard", "-t", "image/png", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
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
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
/* -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - - - - - - - - - - - - - - */
    { MODKEY|ControlMask,           XK_Return,                 spawn,         SHCMD("samedir") },

	{ 0,                            XF86MonBrightnessUp,       spawn,         {.v = brightnessupcmd } },
	{ 0,                            XF86MonBrightnessDown,     spawn,         {.v = brightnessdowncmd } },
	{ 0,                            XF86AudioRaiseVolume,      spawn,         {.v = soundupcmd } },
	{ 0,                            XF86AudioLowerVolume,      spawn,         {.v = sounddowncmd } },
	{ 0,                            XF86AudioMute,             spawn,         {.v = soundtogglecmd } },
	{ MODKEY|ShiftMask,             XK_F11,         spawn,          {.v = ouputextcmd } },
	{ MODKEY|ShiftMask,             XK_F12,         spawn,          {.v = ouputintcmd } },

    { 0,                            XK_Print,      spawn,           SHCMD("maim ~/$(date +%F-%s).png") },
    { MODKEY,                       XK_Print,      spawn,           SHCMD("maim -i $(xdotool getactivewindow) > ~/$(date +%F-%s).png") },
    { ShiftMask,                    XK_Print,      spawn,           SHCMD("maim -s > ~/$(date +%F-%s).png") },
	// { 0,                            XK_Print,      spawn,          {.v = sscmd} },
    // { MODKEY,                       XK_Print,      spawn,          {.v = sswindowcmd} },
    // { MODKEY|ShiftMask,             XK_PrintScreen,      spawn,          {.v = } },

	{ MODKEY,                       XK_w,           spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_w,           spawn,          {.v = networkmanagercmd} },
	{ MODKEY|ControlMask,           XK_l,           spawn,          {.v = lockcmd} },
	{ MODKEY|ShiftMask,             XK_quotedbl,    spawn,          {.v = showclipcmd} },
	{ MODKEY,                       XK_s,           spawn,          {.v = musiccmd } },

	{ MODKEY|ShiftMask,             XK_m,           spawn,          {.v = mountcmd} },
	{ MODKEY|ShiftMask,             XK_u,           spawn,          {.v = umountcmd} },
	{ MODKEY|ControlMask,           XK_Delete,      spawn,          {.v = sessionmngrcmd} },
	{ MODKEY,                       XK_quotedbl,    spawn,          {.v = unicodecmd} },
    { MODKEY,                       XK_Insert,      spawn,          {.v = clipmenucmd } },
    { MODKEY|ShiftMask,             XK_Insert,      spawn,          {.v = clipmenuurlcmd } },

	{ MODKEY,                       XK_e,           spawn,          {.v = filemanagercmd} },
	{ MODKEY|ShiftMask,             XK_e,           spawn,          {.v = filemanagerxcmd} },
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

