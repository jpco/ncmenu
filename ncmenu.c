#include <ncurses.h>
#include <string.h>

static int idx = 1;
static int num_elems = 0;
static WINDOW *win;
static char **elems;
static char *title;

void mkwindow (int maxy, int maxx)
{
        int maxstrw = 0;
        int i;
        for (i = 0; i < num_elems; i++) {
                int currstrw = strlen(elems[i]);
                if (maxstrw < currstrw) maxstrw = currstrw;
        }

        int winx = maxx/2 - maxstrw/2;
        int winy = maxy/2 - num_elems;

        win = newwin(1+num_elems*2, maxstrw+4, winy, winx-2);
        box(win, 0, 0);
        wrefresh(win);
}

void printmenu (void)
{
        int i;
        wattron(win, A_BOLD);
        for (i = 0; i < num_elems; i++) {
                if (i == idx) {
                        wattron(win, A_STANDOUT);
                        mvwprintw(win, i*2, 2, "%s", elems[i]);
                        wattroff(win, A_STANDOUT);
                } else {
                        mvwprintw(win, i*2, 2, "%s", elems[i]);
                }
        }
        wattroff(win, A_BOLD);
        move(0,0);
        wrefresh(win);
}

void keyloop()
{
        while (1) {
                short c = getch();
                switch (c) {
                        case '':
                                idx = 0;
                        case 10:
                                return;
                        case 'K':
                        case 'k':
                                if (idx > 1) {
                                        idx--;
                                        printmenu();
                                }
                                break;
                        case 'J':
                        case 'j':
                                if (idx < num_elems-1) {
                                        idx++;
                                        printmenu();
                                }
                                break;
                        default:
                                break;
                }
        }
}

int main (int argc, char **argv)
{
        if (argc < 2) return 1;

        initscr();
        noecho();
        cbreak();
        
        num_elems = argc-1;
        title = argv[1];
        elems = argv+1;

        int y,x;
        getmaxyx(stdscr, y, x);

        refresh();
        mkwindow(y, x);

        printmenu();
        keypad(win, TRUE);

        keyloop();

        endwin();

        FILE *out = fopen("ncm-tmp", "w");
        fprintf(out, "%d\n", idx);
        fclose(out);

        return 0;
}
