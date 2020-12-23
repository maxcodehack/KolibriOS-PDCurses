#include<menuet/os.h>
#include<menuet/textcon.h>

static char * WinTitle="MenuetCurses Application";

typedef struct
{
	unsigned char *data;
	unsigned size, in_ptr, out_ptr;
} queue_t;

static int inq(queue_t *q, unsigned data)
{
	unsigned temp;

	temp = q->in_ptr + 1;
	if(temp >= q->size)
		temp = 0;
/* if in_ptr reaches out_ptr, the queue is full */
	if(temp == q->out_ptr)
		return -1;
	q->data[q->in_ptr] = data;
	q->in_ptr = temp;
	return 0;
}

static int deq(queue_t *q, unsigned char *data)
{
/* if out_ptr reaches in_ptr, the queue is empty */
	if(q->out_ptr == q->in_ptr)
		return -1;
	*data = q->data[q->out_ptr++];
	if(q->out_ptr >= q->size)
		q->out_ptr = 0;
	return 0;
}

static int empty(queue_t *q)
{
	return q->out_ptr == q->in_ptr;
}

#define	BUF_SIZE	64

static unsigned char g_kbd_buf[BUF_SIZE];
static queue_t g_queue =
{
	g_kbd_buf, BUF_SIZE, 0, 0
};

static int did_con_init=0;

static void paint_pdc_wnd(void)
{
 __menuet__window_redraw(1);
 __menuet__define_window(100,100,20+(NR_CHARS_X*CHAR_SIZE_X),
    30+(NR_CHARS_Y*CHAR_SIZE_Y),0x03000080,0x800000FF,0x000080);
 __menuet__write_text(5,5,0xFFFFFF,WinTitle,strlen(WinTitle));
 if(did_con_init) _lcon_flush_console();
 __menuet__window_redraw(2);
}

void PDC_win_check_events(void)
{
 int i;
 i=__menuet__check_for_event();
 switch(i)
 {
  case 1:
   paint_pdc_wnd();
   break;
  case 2:
   inq(&g_queue,__menuet__getkey());
   break;
  case 3:
   if(__menuet__get_button_id()==1) exit(0);
   break;
 }
}

void PDC_win_wait_events(void)
{
 int i;
 i=__menuet__check_for_event();
 switch(i)
 {
  case 1:
   paint_pdc_wnd();
   break;
  case 2:
   inq(&g_queue,__menuet__getkey());
   break;
  case 3:
   if(__menuet__get_button_id()==1) exit(0);
   break;
 }
}

void PDC_init_menuetos(void)
{
 did_con_init=0;
 paint_pdc_wnd();
 init_consoles();
 did_con_init=1;
 PDC_win_check_events();
}

int PDC_getmchar(void)
{
 unsigned char p;
 if(deq(&g_queue,&p)!=0) 
 {
  PDC_win_check_events();
  if(deq(&g_queue,&p)!=0) return p;
  p=0;
 }
 return p;
}

int PDC_check_kbq(void)
{
 int i=empty(&g_queue);
 if(!i)
 {
  PDC_win_check_events();
  i=empty(&g_queue);
  if(!i) return 0;
 }
 return 1;  
}

void PDC_SetWinTitle(char * n)
{
 if(!n) return;
 WinTitle=n;
 paint_pdc_wnd();
 PDC_win_check_events();
}
