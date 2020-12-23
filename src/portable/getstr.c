#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	getstr
#undef	wgetstr
#undef	mvgetstr
#undef	mvwgetstr
#undef	ungetstr
#undef	wgetnstr

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	wrefresh
#  undef	waddch
#  undef	wgetch
#  undef	cbreak
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_getstr  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                        getstr

  Synopsis:
  	int getstr(char *str);
  	int wgetstr(WINDOW *win, char *str);
  	int mvgetstr(int y, int x, char *str);
  	int mvwgetstr(WINDOW *win, int y, int x, char *str);
  	int wgetnstr(WINDOW *win, char *str, int ch);

  X/Open Description:
 	The effect of getstr() is as though a series of calls to getch()
 	were made, until a newline or carriage return is received. The
 	resulting value is placed in the area pointed to by *str. The user's
 	erase and kill characters are interpreted, as well as any special
 	keys; such as function keys.

 	With wgetnstr(), a series of characters are read until a newline 
 	or carriage return is received.  The resulting value is placed 
 	in the area pointed to by the character pointer str.  The user's 
 	erase and kill characters are interpreted.  This differs from 
 	wgetstr() in that the number of characters read is limited by a passed
 	argument.

 	NOTE: getstr(), mvgetstr() and mvwgetstr() are implemented as macros.

 	WARNING:  There is no way to know how long the buffer passed to
 		wgetstr() is, so it is possible to overwrite wrong
 		memory or code!! This is the reason for the wgetnstr()
 		function :-)

  X/Open Return Value:
 	This functions return ERR on failure or any other value on success.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      getstr                                Y        Y       Y
      wgtdch                                Y        Y       Y
      mvgetstr                              Y        Y       Y
      mvwgetstr                             Y        Y       Y
      wgetnstr                              -        -      4.0

**man-end**********************************************************************/

#define MAXLINE 255

/***********************************************************************/
int	getstr(char *str)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("getstr() - called\n");
#endif

	if (stdscr == (WINDOW *)NULL)
		return (ERR);
	return(wgetnstr(stdscr,str,MAXLINE));
}
/***********************************************************************/
int	wgetstr(WINDOW *win, char *str)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("wgetstr() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return (ERR);

	return(wgetnstr(win,str,MAXLINE));
}
/***********************************************************************/
int	mvgetstr(int y, int x, char *str)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvgetstr() - called\n");
#endif

	if (stdscr == (WINDOW *)NULL)
		return (ERR);
	if (move(y,x) == ERR)
		return (ERR);
	return(wgetnstr(stdscr,str,MAXLINE));
}
/***********************************************************************/
int	mvwgetstr(WINDOW *win, int y, int x, char *str)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("mvwgetstr() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return (ERR);
	if (wmove(win,y,x) == ERR)
		return (ERR);
	return(wgetnstr(win,str,MAXLINE));
}
/***********************************************************************/
int	wgetnstr(WINDOW *win, char *str, int n)
/***********************************************************************/
{
	int	ch, i, num,chars=0;
	int	t = win->_tabsize;
	int	x = win->_curx;
	char*	p = str;
	bool	stop = FALSE;
	bool	oldecho;
	bool	oldcbreak;
	bool	oldnodelay;

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("wgetnstr() - called\n");
#endif

	if (win == (WINDOW *)NULL)
		return (ERR);

	oldcbreak = _cursvar.cbreak;	/* remember states	 */
	oldecho = _cursvar.echo;
	oldnodelay = win->_nodelay;

	_cursvar.echo = FALSE;		/* we do echo ourselves	*/
	cbreak();		/* ensure each key is returned immediately */
	win->_nodelay = FALSE;		/* don't return	-1	 */

	wrefresh (win);

	while (!stop)
	{
	switch (ch = wgetch (win) & A_CHARTEXT)
		{
		case '\t':
			ch = ' ';
			num = t - (win->_curx - x)%t;
			for (i=0; i<num; i++)
				{
				if (oldecho) 
					waddch (win, ch);
				*p++ = ch;
				}
			chars += num;
			break;

		case _ECHAR:  /* CTRL-H */	 /* Delete character */
			if (p > str)
				{
				if (oldecho) 
					waddstr (win, "\b \b");
				ch = *--p;
				if ((ch < ' ') && (oldecho))
					waddstr (win, "\b \b");
				}
			chars--;
			break;

		case _DLCHAR:  /* CTRL-U */	 /* Delete line	*/
			while (p > str)
				{
				if (oldecho) 
					waddstr (win, "\b \b");
				ch = *--p;
				if ((ch < ' ') && (oldecho))
					waddstr (win, "\b \b");
				}
			chars = 0;
			break;

		case _DWCHAR: /* CTRL-W */	/* Delete word */
			while ((p > str) && (*(p-1) == ' '))
				{
				if (oldecho) 
					waddstr (win, "\b \b");
				--p; /* remove space */
				chars--;
				}
			while ((p > str) && (*(p-1) != ' '))
				{
				if (oldecho) 
					waddstr (win, "\b \b");
				ch = *--p;
				if ((ch < ' ') && (oldecho))
					waddstr (win, "\b \b");
				chars--;
				}
			break;

		case '\n':
		case '\r':
			stop = TRUE;
			if (oldecho) 
				waddch (win, '\n');
			break;

		default:
			*p++ = ch;
			if (oldecho) 
				waddch (win, ch);
			chars ++;
			break;
		}
	wrefresh (win);
	if (chars >= n)
		break;
	}
	*p = '\0';

	_cursvar.echo = oldecho;	/* restore old settings	*/
	_cursvar.cbreak = oldcbreak;
	win->_nodelay = oldnodelay;

	return (OK);
}
