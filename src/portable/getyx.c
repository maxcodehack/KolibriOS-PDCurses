#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	getbegy
#undef	getbegx
#undef	getmaxy
#undef	getmaxx
#undef	getpary
#undef	getparx

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#endif

#ifdef PDCDEBUG
char *rcsid_getyx  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                         getyx

  Synopsis:
  	void getyx(WINDOW *win, int y, int x);
  	void getparyx(WINDOW *win, int y, int x);
  	void getbegyx(WINDOW *win, int y, int x);
  	void getmaxyx(WINDOW *win, int y, int x);

  	int getbegy(WINDOW *win);
  	int getbegx(WINDOW *win);
  	int getpary(WINDOW *win);
  	int getparx(WINDOW *win);
  	int getmaxy(WINDOW *win);
  	int getmaxx(WINDOW *win);

  X/Open Description:
 	With the getyx() macro, the cursor position of the window is placed
 	in the two integer variables y and x. getbegyx() and getmaxyx()
 	return the current beginning coordinates and size of the specified
 	window respectively. getparyx() returns the beginning coordinates 
 	of the parent's window if the specified window is a sub-window
 	otherwise -1 is returned.

 	These functions are implemented as macros, so their return values
 	are undefined. Also as these functions are implemented as macros,
 	no '&' is required before the variables y and x.

 	The functions getbegy(), getbegx(), getmaxy(), getmaxx() ,
 	getpary(), and getparx() do return the beginning coordinate or 
 	size values, so they can be specified on the right-hand side of 
 	an assignment statement. They are, however, implemented as macros.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      getyx                                 Y        Y       Y
      getparyx                              -        -      4.0
      getbegyx                              -        -      3.0
      getmaxyx                              -        -      3.0
      getbegy                               -        -       -
      getbegx                               -        -       -
      getpary                               -        -       -
      getparx                               -        -       -
      getmaxy                               -        -       -
      getmaxx                               -        -       -

**man-end**********************************************************************/

/***********************************************************************/
int	getbegy(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getbegy() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_begy);
}
/***********************************************************************/
int	getbegx(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getbegx() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_begx);
}
/***********************************************************************/
int	getpary(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getpary() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_pary);
}
/***********************************************************************/
int	getparx(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getparx() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_parx);
}
/***********************************************************************/
int	getmaxy(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getmaxy() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_maxy);
}
/***********************************************************************/
int	getmaxx(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getmaxx() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(win->_maxx);
}
