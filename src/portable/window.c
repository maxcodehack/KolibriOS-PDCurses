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
#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	newwin
#undef	delwin
#undef	mvwin
#undef	subwin
#undef	derwin
#undef	mvderwin
#undef	dupwin
#undef	wsyncup
#undef	syncok
#undef	wcursyncup
#undef	wsyncdown
#undef	resize_window

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	touchwin
#  undef	wmove
#  undef	overwrite
#  undef	wmove
#  undef	wclrtobot
#endif

#ifdef PDCDEBUG
char *rcsid_window  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                        window

  Synopsis:
  	WINDOW *newwin(int nlines, int ncols, int begy, int begx);
  	int delwin(WINDOW *win);
  	int mvwin(WINDOW *win, int y, int x);
  	WINDOW *subwin(WINDOW* orig,int nlines,int ncols,
  			int begin_y,int begin_x);
  	WINDOW *derwin(WINDOW* orig,int nlines,int ncols,
  			int begin_y,int begin_x);
  	int mvderwin(WINDOW *win, int par_y, int par_x);
  	int dupwin(WINDOW *win);
  ***	void wsyncup(WINDOW *win);
  ***	int syncok(WINDOW *win, bool bf);
  ***	void wcursyncup(WINDOW *win);
  ***	void wsyncdown(WINDOW *win);

  	WINDOW *resize_window(WINDOW *w, int lins, int cols);

  X/Open Description:
 	newwin() creates a new window with the given number of lines, 
 	nlines and columns, ncols. The upper left corner of the window is 
 	at line begy, column begx. If either nlines or ncols is zero,
 	they will be defaulted to LINES - begy and COLS - begx. A
 	new full-screen window is created by calling newwin(0, 0, 0, 0).

 	delwin() deletes the named window, freeing all memory associated 
 	with it. In the case of overlapping windows, subwindows should be 
 	deleted before the main window.

 	mvwin() moves the window so that the upper left-hand corner is at
 	position (y,x). If the move would cause the window to be off the
 	screen, it is an error and the window is not moved. Moving subwindows
 	is allowed, but should be avoided. (I don't know why ?).

 	subwin() creates a new sub-window within a window.  The
 	dimensions of the sub-window are nlines lines and ncols
 	columns.  The sub-window is at position (begin_y, begin_x) on
 	the screen.  This position is relative to the screen, and not
 	to the window orig.
 	The sub-window is made in the middle of the window orig, so
 	that changes made to either window will affect both.  When
 	using this routine, it will often be necessary to call
 	touchwin() before calling wrefresh().

 	derwin() is the same as subwin(), except that begin_y and
 	begin_x are relative to the origin of the window orig rather than
 	the screen.  There is no difference between subwindows and derived
 	windows.

 	mvderwin() moves a derived window (or subwindow) inside its
 	parent window.  The screen-relative parameters of the window are not
 	changed.  This routine is used to dispay different parts of the parent
 	window at the same physical position on the screen.

 	dupwin() creates an exact duplicate of the window win.

  PDCurses Description:
 	resize_window() does necessary initializations for the PDCurses 
 	package when doing screen size changes. The user is responsible for
 	deleting and/or resizing windows after this call is made.

 	See the call PDC_resize_win().

 	WARNING: This routine deallocated the existing stdscr, curscr
 	and modifies LINES, COLS and other internal PDCurses
 	variables.

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  PDCurses Errors:
 	It is an error to call resize_window()function before calling initscr().
 	Also, an error will be generated if we fail to create a newly
 	sized replacement window for curscr, or stdscr.
 	This will typically happen when increasing the window size.

 	NOTE:  If this happens, the previously successfully allocated
 	windows are left alone.  i.e. The resize is NOT cancelled for
 	those windows.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      newwin                                Y        Y       Y
      delwin                                Y        Y       Y
      mvwin                                 Y        Y       Y
      subwin                                Y        Y       Y
      derwin                                Y        -       Y
      mvderwin                              Y        -       Y
      dupwin                                -        -      4.0
      wsyncup                               -        -      4.0
      syncok                                -        -      4.0
      wcursyncup                            -        -      4.0
      wsyncdown                             -        -      4.0
      resize_window                         -        -       -

**man-end**********************************************************************/

/***********************************************************************/
WINDOW*	newwin(int nlines, int ncols, int begy, int begx)
/***********************************************************************/
{
extern	void*	(*mallc)( size_t );
extern	void*	(*callc)( size_t, size_t );
extern	void	(*fre)( void* );

	WINDOW*	win;
	chtype*	ptr;
	int	i;
	int	j;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("newwin() - called:lines=%d cols=%d begy=%d begx=%d\n",nlines,ncols,begy,begx);
#endif

	if (nlines == 0)	nlines = LINES - begy;
	if (ncols  == 0)	ncols  = COLS  - begx;

	if ((win = PDC_makenew(nlines, ncols, begy, begx)) == (WINDOW *) NULL)
		return( (WINDOW *)NULL );

	for (i = 0; i < nlines; i++)
	{
		/*
		 * make and clear the lines
		 */
		if ((win->_y[i] = (*callc)(ncols, sizeof(chtype))) == NULL)
		{
			for (j = 0; j < i; j++)
			{
				/*
				 * if error, free all the data
				 */
				(*fre)(win->_y[j]);
			}
			(*fre)(win->_firstch);
			(*fre)(win->_lastch);
			(*fre)(win->_y);
			(*fre)(win);
			return( (WINDOW *)NULL );
		}
		else
		{
			for (ptr = win->_y[i];
			     ptr < win->_y[i] + ncols;)
			{
				/*
				 * Retain the original screen attributes...
				 */
				*ptr++ = _cursvar.blank;
			}
		}
	}
#ifdef UNIX
	PDC_gotoxy(begy, begx);
#endif
	return( win );
}
/***********************************************************************/
int	delwin(WINDOW *win)
/***********************************************************************/
{
extern	void	(*fre)( void* );
	int	i;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("delwin() - called\n");
#endif


	if (win == (WINDOW *)NULL)
		return( ERR );

#ifdef	REGISTERWINDOWS
	_rmwin(win);		/* Remove from the visible windows list... */
#endif

	/*
	 * FYI:  Subwindow's use 'parent's' lines
	 */
	if (!(win->_flags & _SUBWIN)
	&&  !(win->_flags & _SUBPAD))
	{
		for (i = 0; i < win->_pmaxy && win->_y[i]; i++)
		{
			if (win->_y[i] != NULL)
				(*fre)(win->_y[i]);
		}
	}
	(*fre)(win->_firstch);
	(*fre)(win->_lastch);
	(*fre)(win->_y);
	(*fre)(win);
	return( OK );
}
/***********************************************************************/
int	mvwin(WINDOW *win, int y, int x)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvwin() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if (y + win->_maxy > LINES || y < 0)
		return( ERR );

	if (x + win->_maxx > COLS || x < 0)
		return( ERR );

	win->_begy = y;
	win->_begx = x;
	touchwin(win);
	return( OK );
}
/***********************************************************************/
WINDOW*	subwin(WINDOW* orig,int nlines,int ncols,int begin_y,int begin_x)
/***********************************************************************/
{
extern	void*	(*mallc)( size_t );
extern	void*	(*callc)( size_t, size_t );
extern	void	(*fre)( void* );

	WINDOW*	win;
	int	i;
	int	j = begin_y - orig->_begy;
	int	k = begin_x - orig->_begx;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("subwin() - called: lines %d cols %d begy %d begx %d\n",nlines,ncols,begin_y,begin_x);
#endif

	if (!orig)
		return( (WINDOW *)NULL );

	/*
	 * make sure window fits inside the original one
	 */
	if ((begin_y < orig->_begy) ||
	    (begin_x < orig->_begx) ||
	    (begin_y + nlines) > (orig->_begy + orig->_maxy) ||
	    (begin_x + ncols)  > (orig->_begx + orig->_maxx))
	{
		return( (WINDOW *)NULL );
	}
	if (!nlines)	nlines = orig->_maxy - 1 - j;
	if (!ncols)	ncols  = orig->_maxx - 1 - k;
	if ((win = PDC_makenew(nlines, ncols, begin_y, begin_x)) == (WINDOW *) NULL)
	{
		return( (WINDOW *)NULL );
	}

	/*
	 * initialize window variables
	 */
	win->_attrs	= orig->_attrs;
/* wrs (4/10/93) -- account for window background */
	win->_bkgd	= orig->_bkgd;
	win->_leave	= orig->_leave;
	win->_scroll	= orig->_scroll;
	win->_nodelay	= orig->_nodelay;
	win->_use_keypad	= orig->_use_keypad;
	win->_immed	= orig->_immed;
	win->_pary	= j;
	win->_parx	= k;
	win->_parent	= orig;

	for (i = 0; i < nlines; i++)
	{
		win->_y[i] = (orig->_y[j++]) + k;
	}

	win->_flags |= _SUBWIN;
	return (win);
}
/***********************************************************************/
WINDOW*	derwin(WINDOW* orig,int nlines,int ncols,int begin_y,int begin_x)
/***********************************************************************/
{
	if (orig == (WINDOW *)NULL)
		return( ERR );

	return subwin(orig, nlines, ncols, begin_y+orig->_begy, begin_x+orig->_begx);
}
/***********************************************************************/
int	mvderwin( WINDOW* win, int par_y, int par_x )
/***********************************************************************/
{
	int i, j;
	WINDOW *mypar;

	if (win == (WINDOW *)NULL)
		return( ERR );

	if( win->_parent == NULL )
		return( ERR );

	mypar = win->_parent;

	if( par_y < 0 || par_x < 0 ||
		(par_y + win->_maxy) > mypar->_maxy ||
		(par_x + win->_maxx) > mypar->_maxx )
	{
		return( ERR );
	}

	j = par_y;
	for (i = 0; i < win->_maxy; i++)
	{
		win->_y[i] = (mypar->_y[j++]) + par_x;
	}

	win->_pary	= par_y;
	win->_parx	= par_x;

	return( OK );
}
/***********************************************************************/
WINDOW*	dupwin( WINDOW* win )
/***********************************************************************/
{
extern	void*	(*mallc)( size_t );
extern	void*	(*callc)( size_t, size_t );
extern	void	(*fre)( void* );

	WINDOW* new;
	chtype* ptr;
	chtype* ptr1;
	int nlines, ncols, begy, begx;
	int	i;
	int	j;

	if (!win)
		return( (WINDOW *)NULL );

	nlines	= win->_maxy;
	ncols	= win->_maxx;
	begy	= win->_begy;
	begx	= win->_begx;

	if ((new = PDC_makenew(nlines, ncols, begy, begx)) == (WINDOW *) NULL)
		return( (WINDOW *)NULL );

/* copy the contents of win into new */

	for (i = 0; i < nlines; i++)
	{
		if ((new->_y[i] = (*callc)(ncols, sizeof(chtype))) == NULL)
		{
			for (j = 0; j < i; j++)
			{
				/*
				 * if error, free all the data
				 */
				(*fre)(new->_y[j]);
			}
			(*fre)(new->_firstch);
			(*fre)(new->_lastch);
			(*fre)(new->_y);
			(*fre)(new);
			return( (WINDOW *)NULL );
		}
		else
		{
			for (ptr = new->_y[i], ptr1 = win->_y[i];
			     ptr < new->_y[i] + ncols;)
			{
				*ptr++ = *ptr1++;
			}
		}
	}

	new->_curx = win->_curx;
	new->_cury = win->_cury;
	new->_maxy = win->_maxy;
	new->_maxx = win->_maxx;
	new->_pmaxy = win->_pmaxy;
	new->_pmaxx = win->_pmaxx;
	new->_begy = win->_begy;
	new->_begx = win->_begx;
	new->_lastpy = win->_lastpy;
	new->_lastpx = win->_lastpx;
	new->_lastsy1 = win->_lastsy1;
	new->_lastsx1 = win->_lastsx1;
	new->_lastsy2 = win->_lastsy2;
	new->_lastsx2 = win->_lastsx2;
	new->_flags = win->_flags;
	new->_attrs = win->_attrs;
	new->_tabsize = win->_tabsize;
	new->_clear = win->_clear;
	new->_leave = win->_leave;
	new->_scroll = win->_scroll;
	new->_nodelay = win->_nodelay;
	new->_use_keypad = win->_use_keypad;
	new->_use_idl = win->_use_idl;
	new->_use_idc = win->_use_idc;
	new->_tmarg = win->_tmarg;
	new->_bmarg = win->_bmarg;
	new->_title = win->_title;
	new->_title_ofs = win->_title_ofs;
	new->_title_attr = win->_title_attr;
	new->_blank = win->_blank;
	new->_parx = win->_parx;
	new->_pary = win->_pary;
	new->_parent = win->_parent;
	new->_bkgd = win->_bkgd;

	for (i = 0; i < nlines; i++)
	{
		new->_firstch[i] = 0;
		new->_lastch[i] = ncols - 1;
	}

	new->_flags = win->_flags;

	return( new );

}
/***********************************************************************/
WINDOW*	resize_window(WINDOW *w, int lins, int cols)
/***********************************************************************/
{
extern	void*	(*mallc)();	/* ptr to some malloc(size)	*/
extern	void*	(*callc)();	/* ptr to some ecalloc(num,size)*/
extern	void	(*fre)();	/* ptr to some free(ptr)	*/

	WINDOW*	new;
	int	ncols  = max(cols, w->_pmaxx);
	int	nlines = max(lins, w->_pmaxy);
	int	i;
	int	j;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("resize_window() - called: lins %d cols %d\n",lins,cols);
#endif

	if (w == (WINDOW *)NULL)
		return( (WINDOW *)NULL );

	if ((lins > w->_pmaxy) || (cols > w->_pmaxx))
	{
		if ((new = PDC_makenew(nlines, ncols, w->_begy, w->_begx)) == (WINDOW *)NULL)
			return( (WINDOW *)NULL );

		new->_curx = w->_curx;
		new->_cury = w->_cury;
		new->_flags = w->_flags;
		new->_attrs = w->_attrs;
		new->_tabsize = w->_tabsize;
		new->_clear = w->_clear;
		new->_leave = w->_leave;
		new->_scroll = w->_scroll;
		new->_nodelay = w->_nodelay;
		new->_use_keypad = w->_use_keypad;
		new->_tmarg = w->_tmarg;
		new->_bmarg = w->_bmarg;
		new->_title = w->_title;
		new->_title_ofs = w->_title_ofs;
		new->_title_attr = w->_title_attr;
		new->_parent = w->_parent;
#if 0
		memcpy(new->_borderchars, w->_borderchars, 8*sizeof(chtype));
#endif

		for (i = 0; i < nlines; i++)
		{
			/*
			 * make and clear the lines
			 */
			if ((new->_y[i] = (chtype*)(*callc)(ncols, sizeof(chtype))) == NULL)
			{
				for (j = 0; j < i; j++)
				{
					/*
					 * if error, free all the data
					 */
					(*fre)(new->_y[j]);
				}
				(*fre)(new->_firstch);
				(*fre)(new->_lastch);
				(*fre)(new->_y);
				(*fre)(new);
				return( (WINDOW *)NULL );
			}
		}
		if ((w != curscr))
		{
			overwrite(w, new);
			wmove(new, w->_maxy - 1, 0);
			wclrtobot(new);
#if 0
/* JGB box uses defaults if arguments are zero, but we don't want to do
   this if the window currently has no box */
			if (w->_borderchars[0] || w->_borderchars[2])
				box(new, w->_borderchars[0], w->_borderchars[2]);
#endif
		}
		delwin(w);
		return( new );
	}
	else
	{
		w->_bmarg = lins - 1;
		w->_maxy = lins;
		w->_maxx = cols;
#if 0
/* JGB box uses defaults if arguments are zero, but we don't want to do
   this if the window currently has no box */
		if (w->_borderchars[0] || w->_borderchars[2])
			box(w, w->_borderchars[0], w->_borderchars[2]);
#endif
		return( w );
	}
}
