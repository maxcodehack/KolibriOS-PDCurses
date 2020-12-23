#include <stdio.h>
#include <signal.h>
#include <curses.h>
#include <ctype.h>
#define DELAYSIZE 15

int main()
{
       int start,end,row,diff,flag,direction,seed;
       int myrefresh();
       void explode();

       initscr();
       nodelay( stdscr, TRUE );
       noecho();
       if (has_colors())
          start_color();
       seed = __menuet__getsystemclock();
       srand(seed);
       while(1)
               {
	       PDC_win_check_events();
                do {
                       start = rand() % (COLS -3);
                       end = rand() % (COLS - 3);
                       start = (start < 2) ? 2 : start;
                       end = (end < 2) ? 2 : end;
                       direction = (start > end) ? -1 : 1;
                       diff = abs(start-end);
                   } while (diff<2 || diff>=LINES-2);
                attrset(A_NORMAL);
                for (row=0;row<diff;row++)
                       {
                        mvprintw(LINES - row,start + (row * direction),
                               (direction < 0) ? "\\" : "/");
                        if (flag++)
                               {
                                myrefresh();
                                clear();
                                flag = 0;
                               }
                       }
               if (flag++)
                       {
                        myrefresh();
                        flag = 0;
                       }
               seed = __menuet__getsystemclock();
               srand(seed);
               explode(LINES-row,start+(diff*direction));
               clear();
               myrefresh();
              }
       endwin();
       exit(0);
}
void explode(row,col)
int row,col;
{
       clear();
       mvprintw(row,col,"-");
       myrefresh();

       init_pair(1,get_colour(),COLOR_BLACK);
       attrset(COLOR_PAIR(1));
       mvprintw(row-1,col-1," - ");
       mvprintw(row,col-1,"-+-");
       mvprintw(row+1,col-1," - ");
       myrefresh();

       init_pair(1,get_colour(),COLOR_BLACK);
       attrset(COLOR_PAIR(1));
       mvprintw(row-2,col-2," --- ");
       mvprintw(row-1,col-2,"-+++-");
       mvprintw(row,  col-2,"-+#+-");
       mvprintw(row+1,col-2,"-+++-");
       mvprintw(row+2,col-2," --- ");
       myrefresh();

       init_pair(1,get_colour(),COLOR_BLACK);
       attrset(COLOR_PAIR(1));
       mvprintw(row-2,col-2," +++ ");
       mvprintw(row-1,col-2,"++#++");
       mvprintw(row,  col-2,"+# #+");
       mvprintw(row+1,col-2,"++#++");
       mvprintw(row+2,col-2," +++ ");
       myrefresh();

       init_pair(1,get_colour(),COLOR_BLACK);
       attrset(COLOR_PAIR(1));
       mvprintw(row-2,col-2,"  #  ");
       mvprintw(row-1,col-2,"## ##");
       mvprintw(row,  col-2,"#   #");
       mvprintw(row+1,col-2,"## ##");
       mvprintw(row+2,col-2,"  #  ");
       myrefresh();

       init_pair(1,get_colour(),COLOR_BLACK);
       attrset(COLOR_PAIR(1));
       mvprintw(row-2,col-2," # # ");
       mvprintw(row-1,col-2,"#   #");
       mvprintw(row,  col-2,"     ");
       mvprintw(row+1,col-2,"#   #");
       mvprintw(row+2,col-2," # # ");
       myrefresh();
       return;
}
int myrefresh()
{
       napms(DELAYSIZE);
       move(LINES-1,COLS-1);
       refresh();
}

int get_colour()
{
 int attr, tmp;

       attr = (rand() % 8)+1;

       if (attr == 1)
          attr = COLOR_RED;  /* use red twice */
       else if (attr == 2)
          attr = COLOR_BLUE;
       else if (attr == 3)
          attr = COLOR_GREEN;
       else if (attr == 4)
          attr = COLOR_CYAN;
       else if (attr == 5)
          attr = COLOR_RED;
       else if (attr == 6)
          attr = COLOR_MAGENTA;
       else if (attr == 7)
          attr = COLOR_YELLOW;
       else if (attr == 8)
          attr = COLOR_WHITE;

       tmp = (rand() % 2)+1;
       if (tmp > 1)
          attr |= A_BOLD;
       return(attr);
}
