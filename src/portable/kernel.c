#define	CURSES_LIBRARY	1
#include <curses.h>

#include<menuet/textcon.h>

/* undefine any macros for functions defined in this module */
#undef	def_prog_mode
#undef	def_shell_mode
#undef	reset_prog_mode
#undef	reset_shell_mode
#undef	resetty
#undef	savetty
#undef	getsyx
#undef	setsyx
#undef	ripoffline
#undef	curs_set
#undef	napms

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#  undef	move
#  undef	wmove
#endif

#ifdef PDCDEBUG
char *rcsid_kernel  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                        kernel

  Synopsis:
  	int def_prog_mode(void);
  	int def_shell_mode(void);
  	int reset_prog_mode(void);
  	int reset_shell_mode(void);
  	int resetty(void);
  	int savetty(void);
  ***	int getsyx(int y, int x);
  ***	int setsyx(int y, int x);
  ***	int ripoffline(int line, init(*int)(WINDOW *,int));
  	int curs_set(int visibility);
  	int napms(int ms);

  X/Open Description:
 	The def_prog_mode() and def_shell_mode() functions save the 
 	current terminal modes as the "program" (in curses) or
 	"shell" (not in curses) state for use by the reset_prog_mode()
 	and reset_shell_mode() functions.  This is done automatically by
 	initscr().

 	The reset_prog_mode() and reset_shell_mode() functions restore 
 	the terminal to "program" (in curses) or "shell" (not in curses)
 	state.  These are done automatically by endwin()
 	and doupdate() after an endwin(), so they would normally not
 	be called before these functions.

 	The savetty() and resetty() routines save and restore the state of 
 	the terminal modes. The savetty() function saves the current state 
 	in a buffer and resetty() restores the state to what it was at the 
 	last call to savetty().

  PDCurses Description:
 	FYI: It is very unclear whether savetty() and resetty() functions
 	are a duplication of the reset_prog_mode() and reset_shell_mode() 
 	functions or whether this is a backing store type of operation.  
 	At this time, they are implemented similar to the reset_*_mode() 
 	routines.

 	The curs_set() routine is used to set the visibility of the cursor.
 	The cursor can be made invisible, normal or highly visible by setting
 	the parameter to 0, 1 or 2 respectively. If an invalid value is passed
 	the function will set the cursor to "normal".

  X/Open Return Value:
 	All functions return OK on success and ERR on error except curs_set()
 	which returns the previous visibility.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      def_prog_mode                         Y        Y       Y
      def_shell_mode                        Y        Y       Y
      reset_prog_mode                       Y        Y       Y
      reset_shell_mode                      Y        Y       Y
      resetty                               Y        Y       Y
      savetty                               Y        Y       Y
      getsyx                                -        -      3.0
      setsyx                                -        -      3.0
      ripoffline                            -        -      3.0
      curs_set                              -        -      3.0
      napms                                 Y        Y       Y

**man-end**********************************************************************/

/***********************************************************************/
int	def_prog_mode(void)
/***********************************************************************/
{
	c_pr_tty.been_set = TRUE;

	memcpy(&c_pr_tty.saved, &_cursvar, sizeof(SCREEN));

	return( OK );
}

/***********************************************************************/
int	def_shell_mode(void)
/***********************************************************************/
{
	c_sh_tty.been_set = TRUE;

	memcpy(&c_sh_tty.saved, &_cursvar, sizeof(SCREEN));

	return( OK );
}

/***********************************************************************/
int	reset_prog_mode(void)
/***********************************************************************/
{

	if	(c_pr_tty.been_set == TRUE)
	{

		memcpy(&_cursvar, &c_pr_tty.saved, sizeof(SCREEN));

		mvcur(0, 0, c_pr_tty.saved.cursrow, c_pr_tty.saved.curscol);
		if (PDC_get_ctrl_break() != c_pr_tty.saved.orgcbr)
			PDC_set_ctrl_break(c_pr_tty.saved.orgcbr);
		if (c_pr_tty.saved.raw_out)
			raw();
		if (c_pr_tty.saved.visible_cursor)
			PDC_cursor_on();
		_cursvar.font = PDC_get_font();
		PDC_set_font(c_pr_tty.saved.font);
		PDC_set_rows(c_pr_tty.saved.lines);
	}
	return( OK );
}

/***********************************************************************/
int	reset_shell_mode(void)
/***********************************************************************/
{

	if	(c_sh_tty.been_set == TRUE)
	{

		memcpy(&_cursvar, &c_sh_tty.saved, sizeof(SCREEN));

		mvcur(0, 0, c_sh_tty.saved.cursrow, c_sh_tty.saved.curscol);
		if (PDC_get_ctrl_break() != c_sh_tty.saved.orgcbr)
			PDC_set_ctrl_break(c_sh_tty.saved.orgcbr);
		if (c_sh_tty.saved.raw_out)
			raw();
		if (c_sh_tty.saved.visible_cursor)
			PDC_cursor_on();
		_cursvar.font = PDC_get_font();
		PDC_set_font(c_sh_tty.saved.font);
		PDC_set_rows(c_sh_tty.saved.lines);
	}
	return( OK );
}

/***********************************************************************/
int	resetty(void)
/***********************************************************************/
{
	return (OK);
}
/***********************************************************************/
int	savetty(void)
/***********************************************************************/
{
#ifdef PDCDEBUG
	if (trace_on) PDC_debug("savetty() - called\n");
#endif

	c_save_tty.been_set = TRUE;
	memcpy(&c_save_tty.saved, &_cursvar, sizeof(SCREEN));
	return( OK );
}
/***********************************************************************/
int	curs_set(int visibility)
/***********************************************************************/
{
 visible_console->cur_visible=visibility?1:0;
 _lcon_flushxy(visible_console->cur_x,visible_console->cur_y);
}
/***********************************************************************/
int	napms(int ms)
/***********************************************************************/
{
	return(delay_output(ms));
}
