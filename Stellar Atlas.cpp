// Stellar Atlas.cpp 
// G. Inkpen 2007-8 2022

// Include the header files
#include "D:\George\My Documents\UCE\Stellar Atlas\Stellar Atlas\Stellar Atlas.h"

// Main program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX WindowClass;			// Structure to hold our window's attributes
	static LPCTSTR szAppName = L"OFWin";      // Define window class name
	static LPCTSTR szAppTitle = L"Stellar Views" ;  // Define window name
	HWND hWnd;						// Window handle
	MSG msg;						// Windows message structure

	WindowClass.cbSize = sizeof(WNDCLASSEX);  // Set structure size
  
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;// Redraw the window if the size changes

	WindowClass.lpfnWndProc = WindowProc;	// Define the message handling function
	WindowClass.cbClsExtra = 0;				// No extra bytes after the window class
	WindowClass.cbWndExtra = 0;				// structure or the window instance
	WindowClass.hInstance = hInstance;      // Application instance handle
	// Set window cursor to be the standard arrow
	WindowClass.hCursor = LoadCursor(0, IDC_ARROW);

	// Set gray brush for background color
	WindowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));

	WindowClass.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
             // Menu as defined in Menu.rc
	WindowClass.lpszClassName = szAppName;    // Set class name defined above
	WindowClass.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));
    WindowClass.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);
		// Add my own icons as defined in Menu.rc file

	// Now register our window class
	RegisterClassEx(&WindowClass);

	// Now we can create the window 

	hWnd = CreateWindow( 
                           szAppName,           // Window Class name
                           szAppTitle,          // Window Title
                           WS_MAXIMIZE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME |
						   WS_MINIMIZEBOX | WS_MAXIMIZEBOX , // Window style
						   0,					// 0,0 for
                           0,					// of upper left X and Y
                           CW_USEDEFAULT,		// Default
                           CW_USEDEFAULT,       // for X and Y
                           0,                   // No Parent window
                           0,					// No menu id
                           hInstance,           // Program instance handle
                           0                    // No window creation data
                         ) ;


	ShowWindow(hWnd, nCmdShow);     // Display the window
	UpdateWindow(hWnd);             // Cause window client area to be drawn

	// The message loop
	while(GetMessage(&msg, 0, 0, 0) == TRUE)  // Get any messages
	{
		TranslateMessage(&msg);                 // Translate the message
		DispatchMessage(&msg);                  // Dispatch the message
	}

	return static_cast<int>(msg.wParam);      // End, so return to Windows
}

HWND planet,long_deg, long_min, long_sec,	// Window handles for combo boxes
		lat_deg, lat_min, lat_sec,
		year, month, day,
		hour, min, sec;	

LRESULT WINAPI WindowProc(HWND hWnd, UINT message, 
                          WPARAM wParam, LPARAM lParam)
{
	HDC hDC;                       // Display context handle
	PAINTSTRUCT PaintSt;           // Structure defining area to be drawn

	switch(message)				// Process selected messages
	{
	case WM_CREATE:			// Create the buttons as child windows

		initialise_data(hWnd);		// Load star data from file
		dropdownlist(hWnd, lParam);	// Add all the dropdown list boxes and button

		return 0;

	case WM_PAINT:			// Message is to redraw the window

		hDC = BeginPaint(hWnd, &PaintSt);	// Prepare to draw the window
		paint(hWnd, hDC);					// Function to paint background

		if (strcmp(observe.planet ,"EARTH") != 0)	// If observer not on Earth
			move_to_star(hWnd);			// Move to star not programmed yet
		else {					// Else will go when move to star is written
		convert_data();			// Manthematical calculations to get x,y co-ords
		display_data(hWnd, hDC);	// Draw the stars
		}
		EndPaint(hWnd, &PaintSt);			// Terminate window redraw operation
		return 0;

	case WM_COMMAND:		// Process menus and button clicks

		if (LOWORD(wParam) == ID_FILE_EXIT) {	//End program
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		//These message boxes will be replaced by setting a flag for the display data
		if (LOWORD(wParam) == ID_DIRECTION_NORTH) {	//function at a later date
			MessageBoxA(hWnd, "You clicked North", "Direction", MB_OK);
		}
		if (LOWORD(wParam) == ID_DIRECTION_EAST) {
			MessageBoxA(hWnd, "You clicked East", "Direction", MB_OK);
		}
		if (LOWORD(wParam) == ID_DIRECTION_SOUTH) {
			MessageBoxA(hWnd, "You clicked South", "Direction", MB_OK);
		}
		if (LOWORD(wParam) == ID_DIRECTION_WEST) {
			MessageBoxA(hWnd, "You clicked West", "Direction", MB_OK);
   	    }
		// Go is pressed so redraw the window with any new data from list boxes
		if (LOWORD(wParam) == 1) {
			RedrawWindow(hWnd,0,0,RDW_INVALIDATE);	//Send message to redraw screen
		}											//when go is pressed

		// If a list box changes selection get data straight away
		if ( HIWORD (wParam) == LBN_SELCHANGE) {		
			  getuserposition (hWnd, wParam);	// Get current selection from box which
		}										// sent the message
		return 0;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

    case WM_DESTROY:              // Window is being destroyed
		PostQuitMessage(0);
		return 0;

    default:         // Any other message call default message processing
                              
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int paint(HWND hWnd, HDC hDC)
{
	HGDIOBJ hBrush, hOldBrush;  // Brushes
	int desktopwidth=GetSystemMetrics (SM_CXSCREEN); // Get specifc screen data
	int desktopheight=GetSystemMetrics (SM_CYSCREEN);
    RECT arect; 
	char buffer1[BUF_SIZE];

	// Create a brush to use in background fills this one is grey           
	hBrush=CreateSolidBrush(RGB(100,100,100));  // Load this into the windows Device 
												// Context saving the existing old 
	hOldBrush=SelectObject(hDC,hBrush);			// value for restoration later
	
	// A rectangular background fill
	Rectangle(hDC, 0,0,desktopwidth,desktopheight);	// Fill all of client area
	
	hBrush=CreateSolidBrush(RGB(0,0,0));  // Change colour to black for star display
									
	hOldBrush=SelectObject(hDC,hBrush);

	// A rectangular background fill to fill in the srat display area with black
	Rectangle(hDC, 10,10,desktopwidth-10,desktopheight-150);
	
	// Create rectangle for adding text
	arect.bottom=desktopheight-110;
	arect.top=desktopheight-145;
	arect.right=210;
	arect.left=60;
	SetBkMode(hDC, TRANSPARENT);
	
	//Label drop down boxes and add in current viewer settings
	// Create string
	sprintf_s(buffer1,100, "Planet\n%s",observe.planet);
	// Draw text from buffer
	DrawTextExA (hDC,  buffer1 ,-1, &arect	, DT_CENTER|DT_VCENTER,0);

	// Change position for next list box
	arect.right=390;
	arect.left=260;

	// And do as above
	sprintf_s(buffer1,100, "Longitude\n %ddeg%d'%d\"", observe.long_deg,
													observe.long_min, observe.long_sec);
	DrawTextExA (hDC,  buffer1 ,-1, &arect	, DT_CENTER,0);

	arect.right=565;
	arect.left=440;
	sprintf_s(buffer1,100, "Latitude\n %ddeg%d'%d\"", observe.lat_deg,
													observe.lat_min, observe.lat_sec);
	DrawTextExA (hDC,  buffer1 ,-1, &arect	,DT_CENTER,0);

	arect.right=815;
	arect.left=615;
	sprintf_s(buffer1,100, "Date\n %d/%d/%d", observe.day,
													observe.month, observe.year);
	DrawTextExA (hDC,  buffer1 ,-1, &arect	,DT_CENTER,0);

	arect.right=985;
	arect.left=865;
	sprintf_s(buffer1,100, "Time\n %d:%d:%d", observe.hour,
													observe.min, observe.sec);
	DrawTextExA (hDC,  buffer1 ,-1, &arect	,DT_CENTER,0);

	DeleteObject(hBrush);

// Replace default brush back into windows DeviceContext
	SelectObject(hDC,hOldBrush);
  
  return 0;
  }

int initialise_data(HWND hWnd)
{
	int loop=0;
    char buf[BUF_SIZE];
	FILE * ptr;
	errno_t err;
	
	// Open file to read from
	err = fopen_s(&ptr, 
		"D:\\George\\My Documents\\UCE\\Stellar Atlas\\Stellar Atlas\\Star Database.txt", 
				"rt" );
	
	// If the file is missing or will not open error message box
	if(ptr == NULL)
		 MessageBoxA(hWnd, "Star Data not available", "File", MB_OK);
	else		// Read from file
	{
		while(!feof(ptr))	// Read until file ends

		{
             fgets(buf, BUF_SIZE, ptr);	// Read a line

			 // Read from buffer each item and store in arrays of sructures
			 sscanf_s( buf, "%s %f %d %d %d %d %d %d %f %c %d %f", raw_data[loop].name,
				 sizeof(raw_data[loop].name),  
				&raw_data[loop].app_mag, &raw_data[loop].ra_hour,
				&raw_data[loop].ra_min, &raw_data[loop].ra_sec,
			 	&raw_data[loop].dec_deg, &raw_data[loop].dec_min,
				&raw_data[loop].dec_sec, &raw_data[loop].abs_mag,
				&raw_data[loop].spectrum, sizeof(raw_data[loop].spectrum), 
				&raw_data[loop].spectrum_num,
				&raw_data[loop].dist );
				manip_data[loop]=raw_data[loop];

			// Increment loop to ensure each star entry is soted correctly
			 loop++;
		}
	}
	fclose(ptr);		// Close file
	numstars=loop;		// Set number of stars stored

	// Initialise observers viewpoint data
	// Jan 1st 1950 at the North Pole
	observe.long_deg=0; 
	observe.long_min=0; 
	observe.long_sec=0;
	observe.lat_deg=90; 
	observe.lat_min=0; 
	observe.lat_sec=0;
	strcpy_s(observe.planet, 29, "EARTH");
	observe.year=1950; 
	observe.month=1; 
	observe.day=1;
	observe.hour=0; 
	observe.min=0; 
	observe.sec=0;

	return 0;
}
int dropdownlist(HWND hWnd, LPARAM lParam)
{
	int loop;
	HWND start;
	char buffer1[BUF_SIZE];
	int desktopwidth=GetSystemMetrics (SM_CXSCREEN);	// Get current screen data
	int desktopheight=GetSystemMetrics (SM_CYSCREEN);

	// Create all the drop down list boxes for the user poition and fill them

	start = CreateWindowExA (0, "button", "GO!",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			10,(desktopheight-135),50,50,hWnd, (HMENU) 1, ((LPCREATESTRUCT) 
              lParam)->hInstance,NULL);

    planet = CreateWindowExA (0, "combobox", "planet",WS_CHILD | WS_VISIBLE |
    WS_VSCROLL | CBS_DROPDOWNLIST | LBS_NOTIFY |WS_BORDER,
			60,(desktopheight-110),150,250,hWnd, (HMENU) IDC_PLANET, ((LPCREATESTRUCT) 
              lParam)->hInstance,NULL);
	SendMessageA(planet,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("EARTH"));
	
	// Fill with star names from star data structure
		for(loop=0;loop<numstars;loop++)
		{
			sprintf_s( buffer1, BUF_SIZE, "%s ", raw_data[loop].name);
			SendMessageA(planet,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
		}

	// Three windows for the Longitude
	long_deg = CreateWindowExA (0, "combobox","Longitude_degree"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		260,(desktopheight-110),50,150,hWnd,(HMENU)IDC_LONG_DEG, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=-180;loop<181;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(long_deg,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	long_min = CreateWindowExA (0, "combobox","Longitude_minute"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		310,(desktopheight-110),40,150,hWnd,(HMENU)IDC_LONG_MIN, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(long_min,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	long_sec = CreateWindowExA (0, "combobox","Longitude_second"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		350,(desktopheight-110),40,150,hWnd,(HMENU)IDC_LONG_SEC, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(long_sec,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	// Three windows for the Latitude
	lat_deg = CreateWindowExA (0, "combobox","Latitude_degree"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		440,(desktopheight-110),45,150,hWnd,(HMENU)IDC_LAT_DEG, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=-90;loop<91;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(lat_deg,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	lat_min = CreateWindowExA (0, "combobox","Latitude_minute"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		485,(desktopheight-110),40,150,hWnd,(HMENU)IDC_LAT_MIN, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(lat_min,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	lat_sec = CreateWindowExA (0, "combobox","Latitude_second"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		525,(desktopheight-110),40,150,hWnd,(HMENU)IDC_LAT_SEC, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(lat_sec,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}
	
	// Three windows for the Date
	day = CreateWindowExA (0, "combobox","Day"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		615,(desktopheight-110),40,150,hWnd,(HMENU)IDC_DAY, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=1;loop<32;loop++) 
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(day,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

    month = CreateWindowExA (0, "combobox","Month",WS_CHILD | WS_VISIBLE |
    WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		655,(desktopheight-110),100,150,hWnd,(HMENU)IDC_MONTH, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);

	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("January"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("Febuary"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("March"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("April"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("May"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("June"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("July"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("August"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("September"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("October"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("November"));
	SendMessageA(month,CB_ADDSTRING,0,reinterpret_cast<LPARAM>("December"));

	year = CreateWindow (L"combobox",L"Year",WS_CHILD | WS_VISIBLE |
    WS_VSCROLL | CBS_DROPDOWNLIST | LBS_NOTIFY |WS_BORDER,
		755,(desktopheight-110),60,150,hWnd, (HMENU) IDC_YEAR, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<101;loop++)
	{
	sprintf_s( buffer1, BUF_SIZE, "%d", (1950+loop) );
	SendMessageA(year,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}
		
	// Three windows for the Time
	hour = CreateWindowExA (0, "combobox","Hour"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		865,(desktopheight-110),40,150,hWnd,(HMENU)IDC_HOUR, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<24;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(hour,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}
	
	min = CreateWindowExA (0, "combobox","Minute"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		905,(desktopheight-110),40,150,hWnd,(HMENU)IDC_MIN, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(min,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	} 
	
	sec	= CreateWindowExA (0, "combobox","Second"
		,WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_BORDER |LBS_NOTIFY,
		945,(desktopheight-110),40,150,hWnd,(HMENU)IDC_SEC, 
		((LPCREATESTRUCT) lParam)->hInstance,NULL);
	for(loop=0;loop<60;loop++)
	{
		sprintf_s( buffer1, BUF_SIZE, "%d", loop );
		SendMessageA(sec,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(buffer1));
	}

	return 0;
}
int convert_data(void)
{
	int loop;
	float temp_dec, temp_RA, radian_RA, radian_dec, k, temp_long, temp_lat;

	// Convert observer position to radians
	temp_long=static_cast<float>((observe.long_deg) + (observe.long_min * 1/60) + 
			(observe.long_sec * 1/60 * 1/60));
	temp_lat=static_cast<float>((observe.lat_deg) + (observe.lat_min * 1/60) + 
			(observe.lat_sec * 1/60 * 1/60));
	temp_long =static_cast<float>(temp_long*M_PI/180);
	temp_lat = static_cast<float>(temp_lat*M_PI/180);

	for(loop=0;loop<numstars;loop++)
	{
	// Convert ra and dec to radians for the trig functions
		temp_RA=static_cast<float>((manip_data[loop].ra_hour + (manip_data[loop].ra_min * 1/60) + 
				(manip_data[loop].ra_sec * 1/60 * 1/60))*15);
		temp_dec=static_cast<float>(manip_data[loop].dec_deg + (manip_data[loop].dec_min * 1/60) + 
				(manip_data[loop].dec_sec * 1/60 * 1/60));
		radian_RA = static_cast<float>(temp_RA*M_PI/180);
		radian_dec = static_cast<float>(temp_dec*M_PI/180);

	// Put star position through map projection transforms
		k=2/(1+(sinf(temp_lat)*sinf(temp_RA))+(cosf(temp_lat)*cosf(temp_RA)*sinf(radian_RA-temp_long)));

		plot_data[loop].x=k*((cosf(radian_dec))*(sinf(radian_RA-temp_long)));
		plot_data[loop].y=k*((cosf(temp_lat)*sinf(radian_dec))-(sinf(temp_lat)*cosf(radian_dec)*cosf(radian_RA-temp_long)));
	
	}
	return 0;
}

int display_data(HWND hWnd, HDC hDC)
{
	int loop, loop2, red=255, green=255, blue=255;
	float mag_size;
	int x_factor;
	int desktopwidth=GetSystemMetrics (SM_CXSCREEN);	// Get current screen data
	int desktopheight=GetSystemMetrics (SM_CYSCREEN);
	HGDIOBJ hBrush, hOldBrush;  // Brushes 

	x_factor= (desktopwidth-20)/8;	// Adjustment depending on screen size

	for(loop=0;loop<numstars;loop++)
	{
		if (manip_data[loop].app_mag <=6.0) // If star bright enough
		{
			// If star in more accurate centre part of map
			if ((plot_data[loop].x <= 10.0)&& (plot_data[loop].x >= -10.0)
				&& (plot_data[loop].y <= 10.0)&& (plot_data[loop].y >= -10.0))
			{
				// Set size of 'point' of star based on magnitude
				// The brighter the bigger
				if (manip_data[loop].app_mag > 5.0)
					mag_size=static_cast <float> (1);
				
				if ((manip_data[loop].app_mag > 4.0)&&(manip_data[loop].app_mag <= 5.0))
					mag_size=static_cast <float> (2);
				
				if ((manip_data[loop].app_mag > 3.0)&&(manip_data[loop].app_mag <= 4.0))
					mag_size=static_cast <float>(3);

				if ((manip_data[loop].app_mag > 2.0)&&(manip_data[loop].app_mag <= 3.0))
					mag_size=static_cast <float>(4);

				if ((manip_data[loop].app_mag > 1.0)&&(manip_data[loop].app_mag <= 2.0))
					mag_size=static_cast <float>(5);

				if ((manip_data[loop].app_mag > 0.0)&&(manip_data[loop].app_mag <= 1.0))
					mag_size=static_cast <float>(6) ;

				if ((manip_data[loop].app_mag > -1.0)&&(manip_data[loop].app_mag <= 0.0))
					mag_size=static_cast <float>(7);

				// Take x,y coords and convert screen ploted x,y
				plot_data[loop].plot_x = (int)(plot_data[loop].x * x_factor)+ 
									desktopwidth/2; 
				plot_data[loop].plot_y = (int)-(plot_data[loop].y  * x_factor)+
									(desktopheight-160)/2 +10;

				// Set colours based on stars spectrum
				if (manip_data[loop].spectrum =='O')
				{
					red=0; green=25; blue=145;
					for(loop2=0;loop2<10;loop2++)
					{
						green+=5; blue-=3;
					}
				}
				if (manip_data[loop].spectrum =='B')
				{	
					red=0; green=70; blue=130;
					for(loop2=0;loop2<10;loop2++)
					{
						red+=2; green+=3; blue+=1;
					}
				}
				if (manip_data[loop].spectrum =='A')
				{
					red=10; green=100; blue=150;
					for(loop2=0;loop2<10;loop2++)
					{
						red-=1; green+=6; blue-=1;
					}
				}
				if (manip_data[loop].spectrum =='F')
				{
					red=0; green=160; blue=140;

					for(loop2=0;loop2<10;loop2++)
					{
						red+=25; green+=8; blue+=6;
					}
				}
				if (manip_data[loop].spectrum =='G')
				{
					red=235; green=235; blue=200;

					for(loop2=0;loop2<10;loop2++)
					{
						red-=1; green-=14; blue-=19;
					}
				}
				if (manip_data[loop].spectrum =='K')
				{
					red=230; green=110; blue=25;

					for(loop2=0;loop2<10;loop2++)
					{
						green-=6; blue-=1;
					}
				}
				if (manip_data[loop].spectrum =='M')
				{
					red=230; green=50; blue=20;
					for(loop2=0;loop2<10;loop2++)
					{
						red+=2; green-=5; blue-=2;
					}
				}
				// Create brush for star based on its colour
				hBrush=CreateSolidBrush(RGB(red,green,blue)); 

			hOldBrush=SelectObject(hDC,hBrush);
			// Check star will print in black client area
			if(((plot_data[loop].plot_x-mag_size)>9)&&
				((plot_data[loop].plot_x + mag_size) < (desktopwidth-9))
				&&((plot_data[loop].plot_y-mag_size) >9)&&
				((plot_data[loop].plot_y+mag_size )<(desktopheight-149) ))

				// Print star
				Ellipse(hDC, (int)(plot_data[loop].plot_x-mag_size),
							(int)(plot_data[loop].plot_y-mag_size),
							(int)(plot_data[loop].plot_x+ mag_size),
							(int)(plot_data[loop].plot_y+mag_size));	

			}
		}
	}
	
	DeleteObject(hBrush);	// Replace default brush back into windows DeviceContext
	SelectObject(hDC,hOldBrush);

	return 0;
}


int getuserposition (HWND hWnd, WPARAM wParam)
{	
	char buffer1[BUF_SIZE];
 
	switch (LOWORD(wParam)) // Get new listbox selection only 
	{						
	case IDC_PLANET:		// Get window text and copy it into relevant structure member
			GetWindowTextA( planet, buffer1, 99 );
			strcpy_s(observe.planet,100, buffer1);
  			
			return 0;

	case IDC_LONG_DEG:
			GetWindowTextA( long_deg, buffer1, 99 );
			observe.long_deg=atoi(buffer1);
			return 0;

	case IDC_LONG_MIN:
			GetWindowTextA( long_min, buffer1, 99 );
			observe.long_min=atoi(buffer1);
			return 0;

	case IDC_LONG_SEC:
			GetWindowTextA( long_sec, buffer1, 99 );
			observe.long_sec=atoi(buffer1);
			return 0;

	case IDC_LAT_DEG:
			GetWindowTextA( lat_deg, buffer1, 99 );
			observe.lat_deg=atoi(buffer1);
			return 0;

	case IDC_LAT_MIN:
			GetWindowTextA( lat_min, buffer1, 99 );
			observe.lat_sec=atoi(buffer1);
			return 0;

	case IDC_LAT_SEC:
			GetWindowTextA( lat_sec, buffer1, 99 );
			observe.lat_sec=atoi(buffer1);
			return 0;

	case IDC_YEAR:
			GetWindowTextA( year, buffer1, 99 );
			observe.year=atoi(buffer1);
			return 0;

	case IDC_MONTH:
			GetWindowTextA( month, buffer1, 99 );
			strcpy_s(observe.month_name,29, buffer1);
			convert_month_to_num ();
			return 0;

	case IDC_DAY:
			GetWindowTextA( day, buffer1, 99 );
			observe.day=atoi(buffer1);
			return 0;

	case IDC_HOUR:
			GetWindowTextA( hour, buffer1, 99 );
			observe.hour=atoi(buffer1);
			return 0;

	case IDC_MIN:
			GetWindowTextA( min, buffer1, 99 );
			observe.min=atoi(buffer1);
			return 0;

	case IDC_SEC:
			GetWindowTextA( sec, buffer1, 99 );
			observe.sec=atoi(buffer1);
			return 0;

	default:
		return 0;
	}
	return 0;
}

int	move_to_star(HWND hWnd)		// To be completed
{
	return 0;
}
int convert_month_to_num (void)	// Not operating correctly due to string issue
{								// mentioned in report
 if (observe.month_name=="January")
	 observe.month=1;
 if (observe.month_name=="Febuary")
	 observe.month=2;
 if (observe.month_name=="March")
	 observe.month=3;
 if (observe.month_name=="April")
	 observe.month=4;
 if (observe.month_name=="May")
	 observe.month=5;
 if (observe.month_name=="June")
	 observe.month=6;
 if (observe.month_name=="July")
	 observe.month=7;
 if (observe.month_name=="August")
	 observe.month=8;
 if (observe.month_name=="September")
	 observe.month=9;
 if (observe.month_name=="October")
	 observe.month=10;
 if (observe.month_name=="November")
	 observe.month=11;
 if (observe.month_name=="December")
	 observe.month=12;
 
	return 0;
}
