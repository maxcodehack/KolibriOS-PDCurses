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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#define	CURSES_LIBRARY	1
#include <curses.h>
#ifdef UNIX
#include <defs.h>
#include <term.h>
#endif

/* undefine any macros for functions defined in this module */
#undef	baudrate
#undef	erasechar
#undef	has_ic
#undef	has_il
#undef	killchar
#undef	longname
#undef	termattrs
#undef	termname
#undef	wordchar

/* undefine any macros for functions called by this module if in debug mode */
#ifdef PDCDEBUG
#endif

#ifdef PDCDEBUG
char *rcsid_termattrs  = "$Id$";
#endif

/*man-start*********************************************************************

  Name:                                                     termattrs

  Synopsis:
  	int baudrate(void);
  	char erasechar(void);
  	bool has_ic(void);
  	bool has_il(void);
  	char killchar(void);
  	char *longname(void);
  	chtype termattrsr(void);
  	char *termname(void);

  	char	wordchar(void);

  X/Open Description:
 	baudrate() returns the output speed of the terminal.  The number 
 	returned is bits per second, for example 9600, and is an integer.

 	The user's current ERASE character is returned from a call to
 	erasechar();

 	The has_ic function will return TRUE if the terminal has insert 
 	character and delete character capabilities.

 	The has_il() function will return TRUE if the terminal has insert
 	line and delete line capabilities.

 	The user's current KILL character is returned from a call to
 	killchar();

 	longname() returns a pointer to a static area containing a
 	verbose description of the current terminal.  The maximum length
 	of the string is 128 characters.  It is defined only after the
 	call to initscr() or newterm().  The area is overwritten by each
 	call to newterm() and is not restored by set_term().  The value
 	should therefore be saved between calls to newterm(), if
 	longname() is going to be used with multiple terminals.

 	termname() returns a pointer to a static area containing a
 	short description of the current terminal (14 characters).

 	termattrs() returns a logical OR of all video attributes
 	supported by the terminal.

  PDCurses Description:
 	If FAST_VIDEO is true, then this is the largest possible
 	(portable) int value (INT_MAX from limits.h) IF direct
 	video is possible, OR the approximate guess at BIOS speeds,
 	19200.
 	If FAST_VIDEO is false, this is an approximate guess at BIOS
 	speeds, 19200.

 	erasechar(), killchar() and wordchar() all return values that are 
 	hardcoded at this time.  There may be future development
 	to allow applications an easy way to specify these value.

 	For DOS, has_ic() and has_il() always return TRUE, as the speed of
 	the DIRECT VIDEO or even BIOS writes is quicker than on a
 	serial terminal.
 	For FLEXOS, this value has meaning and depends on the terminal,
 	though at this time, it is a constant TRUE.

 	In addition to the above definition for longname(), the form of 
 	this string is the adapter name (or video card name) and the text 
 	resolution. This may also be followed by the notation that the video 
 	card may be a clone, which indicates that the card identification
 	maps to more than one unique card.
 	e.g. The MDS Genius and the Quadram QuadHPG identify themselves
 	in the same manner, but are vastly different in maximum resolution.

 	The user's current WORD character is returned from a call to
 	wordchar();

  X/Open Return Value:
 	All functions return OK on success and ERR on error.

  X/Open Errors:
 	No errors are defined for this function.

  Portability                             X/Open    BSD    SYS V
                                          Dec '88
      baudrate                              Y        Y       Y
      erasechar                             Y        Y       Y
      has_ic                                Y        Y       Y
      has_il                                Y        Y       Y
      killchar                              Y        Y       Y
      longname                              Y        Y       Y
      termattrs                             Y        Y       Y
      termname                              Y        Y       Y
      wordchar                              -        -       -

**man-end**********************************************************************/

#ifdef	FLEXOS
extern	char*	_flexos_gname();
#endif
static	char	_display[ 128 ];
static	char	_shrtnme[ 14 ];

/***********************************************************************/
int	baudrate(void)
/***********************************************************************/
{
	return (19200);			/* Approx. guess at BIOS speeds.*/
}
/***********************************************************************/
char	erasechar(void)
/***********************************************************************/
{
	return(	_ECHAR );	/* character delete char (^H) */
}
/***********************************************************************/
bool	has_ic(void)
/***********************************************************************/
{
	return( TRUE );
}
/***********************************************************************/
bool	has_il(void)
/***********************************************************************/
{
	return( TRUE );
}
/***********************************************************************/
char	killchar(void)
/***********************************************************************/
{
	return( _DLCHAR );	/* line delete char (^U) */
}
/***********************************************************************/
char*	longname(void)
/***********************************************************************/
{
	sprintf(_display,"MenuetOS Virtual Text Console Interface. Author: Jarek Pelczar");
	return (_display);
}
/***********************************************************************/
chtype	termattrs(void)
/***********************************************************************/
{
	chtype temp =   (A_NORMAL | A_BOLD) | A_BLINK;


	if( ! _cursvar.mono )
		temp |= A_COLOR;

	if( COLORS < 1 )
		temp |= A_REVERSE;

	return( temp );
}
/***********************************************************************/
char*	termname(void)
/***********************************************************************/
{
	sprintf(_shrtnme,"MenuetVTC");
	return (_shrtnme);
}
/***********************************************************************/
char	wordchar(void)
/***********************************************************************/
{
	return (_DWCHAR);			/* word delete char */
}
