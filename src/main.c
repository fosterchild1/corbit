#include <ncurses.h>
#include "../include/planet.h"
#include "../include/input.h"
#include "../include/simulation.h"
#include "../include/render.h"

float VIEW_ANGLE = 0;

int main() {
    int width, height;
    initscr(); noecho(); cbreak(); curs_set(0); start_color(); nodelay(stdscr, TRUE); keypad(stdscr, TRUE);
    getmaxyx(stdscr, height, width);

    InitBinds();
    Scene tes = {NULL, 0, 0, {width/2, height/2}, 0};
    
    /*OrbitParams t = {17, 3.4, 1.3, 0.6, 0.8, -0.3};
    Color tcolor = {255, 255, 0, 0};
    Planet te = CreatePlanet(&t, &tcolor, "test");
    AddToScene(&tes, &te);

    OrbitParams t2 = {30, 3.4, 1.3, 0.5, 0.8, 0.5};
    Color tcolor2 = {255, 0, 0, 0};
    Planet te2 = CreatePlanet(&t2, &tcolor2, "test2");
    AddToScene(&tes, &te2);*/
    // --- Mercury ---
OrbitParams mercury_orbit = {
    10.0,               // smaxis
    1.35176,            // lpe (~77.45°)
    0.84352,            // lan (~48.33°)
    3.05066,            // mna (~174.79°)
    0.2056f,            // eccentricity
    0.12217f            // inclination (~7.00°)
};
Color mercury_color = {169, 169, 169, 255};
Planet mercury = CreatePlanet(&mercury_orbit, &mercury_color, "Mercury");
AddToScene(&tes, &mercury);

// --- Venus ---
OrbitParams venus_orbit = {
    14.0, 
    2.29615,            // lpe (~131.56°)
    1.33832,            // lan (~76.68°)
    0.87458,            // mna (~50.11°)
    0.0067f, 
    0.05917f            // inclination (~3.39°)
};
Color venus_color = {224, 196, 140, 255};
Planet venus = CreatePlanet(&venus_orbit, &venus_color, "Venus");
AddToScene(&tes, &venus);

// --- Earth ---
OrbitParams earth_orbit = {
    18.0, 
    1.79647,            // lpe (~102.93°)
    0.00000,            // lan (0.0°)
    6.23993,            // mna (~357.52°)
    0.0167f, 
    0.00000f            // inclination (0.0°)
};
Color earth_color = {0, 128, 255, 255};
Planet earth = CreatePlanet(&earth_orbit, &earth_color, "Earth");
AddToScene(&tes, &earth);

// --- Mars ---
OrbitParams mars_orbit = {
    24.0, 
    5.86536,            // lpe (~336.06°)
    0.86500,            // lan (~49.56°)
    0.33859,            // mna (~19.40°)
    0.0934f, 
    0.03229f            // inclination (~1.85°)
};
Color mars_color = {193, 68, 14, 255};
Planet mars = CreatePlanet(&mars_orbit, &mars_color, "Mars");
AddToScene(&tes, &mars);

// --- Jupiter ---
OrbitParams jupiter_orbit = {
    36.0, 
    0.25011,            // lpe (~14.33°)
    1.75336,            // lan (~100.46°)
    0.34941,            // mna (~20.02°)
    0.0484f, 
    0.02269f            // inclination (~1.30°)
};
Color jupiter_color = {213, 172, 131, 255};
Planet jupiter = CreatePlanet(&jupiter_orbit, &jupiter_color, "Jupiter");
AddToScene(&tes, &jupiter);

// --- Saturn ---
OrbitParams saturn_orbit = {
    46.0, 
    1.61461,            // lpe (~92.51°)
    1.98426,            // lan (~113.69°)
    5.59115,            // mna (~320.35°)
    0.0541f, 
    0.04346f            // inclination (~2.49°)
};
Color saturn_color = {238, 222, 175, 255};
Planet saturn = CreatePlanet(&saturn_orbit, &saturn_color, "Saturn");
AddToScene(&tes, &saturn);

// --- Uranus ---
OrbitParams uranus_orbit = {
    56.0, 
    3.00947,            // lpe (~172.43°)
    1.29172,            // lan (~74.01°)
    2.48255,            // mna (~142.24°)
    0.0472f, 
    0.01344f            // inclination (~0.77°)
};
Color uranus_color = {209, 239, 242, 255};
Planet uranus = CreatePlanet(&uranus_orbit, &uranus_color, "Uranus");
AddToScene(&tes, &uranus);

// --- Neptune ---
OrbitParams neptune_orbit = {
    66.0, 
    0.81472,            // lpe (~46.68°)
    2.29999,            // lan (~131.78°)
    4.53575,            // mna (~259.88°)
    0.0086f, 
    0.03089f            // inclination (~1.77°)
};
Color neptune_color = {75, 112, 245, 255};
Planet neptune = CreatePlanet(&neptune_orbit, &neptune_color, "Neptune");
AddToScene(&tes, &neptune);
    
    while (true) {
        RenderScene(tes);
        StepSimulation(&tes, 1);
        HandleInput(&tes, getch());

        napms(16);
        refresh();
        erase();
    }

	endwin();
	return 0;
}
