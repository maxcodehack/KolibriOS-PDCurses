#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	getch
#undef	wgetch
#undef	mvgetch
#undef	mvwgetch
#undef	ungetch

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	wrefresh
#  undef	nocbreak
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_getch  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                         getch

  Synopsis:
  	int getch(void);
  	int wgetch(WINDOW *win);
  	int mvgetch(int y, int x);
  	int mvwgetch(WINDOW *win, int y, int x);
  	int ungetch(int ch);

  X/Open Description:
 	With the getch(), wgetch(), mvgetch(), and mvwgetch() functions, 
 	a character is read from the terminal associated with the window. 
 	In nodelay mode, if there is no input
 	waiting, the value ERR is returned. In delay mode, the program will
 	hang until the system passes text through to the program.
 	Depending on the setting of cbreak(), this will be after one
 	character or after the first newline.  Unless noecho() has
 	been set, the character will also be echoed into the designated
 	window.

 	If keypad() is TRUE, and a function key is pressed, the token for
 	that function key will be returned instead of the raw characters.
 	Possible function keys are defined in <curses.h> with integers
 	beginning with 0401, whose names begin with KEY_.  If a character
 	is received that could be the beginning of a function key (such as
 	escape), curses will set a timer.  If the remainder of the sequence
 	does not come in within the designated time, the character will be
 	passed through, otherwise the function key value will be returned.
 	For this reason, on many terminals, there will be a delay after a
 	user presses the escape key before the escape is returned to the
 	program.  (Use by a programmer of the escape key for a single
 	character function is discouraged.)

 	If nodelay(win,TRUE) has been called on the window and no input is
 	waiting, the value ERR is returned.

 	The ungetch() function places ch back onto the input queue to be
 	returned by the next call to wgetch().

 	NOTE: getch(), mvgetch() and mvwgetch() are implemented as macros.

  PDCurses Description:
 	Given the nature of the PC, there is no such timer set for an
 	incoming ESCAPE value, because function keys generate unique
 	scan codes that are not prefixed with the ESCAPE character.

 	Also, note that the getch() definition will conflict  with
 	many DOS compiler's runtime libraries.

  X/Open Return Value:
 	This functions return ERR or the value of the character, meta 
 	character or function key token.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      getch                                 Y        Y       Y
      wgetch                                Y        Y       Y
      mvgetch                               Y        Y       Y
      mvwgetch                              Y        Y       Y
      ungetch                               Y        Y       Y

**man-end**********************************************************************/

static WINDOW *w;			/* to reduce stack usage   */

/* this define to get around DOS libraries conflict */
#define getch PDC_getch
/***********************************************************************/
int	PDC_getch(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getch() - called\n");
#endif

	if (stdscr == (WINDOW *)NULL)
		return( ERR );

	return(wgetch(stdscr));
}
/***********************************************************************/
int	wgetch(WINDOW *win)
/***********************************************************************/
{
extern	int	c_pindex;		/* putter index		   */
extern	int	c_gindex;		/* getter index		   */
extern	int	c_ungind;		/* ungetch() push index   */
extern	int	c_ungch[NUNGETCH];	/* array of ungotten chars */
extern  WINDOW*	_getch_win_;

/*	signed	key; MH*/
	int	key;
	bool	cbr;
/*	static	chtype	buffer[_INBUFSIZ];MH*/	/* character buffer */
	static	int	buffer[_INBUFSIZ];	/* character buffer */
	short display_key = 0x100;


	if (win == (WINDOW *)NULL)
		return( ERR );

/* wrs (7/31/93) -- System V curses refreshes window when wgetch is called */
/*                  if there have been changes to it and it is not a pad */
	if( (! (win->_flags & _PAD)) && (! win->_nodelay) ) 
	{
		if( is_wintouched(win) )
			wrefresh(win);
	}

	_getch_win_ = win;

	if (c_ungind)	
	{			/* if ungotten char exists */
						/* remove and return it */
		if ((! (win->_flags & _PAD)) && (win->_nodelay))
		{
			if ( is_wintouched(win) )
				wrefresh(win);
		}
		return( c_ungch[--c_ungind] );
	}

	if ((!_cursvar.raw_inp) && (!_cursvar.cbreak))
	{
		/*
		 * if normal
		 */
		if (c_gindex < c_pindex)
		{
			/*
			 * and data in buffer
			 */
			return( buffer[c_gindex++] );
		}
	}

	w = win;		/* static for speed & stack */
	c_pindex = 0;		/* prepare to buffer data */
	c_gindex = 0;

	for(;;)			/* loop for any buffering */
	{


			key = PDC_rawgetch();


		if (w->_nodelay)
		{
			/*
			 * if nodelay and no char
			 */
			if (key == -1)
				return( ERR );
			else if ( ! _cursvar.echo ) {
				if( ! (w->_flags & _PAD) ) {
					if( is_wintouched(w) )
						wrefresh(w);
				}
			}
		}
		if ((key == '\r') &&
		    (_cursvar.autocr) &&
		    (!_cursvar.raw_inp))
		{
			/*
			 * translate CR
			 */
			key = '\n';
		}
		if (_cursvar.echo && (key < display_key))
		{
			/*
			 * if echo is enabled
			 */
			waddch(w, key);
			wrefresh(w);
		}
		if (_cursvar.raw_inp || _cursvar.cbreak)
		{
			/*
			 * if no buffering
			 */
			return( key );
		}

		if (c_pindex < _INBUFSIZ - 2)
		{
			/*
			 * if no overflow, put data in buffer
			 */
			buffer[c_pindex++] = key;
		}
		if ((key == '\n') || (key == '\r'))
		{
			/*
			 * if we got a line
			 */
			return( buffer[c_gindex++] );
		}
	}
}
/***********************************************************************/
int	mvgetch(int y, int x)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvgetch() - called\n");
#endif

	if (stdscr == (WINDOW *)NULL)
		return( ERR );

	if (move(y, x) == ERR)
		return(ERR);
	return(getch());
}
/***********************************************************************/
int	mvwgetch(WINDOW *win, int y, int x)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvwgetch() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( ERR );

	if (wmove(win,y, x) == ERR)
		return(ERR);
	return(wgetch(win));
}
/***********************************************************************/
int	ungetch(int ch)
/***********************************************************************/
{
extern	int	c_pindex;		/* putter index */
extern	int	c_gindex;		/* getter index */
extern	int	c_ungind;		/* ungetch() push index */
extern	int	c_ungch[NUNGETCH];	/* array of ungotten chars */

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("ungetch() - called\n");
#endif

	if (c_ungind >= NUNGETCH)	/* pushback stack full */
		return( ERR );

	c_ungch[c_ungind++] = ch;
	return( OK );
}
