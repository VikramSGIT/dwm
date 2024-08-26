/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 0;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 0;       /* vert inner gap between windows */
static const unsigned int gappoh    = 0;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 0;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Noto Sans Mono:style=Regular:size=13", "Symbols Nerd Font Mono:style=Regular:size=13" };
static const char dmenufont[]       = "monospace:size=13";
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
static const char *tags[] = { "1", "2", "3", "4", "5" };
//static char cb_buffer[32]; //extra
static char vol_buffer[32] = " | - VOL: ??% + | "; //extra
static char bright_buffer[32] = " - BRI: -1% + "; //extra
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int attachbelow = 1;    /* 1 means attach after the currently active window */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "default",      tile },    /* first entry is default */
	{ "floating",      NULL },    /* no layout function means floating behavior */
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

/* custom scripts */
static const char *gettime[] = {"sh", "-c", "while true; do xsetroot -name \" $(date +'%H:%M %a %d-%b')\"; sleep $((60 - $(date +%S))); done", NULL};
static const char *volget[] = { "sh", "-c", "xsetroot -name \"<1>$(wpctl get-volume @DEFAULT_SINK@ | awk '{ if ($3 == \"[MUTED]\") { print \"MUT: \" $2 * 100 \"%\" } else { print \"VOL: \" $2 * 100 \"%\" } }')\"", NULL};
static const char *volinc[] = { "sh", "-c", "wpctl set-volume @DEFAULT_SINK@ 5%+ && xsetroot -name \"<1>$(wpctl get-volume @DEFAULT_SINK@ | awk '{print \"VOL: \"$2 * 100 \"%\"}')\"", NULL };
static const char *volmute[] = { "sh", "-c", "wpctl set-mute @DEFAULT_SINK@ toggle && xsetroot -name \"<1>$(wpctl get-volume @DEFAULT_SINK@ | awk '{ if ($3 == \"[MUTED]\") { print \"MUT: \" $2 * 100 \"%\" } else { print \"VOL: \" $2 * 100 \"%\" } }')\"", NULL };
static const char *voldec[] = { "sh", "-c", "wpctl set-volume @DEFAULT_SINK@ 5%- && xsetroot -name \"<1>$(wpctl get-volume @DEFAULT_SINK@ | awk '{print \"VOL: \" $2 * 100 \"%\"}')\"", NULL };
static const char *brightget[] = { "sh", "-c", "xsetroot -name \"<3>$(brightnessctl | awk '/Current/ {gsub(/[()]/,\"\",$4); print $4}')\"", NULL};
static const char *brightinc[] = { "sh", "-c", "xsetroot -name \"<3>$(brightnessctl set 5%+ | awk '/Current/ {gsub(/[()]/,\"\",$4); print $4}')\"", NULL};
static const char *brightdec[] = { "sh", "-c", "xsetroot -name \"<3>$(brightnessctl set 5%- | awk '/Current/ {gsub(/[()]/,\"\",$4); print $4}')\"", NULL};
static const char * getall[] = { "sh", "-c", "xsetroot -name \"<3>$(brightnessctl | awk '/Current/ {gsub(/[()]/,\"\",$4); print $4}')\" && xsetroot -name \"<1>$(wpctl get-volume @DEFAULT_SINK@ | awk '{ if ($3 == \"[MUTED]\") { print \"MUT: \" $2 * 100 \"%\" } else { print \"VOL: \" $2 * 100 \"%\" } }')\" && while true; do xsetroot -name \" $(date +'%H:%M %a %d-%b')\"; sleep $((60 - $(date +%S))); done", NULL};
/* commands */
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browser[]  = { "brave", NULL };
static const char *browser_other[]  = { "firefox", NULL };
static const char *explorer[]  = { "thunar", NULL };
static const char *discord[]  = { "discord", NULL };
static const char *patchbay[] = { "qpwgraph", NULL };
static const char *steam[] = {"steam", NULL };
static const char *rofi[] = {"rofi", "-show", "drun"};
static const char *cliptyper[] = {"/home/dedshot/script.sh", NULL};
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,		        XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,		        XK_b,      spawn,          {.v = browser } },
	{ MODKEY,		        XK_e,      spawn,          {.v = explorer } },
	{ MODKEY,		        XK_d,      spawn,          {.v = discord } },
	{ MODKEY,		        XK_p,      spawn,          {.v = patchbay } },
	{ MODKEY|ShiftMask,	        XK_b,      spawn,          {.v = browser_other } },
	{ MODKEY,		        XK_s,      spawn,          {.v = steam } },
	{ MODKEY,			XK_v,	   spawn,	   {.v = cliptyper } },
	{ MODKEY,			XK_r,	   spawn,	   {.v = rofi } },
	{ MODKEY|ShiftMask,             XK_p,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
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
	{ MODKEY,			XK_F10,	   spawn,	   {.v =  voldec } },	
	{ MODKEY,			XK_F11,	   spawn,	   {.v =  volinc } },	
	{ MODKEY,			XK_F9,	   spawn,	   {.v =  volmute } },	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY|ShiftMask,             XK_m,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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
	{ ClkVolInc,            0,	        Button1,        spawn,		{.v = volinc} },
	{ ClkVolMute,           0,	        Button1,        spawn,		{.v = volmute} },
	{ ClkVolDec,            0,	        Button1,        spawn,		{.v = voldec} },
	{ ClkBrightInc,         0,	        Button1,        spawn,		{.v = brightinc} },
	{ ClkBrightDec,         0,	        Button1,        spawn,		{.v = brightdec} },
};

