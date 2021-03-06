#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	inch
#undef	winch
#undef	mvinch
#undef	mvwinch

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_inch  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                          inch

  Synopsis:
  	chtype inch(void);
  	chtype winch(WINDOW *win);
  	chtype mvinch(int y, int x);
  	chtype mvwinch(WINDOW *win, int y, int x);

  X/Open Description:

 	NOTE: All these functions are implemented as macros.

  PDCurses Description:
 	Depending upon the state of the raw character output, 7- or
 	8-bit characters will be output.

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      inch                                  Y        Y       Y
      winch                                 Y        Y       Y
      mvinch                                Y        Y       Y
      mvwinch                               Y        Y       Y

**man-end**********************************************************************/

/***********************************************************************/
chtype	inch(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("inch() - called\n");
#endif

	return( stdscr->_y[stdscr->_cury][stdscr->_curx] );
}
/***********************************************************************/
chtype	winch(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("winch() - called\n");
#endif

	return( win->_y[win->_cury][win->_curx] );
}
/***********************************************************************/
chtype	mvinch(int y, int x)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvinch() - called\n");
#endif

	(void)(move(y,x));
	return( stdscr->_y[stdscr->_cury][stdscr->_curx] );
}
/***********************************************************************/
chtype	mvwinch(WINDOW *win, int y, int x)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvwinch() - called\n");
#endif

	(void)(wmove(win,y,x));
	return( win->_y[win->_cury][win->_curx] );
}
