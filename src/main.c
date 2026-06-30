#include <ncurses.h>
#include <math.h>

#define M_PI 3.14159265358979323846

int main() {
    int row, col;
	initscr(); noecho();
    getmaxyx(stdscr,row,col);
    
    int xc=row/2;
    int yc=col/2;
    int a=9;
    int b=15;
    int max=(a > b) ? a : b;
    float step = 1.0/max;
    for (float theta =0.0; theta < 2 * M_PI; theta += step) {
        int x = (int)(xc + a*cos(theta) + 0.5);
        int y = (int)(yc + b*sin(theta) + 0.5);
        mvprintw(x, y, "+");
        if (x==17) {
            mvprintw(x+5, y+2, "Mercury");
        }
    }
    
	refresh();
	getch();
	endwin();
	return 0;
}
