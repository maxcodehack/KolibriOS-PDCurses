#include <string.h>
#define	CURSES_LIBRARY	1
#include <curses.h>
#include <menuet/textcon.h>

#ifndef NO_MEMORY_H
#include <memory.h>
#endif

/*man-start*********************************************************************

  PDC_backchar()	- Visually erase character in window

  PDCurses Description:
 	This is a private PDCurses function

 	This routine will visually erase a character.  It is called by
 	the PDCurses character I/O routines.

  PDCurses Return Value:
 	This routine will return OK upon success and otherwise ERR will be
 	returned.

  PDCurses Errors:
 	It is an error to pass a NULL WINDOW pointer.

  Portability:
 	PDCurses	int	PDC_backchar( WINDOW* w, char* ch, int* len );

**man-end**********************************************************************/

#ifdef THESE_FUNCTIONS_ARENT_USED
/***********************************************************************/
int	PDC_backchar(WINDOW *w, char *ch, int *len)
/***********************************************************************/
{
	int	nbs = 0;
	int	x = w->_curx;
	int	ts = w->_tabsize;
	chtype	s = (w->_y[w->_cury][x - 1] & CHR_MSK);
	char*	p = c_strbeg;
	bool	save_raw_out = _cursvar.raw_out;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_backchar() - called\n");
#endif

	if (w == (WINDOW *)NULL)
		return( ERR );

	(*len)--;		/* Now we are zero relative */
	(*len)--;		/* Now we are looking at the previous
				 * character */
	if( *len >= 0 ) {
		nbs++;
		/*
		 * Determine number of characters to erase...
		 */
		if ((ch[*len] < ' ') || (s == 0x7f))	/* ctrl-char has size 2	 */
		{
			nbs++;
			(*len)--;
		}

		if( *len >= 0 )
		if (ch[*len] == '\t')	/* tabs are very special */
		{
			for (; p < ch; p++)
			{
				if (*p == '\t')
					x = ((x / ts) + 1) * ts;
				else
				{
					if ((*p < ' ') || (*p == 0x7f))
						x += 2;
					else
						x++;
				}
				if (x >= w->_maxx)	/* go to next line? */
					x = 0;
			}
			if (!(w->_curx))
				nbs = w->_maxx - x;
			else
				nbs = w->_curx - x;
		}
	}
	if( *len < 0 )
	{
		beep();
		*len = 0;
	}
	/*
	 * Erase the characters and update...
	 */
	_cursvar.raw_out = FALSE;  /* ensure backspace handled in xlat mode */
	while ( nbs-- > 0 )
	{
		if (w->_curx > 0)
		{
/*			waddstr(w, "\b \b");*/
			mvwaddch(w, w->_cury, w->_curx-1, ' ');
			wmove(w, w->_cury, w->_curx-1);
		}
		else
		if (w->_cury)
		{
			mvwaddch(w, w->_cury - 1, w->_maxx - 1, ' ');
			wmove(w, w->_cury - 1, w->_maxx - 1);
		}
	}
	ch[*len] = '\0';
	_cursvar.raw_out = save_raw_out;
	wrefresh(w);
	return( OK );
}

/*man-start*********************************************************************

  PDC_chg_attr_pair()	- Writes character and attribute to physical screen

  PDCurses Description:
 	This is a private PDCurses function.

 	Writes a single character 'chr' with attribute 'attr' to the
 	current cursor location.

 	NOTE:	Though passed as 16 bit quantities, only the lower 8 bits
 		will be used to create a character/attribute pair.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function under DOS.

 	An ERR may be returned under FLEXOS if s_copy() fails.  See the
 	Flexos Programmer's Reference Manual for details on the error.

  Portability:
 	PDCurses	int PDC_chg_attr_pair( chtype chr, chtype attr );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_chg_attr_pair(chtype chr, chtype attr)
/***********************************************************************/
{
	extern unsigned	char atrtab[MAX_ATRTAB];
	int	phys_attr=chtype_attr(attr);

	_lcon_putch(chr);
	return( OK );
}
#endif

/*man-start*********************************************************************

  PDC_chadd()      - Low level; Put a character to a window

  PDCurses Description:
        This is a private PDCurses function.

        This routine will insert the character 'c' at the current cursor
        position in the passed window.

        If 'xlat' is TRUE, PDC_chadd() will handle things in a cooked
        manner (tabs, newlines, carriage returns, etc).  If 'xlat' is
        FALSE, the characters are simply output directly.

        If 'advance' is TRUE, PDC_chadd() will move the current cusor position
        appropriately. The *addch functions call PDC_chadd() with advance TRUE,
        while the *insch functions call PDC_chadd() with advance FALSE.

        The normal curses routines (non-raw-output-mode) call PDC_chadd()
        with 'xlat' TRUE.

  PDCurses Return Value:
        This function returns OK on success and ERR on error.

  PDCurses Errors:
        It is an error to call this function with a NULL window pointer.

  Portability:
        PDCurses        int PDC_chadd( WINDOW* win, chtype ch, bool xlat, bool advance );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_chadd(register WINDOW *win, chtype ch,bool xlat, bool advance)
/***********************************************************************/
{
	int	retval = ERR;
	int	x=0;
	int	y=0;
	int	newx=0;
	chtype	attr=0, bktmp=0;
	int	ts=0;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_chadd() - called: char=%c attr=0x%x xlat=%d advance=%d\n",ch & A_CHARTEXT,ch & A_ATTRIBUTES,xlat,advance);
#endif

	if (win	== (WINDOW *)NULL)
		return(	retval );

	x	= win->_curx;
	y	= win->_cury;
	ts	= win->_tabsize;

/* if the incoming character doesn't have its own attribute
   then	use the	current	attributes for the window.
   if the incoming character has attributes but	not a colour
   component, or the attributes	to the current attributes
   for the window.
   if the incoming character has a colour component use	the
   attributes solely from the incoming character */

	if ((ch	& A_ATTRIBUTES)	== 0)
	   attr	= win->_attrs;
	else
	   if ((ch & A_COLOR) == 0)
	      attr = (ch & A_ATTRIBUTES) | win->_attrs;
	   else
	      attr = (ch & A_ATTRIBUTES);

	ch	= (ch &	A_CHARTEXT);

/*  wrs (4/10/93)
/*  apply the same sort of logic for the window background, in that it only */
/*  takes precedence if other color attributes are not there and that */
/*  the background character will only print if the printing character is */
/*  blank. */

	if ((attr & A_COLOR) == 0)
	   attr = (attr | (win->_bkgd & A_ATTRIBUTES));
	else {
	   bktmp = (win->_bkgd & A_COLOR);
	   attr = (attr | ( (win->_bkgd & A_ATTRIBUTES) ^ bktmp ));
	}

	if ( ch == ' ' )
		ch = ((win->_bkgd & A_CHARTEXT));


	if ((y > win->_maxy) ||
	    (x > win->_maxx) ||
	    (y < 0) ||
	    (x < 0))
	{
		return(	retval );
	}

	if (xlat)
	{
		switch (ch) {
		case '\t':
			for (newx = ((x	/ ts) +	1) * ts; x < newx; x++)
			{
				if (waddch(win,	' ') ==	ERR)
				{
					return(	retval );
				}
				/*
				 * if tab to next line
				 */
				if (win->_curx == 0)
				{
					/*
					 * exit	the loop
					 */
					return(	OK );
				}
			}
			return(	OK );

		case '\n':
			if (_cursvar.autocr && !(_cursvar.raw_out))
			{
				/*
				 * if lf -> crlf
				 */
				x = 0;
			}
			wclrtoeol( win );
			if ((y = PDC_newline(win, y)) < 0)
				return(	retval );
			if (advance)
			  {
			   win->_cury =	y;
			   win->_curx =	x;
			  }
			return(	OK );

		case '\r':
			if (advance)
			   win->_curx =	x = 0;
			return(	OK );

		case '\b':
			if (--x	< 0)
			{
				/*
				 * no back over	left margin
				 */
				x = 0;
			}
			if (advance)
			   win->_curx =	x;
			return(	OK );

		case 0x7f:
			if (waddch(win,	'^') ==	ERR)
			{
				return(	retval );
			}
			retval = waddch(win, '?');
			return(	retval );

		default:
			break;
		}		/* switch */

		if (ch < ' ')
		{
			/*
			 * handle control chars
			 */
			if (waddch(win,	'^') ==	ERR)
				return(	retval );

			retval = (waddch(win, ch + '@'));
			return(	retval );
		}
	}

	/*
	 *	Add the	attribute back into the	character.
	 */
	ch	|= attr;
/*********************************************************************/
/* only change _firstch/_lastch if character to be added is different */
/* to the character/attribute that is already in that position in the */
/* window.                                                            */
/* Removing this fixes display problems with different windows in the */
/* same physical position. MH 20-03-93                                */
/* Restored again.         MH 02-04-93                                */
/*********************************************************************/
	if (win->_y[y][x] !=	ch)
	{
		/*
		 * only	if data	change
		 */
		if (win->_firstch[y] ==	_NO_CHANGE)
		{
			win->_firstch[y] = win->_lastch[y] = x;
		}
		else
		{
			if (x <	win->_firstch[y])
			{
				win->_firstch[y] = x;
			}
			else
			{
				if (x >	win->_lastch[y])
				{
					win->_lastch[y] = x;
				}
			}
		}
	}
	win->_y[y][x++] = ch;
	if (x >= win->_maxx)
	{
		/*
		 * wrap	around test
		 */
		x = 0;
		if ((y = PDC_newline(win, y)) < 0)
			return(	retval );
	}
	if (advance)
	  {
	   win->_curx =	x;
	   win->_cury =	y;
	  }
	return(	OK );
}

/*man-start*********************************************************************

  PDC_chg_attrs()	- Change attributes in a rectangle

  PDCurses Description:
 	This routine will change the attribute(s) from a starting (y,x)
 	position to an ending (y,x) position to the specified attribute.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	It is an error to call this function with a NULL window pointer.
 	It is also an error to pass rectangular coordinates that lay
 	outside of window.

  Portability:
 	PDCurses	int PDC_chg_attrs( WINDOW* w, chtype attr,
 					int sy, int sx,
 					int ey, int ex );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_chg_attrs(WINDOW *w, chtype attr, int sy, int sx, int ey, int ex)
/***********************************************************************/
{
	chtype	oldattr = w->_attrs;
	int	c=0;
	int	l=0;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_chr_attrs() - called\n");
#endif

	if (w == (WINDOW *)NULL)		return( ERR );
	if (sy > w->_maxy)	return( ERR );
	if (sx > w->_maxx)	return( ERR );
	if (ey >= w->_maxy)	ey = w->_maxy - 1;
	if (ex >= w->_maxx)	ex = w->_maxx - 1;

	wattrset(w, attr);
	for (l = sy; l <= ey; l++)
	{
		for (c = sx; c <= ex; c++)
			w->_y[l][c] = (w->_y[l][c] & A_CHARTEXT) | attr;

		if (w->_firstch[l] == _NO_CHANGE)
		{
			w->_firstch[l] = sx;
			w->_lastch[l] = ex;
		}
		else
		if (w->_firstch[l] != _NO_CHANGE)
		{
			if (sx < w->_firstch[l])
				w->_firstch[l] = sx;
			if (ex > w->_lastch[l])
				w->_lastch[l] = ex;
		}
	}
	w->_attrs = oldattr;
	return( OK );
}

/*man-start*********************************************************************

  PDC_chins()	- Low-level insert character in window

  PDCurses Description:
 	This is a private PDCurses routine.

 	This routine provides the basic functionality for the X/Open
 	[mv][w]insch() routines.  The xlat flag indicates that normal
 	character translation is performed or not.  If not, then the
 	character is output as is.

 	The 'xlat' flag is TRUE for the normal curses routines.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	It is an error to call this function with a NULL window pointer.

  Portability:
 	PDCurses	int PDC_chins( WINDOW* win, chtype c, bool xlat );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_chins(WINDOW *win, chtype c, bool xlat)
/***********************************************************************/
{
	int	retval = ERR;
	int	x=0;
	int	y=0;
	int	maxx=0;
	int	offset=0;
	chtype *temp1;
	char	ch	= (c & A_CHARTEXT);

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_chins() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return( retval );

	x	= win->_curx;
	y	= win->_cury;
	maxx	= win->_maxx;
	offset	= 1;
	temp1	= &win->_y[y][x];

	if ((ch < ' ') && xlat)
	{
		offset++;
	}

	memmove( temp1+offset, temp1, (maxx - x -offset) * sizeof(chtype) );

	win->_lastch[y] = maxx-1;

	if ((win->_firstch[y] == _NO_CHANGE) ||
	    (win->_firstch[y] > x))
	{
		win->_firstch[y] = x;
	}
	/*
	 * PDC_chadd() fixes CTRL-chars too
	 */
	retval = (PDC_chadd(win, c, xlat,FALSE));
	return( retval );
}

/*man-start*********************************************************************

  PDC_clr_scrn()	- Clears the physical screen and homes the cursor.

  PDCurses Description:
 	This is an internal routine called by the doupdate() routines.

  PDCurses Return Value:
 	This routine always returns OK.

  Portability:
 	PDCurses	int PDC_clr_scrn( WINDOW* win );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_clr_scrn(WINDOW *win)
/***********************************************************************/
{
	PDC_scroll(0, 0, LINES - 1, COLS - 1, 0, 0);
	PDC_gotoxy(0, 0);
	return( OK );
}

#ifndef NO_MEMORY_H
#include <memory.h>
#endif

/*man-start*********************************************************************

  PDC_clr_update()	- Updates the screen with a full redraw.

  PDCurses Description:
 	Updates the screen by clearing it and then redraw it in its
 	entirety. If _cursvar.refrbrk is TRUE, and there is pending
 	input characters, the update will be prematurely terminated.

  PDCurses Return Value:
 	This routine returns ERR if it is unable to accomplish it's task.
 	This return value is ONLY under FLEXOS.

 	The return value OK is returned if there were no errors.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_clr_update( WINDOW* s );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_clr_update(WINDOW *s)
/***********************************************************************/
{
 register int	i=0,j=0;	
 WINDOW*	w=NULL;
 unsigned long * ch;
 bool rc=FALSE;
 unsigned short temp_line[256]; /* this should be enough for the maximum width of a screen. MH-920715 */
 unsigned short chr;
 w = curscr;
 if (w == (WINDOW *)NULL) return( ERR );
 s->_clear = FALSE;
 for (i = 0; i < LINES; i++)	/* update physical screen */
 {
  if (s != w)	/* copy s to curscr */
   memcpy(w->_y[i], s->_y[i], COLS * sizeof(chtype));
  ch = w->_y[i]; /* now have ch pointing to area to contain real attributes. MH-920715 */
  for(j=0;j<COLS;j++)
  {
   _lcon_putcxy(j,i,w->_y[i][j]&0xFF);
  }
 }
 _lcon_flush_console();
 return( OK );
}

/*man-start*********************************************************************

  PDC_cursor_on()	- Turns on the hardware cursor.

  PDCurses Description:
 	Turns on the hardware curses, it does nothing if it is already on.

  PDCurses Return Value:
 	Returns OK upon success, ERR upon failure.

  Portability:
 	PDCurses	int PDC_cursor_on( void );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_cursor_on(void)
/***********************************************************************/
{
 if(!_cursvar.visible_cursor)
 {
  _cursvar.visible_cursor = TRUE;
  visible_console->cur_visible = 1;
  _lcon_flushxy(visible_console->cur_x,visible_console->cur_y);
 }
 return( OK );
}

/*man-start*********************************************************************

  PDC_cursor_off()	- Turns off the hardware cursor.

  PDCurses Description:
 	Turns off the hardware curses, it does nothing if it is already off.

  PDCurses Return Value:
 	Returns OK upon success, ERR upon failure.

  PDCurses Errors:
 	ERR will be returned (in the case of FLEXOS) if the hardware cursor
 	can not be disabled.

  Portability:
 	PDCurses	int PDC_cursor_off( void );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_cursor_off(void)
/***********************************************************************/
{

	if	(_cursvar.visible_cursor)
	{
		_cursvar.visible_cursor = FALSE;
		visible_console->cur_visible = 0;
		_lcon_flushxy(visible_console->cur_x,visible_console->cur_y);
	}
	return( OK );
}

/*man-start*********************************************************************

  PDC_fix_cursor()	- Fix the cursor start and stop scan lines (if necessary)

  PDCurses Description:
 	This is a private PDCurses routine.

 	This routine will fix the cursor shape for certain video adapters.
 	Normally, the values used are correct, but some adapters choke.
 	The most noticable choke is on a monochrome adapter.  The "correct"
 	scan lines will result in the cursor being set in the middle of the
 	character cell, rather than at the bottom.

 	The passed flag indicates whether the cursor is visible or not.

 	This only applies to the DOS platform.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_fix_cursor( int flag );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_fix_cursor(int flag)
/***********************************************************************/
{
        return( OK );
}


/*man-start*********************************************************************

  PDC_gotoxy()	- position hardware cursor at (x, y)

  PDCurses Description:
 	This is a private PDCurses routine.

 	Moves the physical cursor to the desired address on the
 	screen. We don't optimize here -- on a PC, it takes more time
 	to optimize than to do things directly.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_gotoxy( int row, int col );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_gotoxy(int row, int col)
/***********************************************************************/
{
 _lcon_gotoxy(col,row);
 return (OK);
}

/*man-start*********************************************************************

  PDC_newline()	- Advances 1 newline from supplied line number.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Does line advance and returns the new cursor line.  If error,
 	return -1.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_newline( WINDOW* win, int lin );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_newline(WINDOW *win, int lin)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_newline() - called: line %d\n",lin);
#endif

	if (win == (WINDOW *)NULL)
		return( -1 );

	if (++lin > win->_bmarg)
	{
		lin--;
		if (win->_scroll)
		{
			scroll(win);

/* wrs -- 7/11/93 ** it seems that System V Curses automatically refreshes
 *                   a window when scrolling occurs via a newline.  This
 *                   could be a feature that isn't intended, but I'll
 *                   implement it here as well for consistency.
 */
 			wrefresh(win);
		}
		else
			return( -1 );
	}
	return( lin );
}

/*man-start*********************************************************************

  PDC_putc()	- Output a character in the current attribute.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Outputs character 'chr' to screen in tty fashion. If a colour
 	mode is active, the character is written with colour 'colour'.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_putc( chtype character, chtype color );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_putc( chtype character, chtype color )
/***********************************************************************/
{
	_lcon_putch(character&A_CHARTEXT);	
	return( OK );
}

/*man-start*********************************************************************

  PDC_putctty()	- Output a character and attribute in TTY fashion.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Outputs character 'chr' to screen in tty fashion. If a colour
 	mode is active, the character is written with colour 'colour'.

 	This function moves the physical cursor after writing so the
 	screen will scroll if necessary.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_putctty( chtype character, chtype color );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_putctty( chtype character, chtype color )
/***********************************************************************/
{
	_lcon_putch(character&A_CHARTEXT);	
	return( OK );
}

/*man-start*********************************************************************

  PDC_scroll()	- low level screen scroll

  PDCurses Description:
 	Scrolls a window in the current page up or down. Urow, lcol,
 	lrow, rcol are the window coordinates.	Lines is the number of
 	lines to scroll. If 0, clears the window, if < 0 scrolls down,
 	if > 0 scrolls up.  Blanks areas that are left, and sets
 	character attributes to attr. If in a colour graphics mode,
 	fills them with the colour 'attr' instead.

  PDCurses Return Value:
 	The PDC_scroll() function returns OK on success otherwise ERR is returned.

  PDCurses Errors:
 	An error will only be returned on the Flexos platform if s_copy()
 	fails.

  Portability:
 	PDCurses	int PDC_scroll( int urow, int lcol, int rcol,
 				     int nlines, chtype attr );

**man-end**********************************************************************/

/***********************************************************************/
int	PDC_scroll(int urow, int lcol, int lrow, int rcol, int nlines, chtype attr)
/***********************************************************************/
{
 int i,j;
 if(!nlines)
 {
  _lcon_clrscr();
  return (OK);
 }
 if(nlines>0)
 {
  for(i=urow;i<=lrow;i++)
   for(j=lcol;j<=rcol;j++)
    memcpy(&visible_console->char_table[i-nlines],
	   &visible_console->char_table[i],
	   sizeof(char_info_t));
 } else {
  for(i=lrow;i>=urow;i--)
   for(j=lcol;j<=rcol;j++)
    memcpy(&visible_console->char_table[i-nlines],
	   &visible_console->char_table[i],
	   sizeof(char_info_t));
 }
 return (OK);
}

/*man-start*********************************************************************

  PDC_transform_line()	- display a physical line of the screen

  PDCurses Description:
 	This is a private PDCurses function.

 	Updates the given physical line to look like the corresponding
 	line in _curscr.

  PDCurses Return Value:
 	This routine returns TRUE if a premature refresh end
 	is allowed, and there is an input character pending.  Otherwise,
 	FALSE is returned.

  PDCurses Errors:
 	No errors are defined for this routine.

  Portability:
 	PDCurses	bool	PDC_transform_line( int lineno );

**man-end**********************************************************************/

/***********************************************************************/
bool	PDC_transform_line(register int lineno)
/***********************************************************************/
{
register chtype*	dstp=NULL;
register chtype*	srcp=NULL;

int	x=0;
int	endx=0;
int	len=0;

unsigned short temp_line[256]={0}; /* this should be enough for the maximum width of a screen. MH-920715 */
unsigned short chr=0;
unsigned short*	ch=NULL;
register int j=0;
bool rc=FALSE;

	if (curscr == (WINDOW *)NULL)
		return( FALSE );

	x = curscr->_firstch[lineno];
	endx = curscr->_lastch[lineno];
	dstp = curscr->_y[lineno] + x;
	srcp = curscr->_y[lineno] + x;
	len = endx-x+1;

	ch = temp_line; /* now have ch pointing to area to contain real attributes. MH-920715 */
	for(j=0;j<len;j++)
	{
	 _lcon_putcxy(j+x,lineno,*srcp);
	 srcp++;
	}
	return(FALSE);
}
