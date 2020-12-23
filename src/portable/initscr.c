#define	CURSES_LIBRARY	1
#define	LOCAL_VAR
#include <curses.h>
#include <menuet/textcon.h>

#ifndef NO_MEMORY_H
#include <memory.h>
#endif

/* undefine any macros for functions defined in this module */
#undef	initscr
#undef	endwin
#undef	isendwin
#undef	newterm
#undef	set_term
#undef	delscreen
#undef	resize_screen

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	move
#  undef	wmove
#  undef	resize_window
#  undef	touchwin
#  undef	wnoutrefresh
#endif


#ifdef PDCDEBUG
char *rcsid_initscr  = "$Id$";
#else
char*	_curses_notice = "PDCurses 2.2 - Public Domain 1994";
#endif

SCREEN _cursvar = {0};		/* curses variables		*/

WINDOW*	curscr=NULL;			/* the current screen image	*/
WINDOW*	stdscr=NULL;			/* the default screen window	*/
int	_default_lines = 25;	/* default terminal height	*/
int	LINES=0;			/* current terminal height	*/
int	COLS=0;			/* current terminal width	*/

#if defined	DOS
Regs regs;
#endif

/*
 * Global definitions for charget routines
 */
int	c_pindex = 0;		/* putter index */
int	c_gindex = 1;		/* getter index */
int	c_ungind = 0;		/* wungetch() push index */
int	c_ungch[NUNGETCH];	/* array of ungotten chars */
WINDOW*	_getch_win_=NULL;

/*
 * Global definitions for setmode routines
 */
struct cttyset c_sh_tty = {0};	/* tty modes for def_shell_mode */
struct cttyset c_pr_tty = {0};	/* tty modes for def_prog_mode  */
struct cttyset c_save_tty = {0};
struct cttyset c_save_trm = {0};

/*
 * Global definitions for printscan routines
 */
char c_printscanbuf[513];	/* buffer used during I/O */

/*
 * Global definitions for strget routines
 */
char *c_strbeg;

#if	EMALLOC
void*	emalloc( size_t );
void*	ecalloc( size_t, size_t );
void	efree( void* );
	
extern	void*	emalloc();	/* user's emalloc(size)		*/
extern	void*	ecalloc();	/* user's ecalloc(num,size)	*/
extern	void	efree();	/* user's efree(ptr)		*/
#endif


void*	(*mallc)();		/* ptr to some malloc(size)	*/
void*	(*callc)();		/* ptr to some ecalloc(num,size)*/
void	(*fre)();		/* ptr to some free(ptr)	*/
void*	(*reallc)();		/* ptr to some realloc(ptr,size)	*/

chtype *acs_map;

/*man-start*********************************************************************

  Name:                                                       initscr

  Synopsis:
  	WINDOW *initscr(void);
  	int endwin(void);
  ***	int isendwin(void);
  ***	SCREEN *newterm(char *type, FILE *outfd, FILE *infd);
  	SCREEN *set_term(SCREEN *new);
  ***	void delscreen(SCREEN *sp);

  ***	int resize_screen(int nlines);

  X/Open Description:
 	The first curses routine called should be initscr().  This will
 	determine the terminal type and initialize all curses data
 	structures.  The initscr() function also arranges that the
 	first call to refresh() will clear the screen.  If errors
 	occur, initscr() will write an appropriate error message to
 	standard error and exit.  If the program wants an indication
 	of error conditions, newterm() should be used instead of
 	initscr().

 	A program should always call endwin() before exiting or
 	escaping from curses mode temporarily.  This routine will
 	restore tty modes, move the cursor to the lower left corner
 	of the screen and reset the terminal into the proper non-visual
 	mode.  To resume curses after a temporary escape, refresh() or
 	doupdate() should be called.

 	A program which outputs to more than one terminal should use
 	newterm() for each terminal instead of initscr().  The newterm()
 	function should be called once for each terminal.  It returns a 
 	value of type SCREEN* which should be saved as a reference to that
 	terminal. The arguments are the type of of terminal to be used
 	in place of TERM (environment variable), a file pointer for output
 	to the terminal and another file pointer for input from the terminal.
 	The program must also call endwin() for each terminal no longer being
 	used.

 	This function is used to switch between different terminals.
 	The screen reference 'new' becomes the new current terminal.
 	The previous terminal is returned by the routine.  This is the
 	only routine which manipulates SCREEN pointers; all other routines
 	affect only the current terminal.

  PDCurses Description:
 	Due to the fact that newterm() does not yet exist in PDCurses,
 	there is no way to recover from an error in initscr().

  X/Open Return Value:
 	All functions return NULL on error, except endwin(), which
 	returns ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      initscr                               Y        Y       Y
      endwin                                Y        Y       Y
      isendwin                              -        -      3.0
      newterm                               -        -       Y
      set_term                              -        -       Y
      delscreen                             -        -      4.0

**man-end**********************************************************************/

/***********************************************************************/
WINDOW*	initscr(void)
/***********************************************************************/
{
 register int i=0;
 if(_cursvar.alive) return( NULL);
 memset(&_cursvar, 0, sizeof(SCREEN));
 mallc = malloc;
 callc = calloc;
 fre   = free;
 reallc = realloc;
 PDC_init_menuetos();
 PDC_scr_open(&_cursvar, 0);
 _cursvar.orig_cursor = _cursvar.cursor;
 _cursvar.orig_font = _cursvar.font;
 _cursvar.orgcbr = PDC_get_ctrl_break();
 _cursvar.blank = ' ';
 LINES = _cursvar.lines;
 COLS = _cursvar.cols;
 if (LINES < 2 || COLS < 2)
 {
  exit( 4 );
 }
 if ((curscr = newwin(LINES, COLS, 0, 0)) == (WINDOW *) NULL)
 {
  exit( 2 );
 }
 if ((stdscr = newwin(LINES, COLS, 0, 0)) == (WINDOW *) NULL)
 {
  exit( 1 );
 }
 curscr->_clear = FALSE;
#ifdef	REGISTERWINDOWS
 _cursvar.refreshall = FALSE;
 _inswin(stdscr, (WINDOW *)NULL);
#endif

 if ((acs_map = (chtype *)(*mallc)(128*sizeof(chtype))) == (chtype *)NULL)
 {
  exit( 5 );
 }
 for (i=0;i<128;i++) acs_map[i] = i | A_ALTCHARSET;
 _cursvar.alive = TRUE;
 return( stdscr );
}

/***********************************************************************/
int	endwin(void)
/***********************************************************************/
{
 PDC_scr_close();
 _cursvar.visible_cursor = FALSE;	/* Force the visible cursor */
 _cursvar.cursor = _cursvar.orig_cursor;
 PDC_cursor_on();
 PDC_gotoxy(PDC_get_rows() - 2, 0);
 delwin(stdscr);
 delwin(curscr);
 stdscr = (WINDOW *)NULL;
 curscr = (WINDOW *)NULL;
 _cursvar.alive = FALSE;
 return( OK );
}

/***********************************************************************/
SCREEN*	set_term( SCREEN *new )
/***********************************************************************/
{
	return( &_cursvar );  /* We only have one screen supported right now */
}
/***********************************************************************/
int	resize_screen(int nlines)
/***********************************************************************/
{
 WINDOW*	tmp=NULL;
 if (stdscr == (WINDOW *)NULL) return(ERR);
 if (nlines==NR_CHARS_Y) return (OK);
 return (ERR);
}
