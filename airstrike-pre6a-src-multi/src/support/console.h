#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_LINES 12
#define CONSOLE_COLUMNS 35

int console_setup();
void console_set_pos(int x, int y);
void console_load_bg(char *filename);
void console_draw();
void console_update();
void console_write(const char *s);
void console_clear();
char console_getch();
char *console_readline();


#endif
