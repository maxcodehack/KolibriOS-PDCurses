#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	cbreak
#undef	nocbreak
#undef	echo
#undef	noecho
#undef	halfdelay
#undef	intrflush
#undef	keypad
#undef	meta
#undef	nodelay
#undef	notimeout
#undef	raw
#undef	noraw
#undef	noqiflush
#undef	qiflush
#undef	timeout
#undef	wtimeout
#undef	typeahead

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_inopts  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                        inopts

  Synopsis:
  	int cbreak(void);
  	int nocbreak(void);
  	int echo(void);
  	int noecho(void);
  ***	int halfdelay(int tenths);
  	int intrflush(WINDOW *win, bool bf);
  	int keypad(WINDOW *win, bool bf);
  	int meta(WINDOW *win, bool bf);
  	int nodelay(WINDOW *win, bool bf);
  	int notimeout(WINDOW *win, bool bf);
  	int raw(void);
  	int noraw(void);
  ***	void noqiflush(void);
  ***	void qiflush(void);
  ***	int timeout(int delay);
  ***	int wtimeout(WINDOW *win, int delay);
  	int typeahead(int fildes);

  X/Open Description:
 	cbreak() and nocbreak() puts the terminal into and out of cbreak
 	mode. In cbreak mode, characters typed by the user are immediately
 	available to the program and erase/kill character processing is
 	not performed.  When out of cbreak mode, the terminal driver
 	will buffer characters typed until a newline or carriage return
 	is typed.  Interrupt and flow control characters are unaffected
 	by this mode.  Initially the terminal may or may not need be
 	in cbreak mode.

 	echo() and noecho() control whether characters typed by the user
 	are echoed by the input routine.  Initially, input characters
 	are echoed.  Subsequent calls to echo() and noecho() do not
 	flush type-ahead.

 	If the intrflush() option is enabled (bf is TRUE), and an interrupt
 	is pressed on the keyboard (INTR, BREAK, or QUIT) all output in
 	the terminal driver queue will be flushed, giving the effect
 	of faster response to the interrupt but causing curses to have
 	the wrong idea of what is on the screen.  Disabling the option
 	prevents the flush.  The default for the option is inherited
 	from the terminal driver settings.  The window argument is
 	ignored.

 	The keypad() function changes the keypad option of the user's terminal.
 	If enabled (bf is TRUE), the user can press a function key (such
 	as the left arrow key) and getch() will return a single value
 	that represents the KEY_LEFT function key.  (See Section 11.3.3,
 	Input Values.)  If disabled, curses will not treat function keys
 	as special keys and the program has to interpret the escape
 	sequences itself.  If the keypad is enabled, the terminal keypad
 	is turned on before input begins.

 	The meta() function forces the user's terminal to return 7 or 8
 	significant bits on input.  To force 8 bits to be returned,
 	invoke meta() with bf as TRUE.  To force 7 bits to be returned,
 	invoke meta() with bf as FALSE.
 	The window argument is always ignored, but it must still be a
 	valid window to avoid compiler errors.

 	The nodelay() function controls whether wgetch() is a non-blocking
 	call. If the option is enabled, and no input is ready, wgetch()
 	will return ERR. If disabled, wgetch() will hang until input
 	is ready.

 	While interpreting an input escape sequence, wgetch sets a timer while
 	waiting for the next character.  If notimeout(win,TRUE) is called, then
 	wgetch does not set a timer.  The purpose of the timeout is to
 	differentiate between sequences received from a function key and those
 	typed by a user.

 	With raw() and noraw(), the terminal in placed into or out of raw 
 	mode.  Raw mode is similar to cbreak mode, in that characters typed 
 	are immediately passed through to the user program.  The differences
 	are that in raw mode, the INTR, QUIT, SUSP, and STOP characters are 
 	passed through without being interpreted, and without generating a
 	signal.  The behaviour of the BREAK key depends on other
 	parameters of the terminal drive that are not set by curses.

 	The curses package does the "line-breakout optimisation" by
 	looking for type-ahead periodically while updating the screen.
 	If input is found, the current update will be postponed until
 	refresh() or doupdate() are called again.  This allows faster
 	response to commands typed in advance.  Normally, the input FILE
 	pointer passed to newterm(), or stdin in the case when initscr()
 	was called, will be used to do this type-ahead checking.  The
 	typeahead() routine specified that the file descriptor fd is to
 	be used to check for type-ahead instead.  If fd is -1, then no
 	type-ahead checking will be done.

  PDCurses Description:
 	The meta() function is provided for portability.  By default, 8 bits
 	are returned.

 	notimeout() is a no-op in PDCurses.

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      cbreak                                Y        Y       Y
      nocbreak                              Y        Y       Y
      echo                                  Y        Y       Y
      noecho                                Y        Y       Y
      halfdelay                             Y        Y       Y
      intrflush                             Y        Y       Y
      keypad                                Y        Y       Y
      meta                                  Y        Y       Y
      nodelay                               Y        Y       Y
      notimeout                             Y        Y       Y
      raw                                   Y        Y       Y
      noraw                                 Y        Y       Y
      noqiflush                             Y        Y       Y
      qiflush                               Y        Y       Y
      timeout                               Y        Y       Y
      wtimeout                              Y        Y       Y
      typeahead                             Y        Y       Y

**man-end**********************************************************************/

/***********************************************************************/
int	cbreak(void)
/***********************************************************************/
{
 _cursvar.cbreak = TRUE;
 return( OK );
}
/***********************************************************************/
int	nocbreak(void)
/***********************************************************************/
{
 _cursvar.cbreak = FALSE;
 return( OK );
}
/***********************************************************************/
int	echo(void)
/***********************************************************************/
{
 _cursvar.echo = TRUE;
 return( OK );
}
/***********************************************************************/
int	noecho(void)
/***********************************************************************/
{
 _cursvar.echo = FALSE;
 return( OK );
}
/***********************************************************************/
int	intrflush( WINDOW *win, bool bf )
/***********************************************************************/
{
	int	y;
	int	maxy;

	if (win == (WINDOW *)NULL)
		return( ERR );

	maxy = win->_maxy - 1;

	for (y = 0; y <= maxy; y++)
	{
		win->_firstch[y] = _NO_CHANGE;
	}
	return( OK );
}
/***********************************************************************/
int	keypad( WINDOW *win, bool bf )
/***********************************************************************/
{

	win->_use_keypad = bf;
	return( OK );
}
/***********************************************************************/
int	meta( WINDOW *win, bool bf )
/***********************************************************************/
{
	_cursvar.raw_inp = bf;
	return( OK );
}
/***********************************************************************/
int	nodelay( WINDOW *win, bool flag )
/***********************************************************************/
{

	win->_nodelay = flag;
	return( OK );
}
/***********************************************************************/
int	notimeout( WINDOW *win, bool flag )
/***********************************************************************/
{

	return( OK );
}
/***********************************************************************/
int	raw(void)
/***********************************************************************/
{
 _cursvar.raw_inp = TRUE;
 PDC_set_ctrl_break(FALSE);      /* disallow ^BREAK on disk I/O */
 return( OK );
}
/***********************************************************************/
int	noraw(void)
/***********************************************************************/
{
	_cursvar.raw_inp = FALSE;
	PDC_set_ctrl_break(TRUE);
	return( OK );
}
/***********************************************************************/
int	typeahead( int fildes )
/***********************************************************************/
{
	if (fildes < 0)
		_cursvar.refrbrk = FALSE;
	else
		_cursvar.refrbrk = TRUE;
	return(OK);
}
