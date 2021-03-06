#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	addchstr
#undef	addchnstr
#undef	waddchstr
#undef	waddchnstr
#undef	mvaddchstr
#undef	mvaddchnstr
#undef	mvwaddchstr
#undef	mvwaddchnstr

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_addchstr  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                        addchstr

  Synopsis:
  	int addchstr(chtype *ch);
  	int addchnstr(chtype *ch, int n);
  	int waddchstr(WINDOW *win, chtype *ch);
  	int waddchnstr(WINDOW *win, chtype *ch, int n);
  	int mvaddchstr(int y, int x, chtype *ch);
  	int mvaddchnstr(int y, int x, chtype *ch, int n);
  	int mvwaddchstr(WINDOW *, int y, int x, chtype *ch);
  	int mvwaddchnstr(WINDOW *, int y, int x, chtype *ch, int n);

  X/Open Description:
 	These routines write a chtype directly into the window structure
 	starting at the current position.
 	The four routines with n as the last argument copy at most n
 	elements, but no more than will fit on the line.
 	If n=-1 then the whole string is copied, to the maximum number
 	that will fit on the line.

 	The cursor position is not advanced. These routines do not check for
 	newline or other special characters, no does any line wrapping occur.

 	NOTE:	addchstr(), mvaddchstr(), mvwaddchstr() addchnstr(), 
 		mvaddchnstr(), and mvwaddchnstr() are implemented as macros.

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      addchstr                              -        -      4.0
      waddchstr                             -        -      4.0
      mvaddchstr                            -        -      4.0
      mvwaddchstr                           -        -      4.0
      addchnstr                             -        -      4.0
      waddchnstr                            -        -      4.0
      mvaddchnstr                           -        -      4.0
      mvwaddchnstr                          -        -      4.0

**man-end**********************************************************************/

/***********************************************************************/
int	addchstr(chtype *ch)
/***********************************************************************/
{
	int i;
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("addchstr() - called\n");
#endif

	return( addchnstr( ch, -1) );
}
/***********************************************************************/
int	addchnstr(chtype *ch, int n)
/***********************************************************************/
{
	int y,x,num,maxx;
	chtype *ptr,*saveptr;
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("addchnstr() - called\n");
#endif

	if (stdscr == (WINDOW *)NULL)
		return( ERR );

	if (n == 0
	||  n < (-1))
		return( ERR );

	x = stdscr->_curx;
	y = stdscr->_cury;
	ptr = &(stdscr->_y[y][x]);


	if (n == (-1))
		{
		for (num = stdscr->_maxx - x; *ch && num--; num++)
			*ptr++ = *ch++;
			maxx = num;
		}
	else
		{
		num = min(stdscr->_maxx - x,n);
		memcpy((char *)ptr, (char *)ch, (int)(num * sizeof(chtype)));
		maxx = x+num-1;
		}

	if (stdscr->_firstch[y] ==	_NO_CHANGE)
		{
		stdscr->_firstch[y] = x;
		stdscr->_lastch[y] = maxx;
		}
	else
		{
		if (x <	stdscr->_firstch[y])
			{
			stdscr->_firstch[y] = x;
			}
		if (maxx >	stdscr->_lastch[y])
			{
			stdscr->_lastch[y] = maxx;
			}
		}
	return( OK );
}
/***********************************************************************/
int	waddchstr(WINDOW *win, chtype *ch)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("waddchstr() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return( waddchnstr( win, ch, -1) );
}
/***********************************************************************/
int	waddchnstr(WINDOW *win, chtype *ch, int n)
/***********************************************************************/
{
	int y,x,num,maxx;
	chtype *ptr,*saveptr;
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("waddchnstr() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if (n == 0
	||  n < (-1))
		return( ERR );

	x = win->_curx;
	y = win->_cury;
	ptr = &(win->_y[y][x]);


	if (n == (-1))
		{
		for (num = win->_maxx - x; *ch && num--; num++)
			*ptr++ = *ch++;
			maxx = num;
		}
	else
		{
		num = min(win->_maxx - x,n);
		memcpy((char *)ptr, (char *)ch, (int)(num * sizeof(chtype)));
		maxx = x+num-1;
		}

	if (win->_firstch[y] ==	_NO_CHANGE)
		{
		win->_firstch[y] = x;
		win->_lastch[y] = maxx;
		}
	else
		{
		if (x <	win->_firstch[y])
			{
			win->_firstch[y] = x;
			}
		if (maxx >	win->_lastch[y])
			{
			win->_lastch[y] = maxx;
			}
		}
	return( OK );
}
/***********************************************************************/
int	mvaddchstr(int y, int x, chtype *ch)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvaddchstr() - called: y %d x %d\n",y,x);
#endif

	if (stdscr == (WINDOW *)NULL)
		return( ERR );

	if (wmove(stdscr,y,x) == ERR)
		return( ERR );

	return( addchnstr( ch, -1) );
}
/***********************************************************************/
int	mvaddchnstr(int y, int x, chtype *ch, int n)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvaddchnstr() - called: y %d x %d n %d\n",y,x,n);
#endif

	if (stdscr == (WINDOW *)NULL)
		return( ERR );

	if (wmove(stdscr,y,x) == ERR)
		return( ERR );

	return( addchnstr( ch, n) );
}
/***********************************************************************/
int	mvwaddchstr(WINDOW *win, int y, int x, chtype *ch)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("waddchstr() - called:\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if (wmove(win,y,x) == ERR)
		return( ERR );

	return( waddchnstr( win, ch, -1) );
}
/***********************************************************************/
int	mvwaddchnstr(WINDOW *win,int y, int x, chtype *ch, int n)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvwaddchnstr() - called: y %d x %d n %d \n",y,x,n);
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if (wmove(win,y,x) == ERR)
		return( ERR );

	return( waddchnstr( win, ch, n) );
}
