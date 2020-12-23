#define	CURSES_LIBRARY	1
#include <curses.h>
#include <menuet/textcon.h>

#ifdef PDCDEBUG
char *rcsid_PDCgetsc  = "$Id$";
#endif

/*man-start*********************************************************************

  PDC_get_cursor_pos()	- return current cursor position

  PDCurses Description:
 	This is a private PDCurses function

 	Gets the cursor position in video page 0.  'row' and 'column'
 	are the cursor address.  At this time, there is no support for
 	use of multiple screen pages.

  PDCurses Return Value:
 	This routine will return OK upon success and otherwise ERR will be
 	returned.

  PDCurses Errors:
 	There are no defined errors for this routine.

  Portability:
 	PDCurses	int	PDC_get_cursor_pos( int* row, int* col );

**man-end**********************************************************************/

int	PDC_get_cursor_pos(int *row, int *col)
{
	if(col) *row=visible_console->cur_x;
	if(row) *row=visible_console->cur_y;
	return( OK );
}

/*man-start*********************************************************************

  PDC_get_cur_col()	- get current column position of cursor

  PDCurses Description:
 	This is a private PDCurses function

 	This routine returns the current column position of the cursor on
 	screen.

  PDCurses Return Value:
 	This routine returns the current column position of the cursor. No
 	error is returned.

  PDCurses Errors:
 	There are no defined errors for this routine.

  Portability:
 	PDCurses	int	PDC_get_cur_col( void );

**man-end**********************************************************************/

int	PDC_get_cur_col(void)
{
 return visible_console->cur_x;
}

/*man-start*********************************************************************

  PDC_get_cur_row()	- get current row position of cursor

  PDCurses Description:
 	This is a private PDCurses function

 	This routine returns the current row position of the cursor on
 	screen.

  PDCurses Return Value:
 	This routine returns the current row position of the cursor. No
 	error is returned.

  PDCurses Errors:
 	There are no defined errors for this routine.

  Portability:
 	PDCurses	int	PDC_get_cur_row( void );

**man-end**********************************************************************/

int	PDC_get_cur_row(void)
{
 return visible_console->cur_y;
}

/*man-start*********************************************************************

  PDC_get_attribute()	- Get attribute at current cursor

  PDCurses Description:
 	This is a private PDCurses function

 	Return the current attr at current cursor position on the screen.

  PDCurses Return Value:
 	This routine will return OK upon success and otherwise ERR will be
 	returned.

  PDCurses Errors:
 	There are no defined errors for this routine.

  Portability:
 	PDCurses	int	PDC_get_attribute( void );

**man-end**********************************************************************/

int	PDC_get_attribute(void)
{
 return 0x73;
}

/*man-start*********************************************************************

  PDC_get_columns()	- return width of screen/viewport.

  PDCurses Description:
 	This is a private PDCurses function

 	This function will return the width of the current screen.

  PDCurses Return Value:
 	This routine will return OK upon success and otherwise ERR will be
 	returned.

  PDCurses Errors:
 	There are no defined errors for this routine.

  Portability:
 	PDCurses	int	PDC_get_columns( void );

**man-end**********************************************************************/

int	PDC_get_columns(void)
{
 return NR_CHARS_X;
}

/*man-start*********************************************************************

  PDC_get_cursor_mode()	- Get the cursor start and stop scan lines.

  PDCurses Description:
 	Gets the cursor type to begin in scan line startrow and end in
 	scan line endrow.  Both values should be 0-31.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_get_cursor_mode( void );

**man-end**********************************************************************/

int	PDC_get_cursor_mode(void)
{
	return(0);/* this is N/A */
}

/*man-start*********************************************************************

  PDC_get_font()	- Get the current font size

  PDCurses Description:
 	This is a private PDCurses routine.

 	This function returns the current font size.  This function only
 	works if the #define FAST_VIDEO is true.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	An ERR will be returned if FAST_VIDEO is not true.

  Portability:
 	PDCurses	int PDC_get_font( void );

**man-end**********************************************************************/

int	PDC_get_font(void)
{
	return(0); /* this is N/A */
}

/*man-start*********************************************************************

  PDC_get_rows()	- Return number of screen rows.

  PDCurses Description:
 	This is a private PDCurses routine.

 	Returns the maximum number of rows supported by the display.
 	e.g.  25, 28, 43, 50, 60, 66...

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_get_rows( void );

**man-end**********************************************************************/

int	PDC_get_rows(void)
{
return NR_CHARS_Y;
}

/*man-start*********************************************************************

  PDC_get_scrn_mode()	- Return the current BIOS video mode

  PDCurses Description:
 	This is a private PDCurses routine.


  PDCurses Return Value:
 	Returns the current BIOS Video Mode Number.

  PDCurses Errors:
 	The FLEXOS version of this routine returns an ERR.
 	The UNIX version of this routine returns an ERR.
 	The EMXVIDEO version of this routine returns an ERR.

  Portability:
 	PDCurses	int PDC_get_scrn_mode( void );

**man-end**********************************************************************/

int	PDC_get_scrn_mode(void)
{
	return(ERR); /* this is N/A */
}

/*man-start*********************************************************************

  PDC_query_adapter_type()	- Determine PC video adapter type

  PDCurses Description:
 	This is a private PDCurses routine.

 	Thanks to Jeff Duntemann, K16RA for providing the impetus
 	(through the Dr. Dobbs Journal, March 1989 issue) for getting
 	the routines below merged into Bjorn Larsson's PDCurses 1.3...
 		-- frotz@dri.com	900730

  PDCurses Return Value:
 	This function returns a macro identifier indicating the adapter
 	type.  See the list of adapter types in CURSPRIV.H.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_query_adapter_type( void );

**man-end**********************************************************************/

int	PDC_query_adapter_type(void)
{
 _cursvar.mono=TRUE;
 return _VGAMONO;
}

/*man-start*********************************************************************

  PDC_sanity_check() - A video adapter identification sanity check

  PDCurses Description:
 	This is a private PDCurses routine.

 	This routine will force sane values for various control flags.

  PDCurses Return Value:
 	This function returns OK on success and ERR on error.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_sanity_check( int adapter );

**man-end**********************************************************************/

int	PDC_sanity_check(int adapter)
{
	int	fontsize = PDC_get_font();
	int	rows	 = PDC_get_rows();
	_cursvar.bogus_adapter = FALSE;
	return (adapter);
}
