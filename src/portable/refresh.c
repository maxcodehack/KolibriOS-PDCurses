/*
***************************************************************************
* This file comprises part of PDCurses. PDCurses is Public Domain software.
* You may use this code for whatever purposes you desire. This software
* is provided AS IS with NO WARRANTY whatsoever.
* Should this software be used in another application, an acknowledgement
* that PDCurses code is used would be appreciated, but is not mandatory.
*
* Any changes which you make to this software which may improve or enhance
* it, should be forwarded to the current maintainer for the benefit of 
* other users.
*
* The only restriction placed on this code is that no distribution of
* modified PDCurses code be made under the PDCurses name, by anyone
* other than the current maintainer.
* 
* See the file maintain.er for details of the current maintainer.
***************************************************************************
*/
#include <string.h>
#define	CURSES_LIBRARY	1
#include <curses.h>

#ifndef NO_MEMORY_H
#include <memory.h>
#endif

#include<menuet/textcon.h>

/* undefine any macros for functions defined in this module */
#undef	refresh
#undef	wrefresh
#undef	wnoutrefresh
#undef	doupdate
#undef	redrawwin
#undef	wredrawln

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	wattrset
#  undef	mvwprintw
#  undef	wmove
#  undef	wattrset
#  undef	touchwin
#  undef	reset_prog_mode
#endif

#ifdef PDCDEBUG
char *rcsid_refresh  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                       refresh

  Synopsis:
  	int	refresh(void);
  	int	wrefresh(WINDOW *win);
  	int	wnoutrefresh(register WINDOW *win);
  	int	doupdate(void);
  	int	redrawwin(WINDOW *win);
  	int	redrawln(WINDOW *win, int beg_line, int num_lines);

  X/Open Description:
 	The routine wrefresh() copies the named window to the physical
 	terminal screen, taking into account what is already there in
 	order to optimize cursor movement.
 	The routine refresh() does the same, using stdscr as a default
 	screen.
 	These routines must be called to get any output on the
 	terminal, as other routines only manipulate data structures.
 	Unless leaveok has been enabled, the physical cursor of the
 	terminal is left at the location of the window's cursor.

 	The wnoutrefresh() and doupdate() routines allow multiple updates
 	with more efficiency than wrefresh() alone.  In addition to all 
 	of the window structures representing the terminal screen: a physical
 	screen, describing what is actually on the screen and a virtual screen,
 	describing what the programmer wants to have on the screen.

 	The wrefresh() function works by first calling wnoutrefresh(),
 	which copies the named window to the virtual screen.  It then
 	calls doupdate(), which compares the virtual screen to the
 	physical screen and does the actual update.  If the programmer
 	wishes to output several windows at once, a series of cals to
 	wrefresh() will result in alternating calls to wnoutrefresh()
 	and doupdate(), causing several bursts of output to the
 	screen.  By first calling wnoutrefresh() for each window, it
 	is then possible to call doupdate() once.  This results in
 	only one burst of output, with probably fewer total characters
 	transmitted and certainly less CPU time used.

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      refresh                               Y        Y       Y
      wrefresh                              Y        Y       Y
      wnoutrefresh                          Y        Y       Y
      doupdate                              Y        Y       Y
      redrawwin                             -        -      4.0
      wredrawln                             -        -      4.0

**man-end**********************************************************************/

/***********************************************************************/
int	refresh( void )
/***********************************************************************/
{
 int i;
 i=wrefresh(stdscr);
 return i;
}
/***********************************************************************/
int	wrefresh(WINDOW *win)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("wrefresh() - called\n");
#endif

	if (win == (WINDOW *)NULL)	return( ERR );
	if (win->_flags & _PAD)	return( ERR );
	if (win->_flags & _SUBPAD)	return( ERR );

	if (win == curscr)
		curscr->_clear = TRUE;
	else	
		wnoutrefresh(win);
	doupdate();
	return( OK );
}
/***********************************************************************/
int	wnoutrefresh(WINDOW *win)
/***********************************************************************/
{
register int		first;	/* first changed char on line */
register int		last;	/* last changed char on line  */
	int		begy;	/* window's place on screen   */
	int		begx;
	int		i;
	int		j;
	int		y;
	int		x;
	int		len;
	chtype		attrs;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("wnoutrefresh() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if ((win->_flags == _PAD)
	||  (win->_flags == _SUBPAD))
		return( ERR );

	y = win->_cury;
	x = win->_curx;
	attrs = win->_attrs;
	if (win->_title != NULL)
		len = strlen(win->_title);
	/*
	 * There may be a better place to implement window titles, but this
	 * seems to be the best place. -- Frotz
	 */
	if ((len > 0) 
	&& (win->_title != NULL) 
	&& !(win->_flags & _SUBWIN)
	&& !(win->_flags & _SUBPAD))
	{
		wattrset(win, win->_title_attr);
		mvwprintw(win, 0, (win->_title_ofs), "%s", (long) win->_title);
		wmove(win, y, x);	/* restore cursor postion */
		wattrset(win, attrs);	/* restore attributes	  */
	}

	if (win->_flags & _PAD)
		return( ERR );

	begy = win->_begy;
	begx = win->_begx;

	for (i = 0, j = begy; i < win->_maxy; i++, j++)
	{
		if (win->_firstch[i] != _NO_CHANGE)
		{
			first = win->_firstch[i];
			last = win->_lastch[i];

			memcpy(&(curscr->_y[j][begx + first]),
			       &(win->_y[i][first]),
			       (last - first + 1) * sizeof(chtype));

			first += begx;	/* s's min/max change positions */
			last += begx;

			if (curscr->_firstch[j] != _NO_CHANGE)
				curscr->_firstch[j] = min(curscr->_firstch[j], first);
			else
				curscr->_firstch[j] = first;

			curscr->_lastch[j] = max(curscr->_lastch[j], last);

			win->_firstch[i] = _NO_CHANGE;	/* updated now */
		}
		win->_lastch[i] = _NO_CHANGE;	/* updated now */
	}

	if (win->_clear)
	{
		win->_clear = FALSE;
	}

	if (!win->_leave)
	{
		curscr->_cury = win->_cury + begy;
		curscr->_curx = win->_curx + begx;
	}
	return( OK );
}
/***********************************************************************/
int	doupdate(void)
/***********************************************************************/
{
register int	i;
bool rc;
#ifdef	REGISTERWINDOWS
	WINDS*	next = _cursvar.visible;
#endif
 _lcon_flush_console();

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("doupdate() - called\n");
#endif

#ifdef	REGISTERWINDOWS
	if (_cursvar.refreshall)
	{
		while (next != NULL)
		{
			if (next->w->_parent != NULL)
			{
				touchwin(next->w->_parent);
				wnoutrefresh(next->w->_parent);
			}
			touchwin(next->w);
			wnoutrefresh(next->w);
			next = next->next;
		}
	}
#endif

	if  (_cursvar.shell)
		reset_prog_mode();

	if (curscr == (WINDOW *)NULL)
		return( ERR );

/* if checking for typeahead, bail out here if any is found */

	
	if (_cursvar.refrbrk && (_cursvar.cbreak || _cursvar.raw_inp)) 
	{
		rc = PDC_breakout();	 
		if(rc)
			return( OK );
	}

	if (curscr->_clear)
	{
		PDC_clr_update(curscr);
	}
	else
	{
		for (i = 0; i < LINES; i++)
		{
			if (curscr->_firstch[i] != _NO_CHANGE)
				if (PDC_transform_line(i))  /* if test new */
					break;
		}
	}

#if defined (XCURSES)
	XCurses_wait_for_display();
#endif

	PDC_gotoxy(curscr->_cury, curscr->_curx);
	_cursvar.cursrow = curscr->_cury;
	_cursvar.curscol = curscr->_curx;

	return( OK );
}
/***********************************************************************/
int	redrawwin(WINDOW *win)
/***********************************************************************/
{
	register int i;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("redrawwin() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	return(wredrawln(win,0,win->_maxy));
}
/***********************************************************************/
int	wredrawln(WINDOW *win, int start, int num)
/***********************************************************************/
{
	register int i;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("wredrawln() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if  (start > win->_maxy || start + num > win->_maxy)
		return( ERR );
	for(i=start;i<start+num;i++)
	   {
		win->_firstch[i] = 0;
		win->_lastch[i] = win->_maxx - 1;
	   }
 _lcon_flush_console();
	return( OK );
}
