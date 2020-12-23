#define	CURSES_LIBRARY	1
#include <curses.h>


/*man-start*********************************************************************

  PDC_breakout()	- check for type-ahead

  X/Open Description:
 	The curses package does the "line-breakout optimisation" by
 	looking for type-ahead periodically while updating the screen.
 	If input is found, the current update will be postponed until
 	refresh() or doupdate() are called again.  This allows faster
 	response to commands typed in advance.  Normally, the input FILE
 	pointer passed to newterm(), or stdin in the case when initscr()
 	was called, will be used to do this type-ahead checking.  This routine
 	will do the actual check for PDcurses to see if input is pending.

  PDCurses Description:
 	This routine does what the PDcurses typeahead routine used to do.  But
 	to make that routine consistent with its System V counterpart, all it
 	does now is set whether or not typeahead checking is performed.

  X/Open Return Value:
 	The PDC_breakout() routine returns TRUE if keyboard input is pending
 	otherwise FALSE is returned.

  Portability:
 	PDCurses	bool PD_breakout( void );

**man-end**********************************************************************/

bool	PDC_breakout( void )
{
 return FALSE;
}

/*man-start*********************************************************************

  PDC_check_bios_key()	- Check BIOS key data area for input

  PDCurses Description:
 	This is a private PDCurses routine.

 	This routine will check the BIOS for any indication that
 	keystrokes are pending.

  PDCurses Return Value:
 	Returns 1 if a keyboard character is available, 0 otherwise.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	bool	PDC_check_bios_key( void );

**man-end**********************************************************************/

bool PDC_check_bios_key(void)
{
 if(PDC_check_kbq()) return TRUE; else return FALSE;
}         

/*man-start*********************************************************************

  PDC_get_bios_key()	- Returns the next key available from the BIOS.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Returns the next key code struck at the keyboard. If the low 8
 	bits are 0, the upper bits contain the extended character
 	code. If bit 0-7 are non-zero, the upper bits = 0.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_get_bios_key( void );

**man-end**********************************************************************/

int	PDC_get_bios_key(void)
{
 return PDC_getmchar();
}

/*man-start*********************************************************************

  PDC_get_ctrl_break()	- return OS control break state

  PDCurses Description:
 	This is a private PDCurses routine.

 	Returns the current OS Control Break Check state.

  PDCurses Return Value:
 	DOS:
 		This function returns TRUE on if the Control Break
 		Check is enabled otherwise FALSE is returned.

 	FLEXOS:
 		This function returns TRUE on if the Keyboard Mode
 		allows a break to bre reported otherwise FALSE is returned.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	bool	PDC_get_ctrl_break( void );

**man-end**********************************************************************/

bool	PDC_get_ctrl_break(void)
{
 return FALSE;
}

/*man-start*********************************************************************

  PDC_rawgetch()	- Returns the next uninterpreted character (if available).

  PDCurses Description:
 	Gets a character without any interpretation at all and returns
 	it. If keypad mode is active for the designated window,
 	function key translation will be performed.  Otherwise,
 	function keys are ignored.  If nodelay mode is active in the
 	window, then PDC_rawgetch() returns -1 if no character is
 	available.

 	WARNING:  It is unknown whether the FUNCTION key translation
 		  is performed at this level. --Frotz 911130 BUG

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int	PDC_rawgetch( void );

**man-end**********************************************************************/

int	PDC_rawgetch(void)
{
 return PDC_getmchar();
}

/*man-start*********************************************************************

  PDC_set_ctrl_break()	- Enables/Disables the host OS BREAK key check.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Enables/Disables the host OS BREAK key check. This function toggles
 	the BREAK setting. If it was on, it turns itoff; if it was aff it turns
 	it on.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_set_ctrl_break( bool setting );

**man-end**********************************************************************/

int	PDC_set_ctrl_break(bool setting)
{
return (OK);
}

/*man-start*********************************************************************

  PDC_sysgetch()	- Return a character using default system routines.

  PDCurses Description:
 	This is a private PDCurses function.

 	Gets a character without normal ^S, ^Q, ^P and ^C interpretation
 	and returns it.  If keypad mode is active for the designated
 	window, function key translation will be performed. Otherwise,
 	function keys are ignored. If nodelay mode is active in the
 	window, then sysgetch() returns -1 if no character is
 	available.

  PDCurses Return Value:
 	This function returns OK upon success otherwise ERR is returned.

  PDCurses Errors:
 	No errors are defined for this routine.

  Portability:
 	PDCurses	int	PDC_sysgetch( void );

**man-end**********************************************************************/


int	PDC_sysgetch(void)
{
extern	WINDOW*	_getch_win_;
/* extern	WINDOW*	w;*/   /* w defined in wgetch() as static - _getch_win_ */
                        /* is the same window - all references to w changed*/
                        /* to _getch_win_ - marked with @@ */

	int c=0;

#ifdef UNIX
	void (*oldsig)();
	void sigalrm();
	char keybuf[10];
	char *bufp=keybuf;
	int val=0;
	register int i=0;
#define MATCH_FALSE 0
#define MATCH_PARTIAL 1
#define MATCH_TRUE 2
	char match=0;
#endif

#ifdef PDCDEBUG
	if (trace_on) PDC_debug("PDC_sysgetch() - called\n");
#endif

	if (_getch_win_ == (WINDOW *)NULL)  /* @@ */
		return (-1);

	if (_getch_win_->_nodelay && !PDC_breakout())
		return (-1);

	while (1)
	{
		c = PDC_get_bios_key();
		if ((unsigned int)c < 256) return(c);
	}
}


/*man-start*********************************************************************

  PDC_validchar()	- validate/translate passed character
  
  PDCurses Description:
  	This is a private PDCurses function.
  
  	Checks that 'c' is a valid character, and if so returns it,
  	with function key translation applied if 'w' has keypad mode
  	set.  If char is invalid, returns -1.
  
  PDCurses Return Value:
  	This function returns -1 if the passed character is invalid, or
  	the WINDOW* 'w' is NULL, or 'w's keypad is not active.
  
  	This function returns 0 under Flexos if the passed character
  	is 0x0300.  (-:Forget why.  --Frotz	911130:-)
  
  	Otherwise, this function returns the PDCurses equivalent of the
  	passed character.  See the function key and key macros in
  	<curses.h>
  
  PDCurses Errors:
  	There are no errors defined for this routine.
  
  Portability:
  	PDCurses	int	PDC_validchar( int c );

**man-end**********************************************************************/

int	PDC_validchar(int c)
{
	if ((unsigned int)c < 256)	return (c);  /* normal character */
	return( -1 );		/* not found, invalid */
}
