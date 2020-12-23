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

#ifdef PDCDEBUG
char *rcsid_PDCscrn  = "$Id$";
#endif

/*man-start*********************************************************************

  PDC_scr_close()	- Internal low-level binding to close the physical screen

  PDCurses Description:
 	This function provides a low-level binding for the Flexos
 	platform which must close the screen before writing to it.
 	This is a nop for the DOS platform.

 	This function is provided in order to reset the FlexOS 16 bit
 	character set for input rather than the limited input
 	character set associated with the VT52.

  PDCurses Return Value:
 	This function returns OK on success, otherwise an ERR is returned.

  PDCurses Errors:
 	The DOS platform will never fail.  The Flexos platform may fail
 	depending on the ability to close the current virtual console in
 	8 (as opposed to 16) bit mode.

  Portability:
 	PDCurses	int	PDC_scr_close( void );

**man-end**********************************************************************/

int	PDC_scr_close(void)
{

	return( OK );
}

/*man-start*********************************************************************

  PDC_scrn_modes_equal()	- Decide if two screen modes are equal

  PDCurses Description:
 	Mainly required for OS/2. It decides if two screen modes
        (VIOMODEINFO structure) are equal. Under DOS it just compares
        two integers

  PDCurses Return Value:
 	This function returns TRUE if equal else FALSe.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	int PDC_scrn_modes_equal( int mode1, int mode2 );
 	OS2 PDCurses	int PDC_scrn_modes_equal( VIOMODEINFO mode1, VIOMODEINFO mode2 );

**man-end**********************************************************************/

bool	PDC_scrn_modes_equal(int mode1, int mode2)
{
	return (mode1 == mode2);
}

/*man-start*********************************************************************

  PDC_scr_open()	- Internal low-level binding to open the physical screen

  PDCurses Description:
 	This function provides a low-level binding for the Flexos
 	platform which must open the screen before writing to it.

 	This function is provided in order to access the FlexOS 16 bit
 	character set for input rather than the limited input
 	character set associated with the VT52.

  PDCurses Return Value:
 	This function returns OK on success, otherwise an ERR is returned.

  PDCurses Errors:
 	The DOS platform will never fail.  The Flexos platform may fail
 	depending on the ability to open the current virtual console in
 	8 (as opposed to 16) bit mode.

  Portability:
 	PDCurses	int	PDC_scr_open( SCREEN* internal, bool echo );

**man-end**********************************************************************/

int	PDC_scr_open(SCREEN *internal, bool echo)
{
	PDC_get_cursor_pos(&internal->cursrow, &internal->curscol);
	internal->direct_video	= TRUE;		/* Assume that we can	      */
	internal->autocr	= TRUE;		/* lf -> crlf by default      */
	internal->raw_out	= FALSE;	/* tty I/O modes	      */
	internal->raw_inp	= FALSE;	/* tty I/O modes	      */
	internal->cbreak	= TRUE;
	internal->echo		= echo;
	internal->refrbrk	= TRUE;	/* allow premature end of refresh*/
	internal->video_page	= 0;		/* Current Video Page	      */
	internal->visible_cursor= TRUE;		/* Assume that it is visible  */
	internal->cursor	= PDC_get_cursor_mode();
	internal->adapter	= PDC_query_adapter_type();
	internal->font		= PDC_get_font();
	internal->lines		= PDC_get_rows();
	internal->cols		= PDC_get_columns();
	internal->audible	= TRUE;
	internal->visibility	= 1;
	return( OK );
}
