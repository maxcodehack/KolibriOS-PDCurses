#include <stdarg.h>
#include <string.h>
#define	CURSES_LIBRARY	1
#include <curses.h>
#undef	PDC_debug

bool trace_on = FALSE;

/*man-start*********************************************************************

  PDC_debug()	- Write debugging info to log file.

  PDCurses Description:
 	This is a private PDCurses routine.

  PDCurses Return Value:
 	No return value.

  PDCurses Errors:
 	No errors are defined for this function.

  Portability:
 	PDCurses	void PDC_debug( char *,... );

**man-end**********************************************************************/

void	PDC_debug( char *fmt, ... )
{
	va_list args;
FILE *dbfp=NULL;
char buffer[256]="";

/*
 * open debug log file append
*/
	if (!trace_on)
		return; 
	dbfp = fopen("trace","a");
	if (dbfp == NULL)
	{
		fprintf( stderr, "PDC_debug(): Unable to open debug log file\n" );
		exit( 5 );
	}

	va_start(args, fmt);
	vsprintf(buffer,fmt,args);
	fputs(buffer,dbfp);
	va_end(args);
	fclose(dbfp);
	return;
}
