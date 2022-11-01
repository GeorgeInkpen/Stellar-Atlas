// Stellar Atlas.h 
// G. Inkpen 2007-8

// Include headerfiles for standard libraries
#include <windows.h>
#include<stdio.h>
#include<conio.h>
#include <math.h>
#include "afxres.h"

// Define Constants (and make it easier to change them)
#define IDR_MYMENU 101
#define IDI_MYICON 201

#define ID_FILE_EXIT 9001

#define ID_DIRECTION_NORTH 9011	
#define ID_DIRECTION_EAST 9012
#define ID_DIRECTION_SOUTH 9013	
#define ID_DIRECTION_WEST 9014	

#define MAXSIZE 100
#define BUF_SIZE 100
#define M_PI       3.14159265358979323846

enum {IDC_PLANET=1000, IDC_LONG_DEG, IDC_LONG_MIN, IDC_LONG_SEC,
				IDC_LAT_DEG, IDC_LAT_MIN, IDC_LAT_SEC,
				IDC_YEAR, IDC_MONTH, IDC_DAY,
				IDC_HOUR, IDC_MIN, IDC_SEC
	};  

// Define the data structures to haold the various types of data
// Star data structure
struct star {char name[100]; int ra_hour; int ra_min; int ra_sec;
				int dec_deg; int dec_min; int dec_sec;
				float abs_mag; float app_mag;
				float dist; char spectrum; int spectrum_num; int index;
};
// Define two arrays of structures for raw data and altered data
// for if travelling to a new viewpoint outside the solar system
star raw_data[MAXSIZE],manip_data[MAXSIZE];

// Structure for plotting data to store calculations from map projection
struct starplot { float x; float y; float z;
				int plot_x; int plot_y;
};
starplot plot_data[MAXSIZE];

// Structure for observers data such as latitude time date etc
struct observer{ char planet[100];
				int long_deg; int long_min; int long_sec;
				int lat_deg; int lat_min; int lat_sec;
				int year; char month_name[30]; int month; int day;
				int hour; int min; int sec;
};
observer observe;

// Global variable for total number of stars read in from star database
int numstars;

// Include function prototypes
LRESULT WINAPI WindowProc(HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam);
int paint( HWND hWnd, HDC hDC);
   
int initialise_data(HWND hWnd);
int dropdownlist(HWND hWnd, LPARAM lParam);
int convert_data(void);
int display_data(HWND hWnd, HDC hDC);
int getuserposition (HWND hWnd, WPARAM wParam);
int	move_to_star(HWND hWnd);
int convert_month_to_num (void);