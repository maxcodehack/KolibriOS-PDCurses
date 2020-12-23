#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	start_color
#undef	init_pair
#undef	init_color
#undef	has_colors
#undef	can_change_color
#undef	color_content
#undef	pair_content

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#endif

static int PDC_init_pair(short,short,short);

#ifdef PDCDEBUG
char *rcsid_color  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                         color

  Synopsis:
  	int start_color(void);
  	int init_pair(short pair, short fg, short bg);
  ***	int init_color(short color, short red, short green, short blue);
  	bool has_colors(void);
  	bool can_change_color(void);
  ***	int color_content(short color, short *redp, short *greenp, short bluep);
  	int pair_content(short pair, short *fgp, short *bgp);

  X/Open Description:
 	To use these routines, start_color() must be called, usually
 	immediately after initscr(). Colors are always used in pairs refered
 	to as color-pairs. A color-pair consists of a foreground color and
 	a background color. A color-pair is initialized with init_pair().
 	After it has been initialized, COLOR_PAIR(n), a macro defined in
 	<curses.h>, can be used like any other video attribute.

 	start_color() initializes eight basic colors (black,red,green,yellow,
 	blue,magenta,cyan, and white), and two global variables; COLORS
 	and COLOR_PAIRS (respectively defining the maximum number of
 	colors and color-pairs the terminal is capable of displaying).

 	init_pair() changes the definitions of a color-pair.
 	The routine takes three arguments: the number of the color-pair
 	to be redefined, and the new values of the foreground and
 	background colors.
 	The value of color-pair must be between 1 and COLOR_PAIRS-1.
 	The values of foreground and background must be between 0 and
 	COLORS-1 (this is a PDCurses abberation; normally it is 0 and
 	COLORS)
 	If the color pair was previously initialized, the screen is refreshed
 	and all occurrences of that color-pair are changed to the new
 	definition.

 	has_colors() indicates if the terminal supports, and can maniplulate
 	color. It returns TRUE or FALSE.

 	can_change_color() indicates if the terminal has the capability 
 	to change the definition of its colors. Although this is possible,
 	at least with VGA monitors, this function always returns FALSE.

 	pair_content() is used to determine what the colors of a given 
 	color-pair consist of.

 	NOTE: has_colors() is implemented as a macro.

  X/Open Return Value:
 	All functions return OK on success and ERR on error except for
 	has_colors() and can_change_colors() which return TRUE or FALSE.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      start_color                           -        -      3.2
      init_pair                             -        -      3.2
      init_color                            -        -      3.2
      has_colors                            -        -      3.2
      can_change_color                      -        -      3.2
      color_content                         -        -      3.2
      pair_content                          -        -      3.2

**man-end**********************************************************************/

int COLORS=0,COLOR_PAIRS=0;

unsigned char  atrtab[MAX_ATRTAB] = /* COLOR_PAIR to attribute encoding table. */
 {0x07,0x0F,0x70,0x78,0x87,0x8F,0xF0,0xF8, /* A_NORMAL */
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x00,0x08,0x70,0x78,0x80,0x88,0xF0,0xF8,
  0x70,0x00,0x17,0x00,0x00,0x00,0x00,0x00  /* for flash() */
  };

/***********************************************************************/
int start_color(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("start_color() - called\n");
#endif
 if (_cursvar.mono)
    return(ERR);
 COLORS = 8;
 COLOR_PAIRS = 33;  /* actually only allows 32 */
 return(OK);
}
/***********************************************************************/
int init_pair(short colorpair,short foreground,short background)
/***********************************************************************/
{
 register int i;
 short oldforeground, oldbackground;
 unsigned char norm;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("init_pair() - called: colorpair %d fore %d back %d\n",colorpair,foreground,background);
#endif

 if (colorpair >= COLOR_PAIRS || colorpair < 1)
    return(ERR);

 norm = (unsigned char)(foreground & 0x0007) + ((background & 0x0007)<<4);
 oldforeground = (short)(atrtab[colorpair*PDC_OFFSET] & 0x0F);
 oldbackground = (short)((atrtab[colorpair*PDC_OFFSET] & 0xF0)>>4);

 atrtab[(colorpair*PDC_OFFSET)+0] = norm;                         /* normal */
 atrtab[(colorpair*PDC_OFFSET)+1] = norm + 8;                       /* bold */
 atrtab[(colorpair*PDC_OFFSET)+4] = norm + 128;                    /* blink */
 atrtab[(colorpair*PDC_OFFSET)+5] = norm + 8 + 128;           /* bold-blink */

 if ( oldforeground != foreground || oldbackground != background )
    curscr->_clear = TRUE;

 return(OK);
}
/***********************************************************************/
bool has_colors(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("has_colors() - called\n");
#endif
	if (_cursvar.mono)
		return(FALSE);
	return(TRUE);
}
/***********************************************************************/
int	init_color(short color,short red,short green,short blue)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("init_color() - called\n");
#endif
	return(OK);
}
/***********************************************************************/
int can_change_color(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("can_change_color() - called\n");
#endif

#ifdef UNIX
	if (can_change)
		return(TRUE);
	else
		return(FALSE);
#else
	return(FALSE);
#endif
}
/***********************************************************************/
int pair_content(int colorpair,short *foreground,short *background)
/***********************************************************************/
{

 if (colorpair >= COLOR_PAIRS || colorpair < 1)
    return(ERR);

 *foreground = (short)(atrtab[colorpair*PDC_OFFSET] & 0x0F);
 *background = (short)((atrtab[colorpair*PDC_OFFSET] & 0xF0)>>4);

 if ( *foreground <= 0 && *background <= 0 )
 	return(ERR);

 return(OK);
}
