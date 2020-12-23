#define	CURSES_LIBRARY	1
#include <curses.h>

/* undefine any macros for functions defined in this module */
#undef	beep
#undef	flash

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	delay_output
#  undef	wrefresh
#endif

#ifdef PDCDEBUG
char *rcsid_beep  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                          beep

  Synopsis:
  	int beep(void);
  	int flash(void);

  X/Open Description:
 	These routines are used to signal the terminal user.  The beep()
 	function will sound the audible bell on the terminal, if possible
 	and if not, will flash the screen (visible bell), if possible.
 	The flash() function will flash the screen, and if that is not
 	possible, will sound the audible signal.  If neither signal is
 	possible, nothing will happen.  Nearly all terminals have an
 	audible signal (bell or beep), but only some can flash the screen.

  X/Open Return Value:
 	These functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for these functions.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      beep                                  Y        Y       Y
      flash                                 Y        Y       Y

**man-end**********************************************************************/

int	beep(void)
{
 return( OK );
}

/***********************************************************************/
int	flash(void)
/***********************************************************************/
{
	return( OK );
}
