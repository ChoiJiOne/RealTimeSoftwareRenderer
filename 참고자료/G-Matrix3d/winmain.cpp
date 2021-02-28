/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

	Using OpenGL 1.1
	Using D3DIM 7.0, Direct3DX Utility Library
	
	2000. 1. 24
	
	2000. 1. 20
		add 3D H/W support through Direct3DX 
	2000. 1. 10
		add 3D H/W support through OpenGL
	1999. 12. 29
		Fog Effect
	1999. 11. 29
		16bit color S/W Rasterizer
	1999. 11. 25
		Texture mapping
	1999. 11. 19
		add 3DS MAX ASE file support
	1999. 11. 3
		Simple Software Rasterizer (24bit color)
			Flat shading
			Gouraud shading
			Automatic creation of smooth vertex normal
*******************************************************/

#include "stdafx.h"
#include "resource.h"

#include "software.h"
#include "directdraw.h"
#include "opengl.h"
#include "direct3d.h"

#include "bmp.h"
#include "readase.h"
#include "fog.h"

#define MODE_DIB	0
#define MODE_DDRAW	1
#define MODE_OPENGL	2
#define MODE_D3D	3

// Windows Globals
HWND hWnd;
HINSTANCE hThisInst;
int ModeSelect = MODE_DIB;

// Function Prototypes
BOOL RegisterWindowClass(HINSTANCE hInstance);
BOOL MakeWindow(char *title, HINSTANCE hInstance, int nCmdShow);
BOOL CreateDIBWindow(HINSTANCE hInstance, int nCmdShow);
BOOL CloseDIBWindow(HWND hWnd);
BOOL CreateDDRAWWindow(HINSTANCE hInstance, int nCmdShow);
BOOL CloseDDRAWWindow(HWND hWnd);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK SelectionDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, // handle to current instance 
					HINSTANCE hPrevInstance, // handle to previous instance 
					LPSTR lpCmdLine, // pointer to command line 
					int nCmdShow) // show state of window 
{
    MSG msg;
	
	int rt = DialogBox(hInstance, "DIALOG_SELECTION1", NULL, SelectionDlgProc);
	if (rt == IDCANCEL)
		return FALSE;

	/////////////////////////////////////////////////
	///
	switch (ModeSelect)
	{
		case MODE_DIB:
			CreateDIBWindow(hInstance, nCmdShow);
			break;
		
		case MODE_DDRAW:
			CreateDDRAWWindow(hInstance, nCmdShow);
			break;

		case MODE_OPENGL:
			hWnd = CreateOPENGLWindow(hInstance, nCmdShow);
			if (hWnd == NULL)
			{
				MessageBox(hWnd, "OpenGL", "Error", MB_OK);
				return FALSE;
			}
			break;
		
		case MODE_D3D:
			RegisterWindowClass(hInstance);
			MakeWindow("Direct3d", hInstance, nCmdShow);
			
			if (SetupDirect3D(hWnd) != S_OK)
			{
				MessageBox(hWnd, "Direct3D", "Error", MB_OK);
				return FALSE;
			}
			
			bIsActive = TRUE;
			break;
		
		default:
			return FALSE;
	}
	
	hThisInst = hInstance;

	// message loop for game application
    while( 1 )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if( !GetMessage( &msg, NULL, 0, 0 ) )
            {
				break;
            }
            else
			{
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else if ( bIsActive )
        {
			switch (ModeSelect)
			{
				case MODE_DIB:
				case MODE_DDRAW:
					UpdateFrame();
					break;

				case MODE_OPENGL:
					UpdateFrameOPENGL();
					break;
		
				case MODE_D3D:
					UpdateFrameD3D();
					break;
			}
        }
        else
        {
            WaitMessage();
        }
    }

	///////////////////////////////////////////////
	///
	switch (ModeSelect)
	{
		case MODE_DIB:
			CloseDIBWindow(hWnd);
			break;
		
		case MODE_DDRAW:
			CloseDDRAWWindow(hWnd);
			break;

		case MODE_OPENGL:
			CloseOPENGLWindow();
			break;
		
		case MODE_D3D:
			CloseDirect3D();
			//ReleaseD3DX();
			break;
		
		default:
			return FALSE;
	}
	

    DestroyWindow(hWnd);

	return msg.wParam;

}

BOOL RegisterWindowClass(HINSTANCE hInstance)
{
    WNDCLASS wc;

    wc.style            = CS_HREDRAW|CS_VREDRAW;
    wc.lpfnWndProc      = WindowProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(hInstance, (const char *)IDI_ICON1);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName     = (char *)IDR_MENU1;
    wc.lpszClassName    = "GM3D Class";

    if (!RegisterClass(&wc)) return FALSE;

	return TRUE;
}

BOOL MakeWindow(char *title, HINSTANCE hInstance, int nCmdShow)
{
    RECT rect = {0, 0, 639, 479};

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	int width = rect.right - rect.left + 1;
	int height = rect.bottom - rect.top + 1;
	
	hWnd = CreateWindowEx(NULL,
                          "GM3D Class",
                          title,//"g-Matrix3D",
                          WS_OVERLAPPEDWINDOW,
                          (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
                          (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
                          width,
                          height,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

	return TRUE;
}

BOOL CreateDIBWindow(HINSTANCE hInstance, int nCmdShow)
{
	if (!RegisterWindowClass(hInstance))
		return FALSE;

    if (!MakeWindow("g-Matrix3D(Software)", hInstance, nCmdShow))
        return FALSE;
	
	SetupSoftware(hWnd);
	
	// load texture file
	pTexture = LoadBMP24("check.bmp", 16);
	if (pTexture == NULL) 
	{
		MessageBox(hWnd, "Cannot find Texture check.bmp", "Error", MB_OK);
		CloseSoftware(hWnd);
		return 0;
	}
	
	SetupWorld();

	return TRUE;
}

BOOL CloseDIBWindow(HWND hWnd)
{
	CloseSoftware(hWnd);
	return TRUE;
}

BOOL CreateDDRAWWindow(HINSTANCE hInstance, int nCmdShow)
{
	if (!RegisterWindowClass(hInstance))
		return FALSE;

    if (!MakeWindow("g-Matrix3D(DirectDraw)", hInstance, nCmdShow))
        return FALSE;

	if ( SetupDirectDraw(hWnd) != DD_OK ) 
	{
		ReleaseDDObjects();
		return FALSE;
	}

	SetupSoftware(hWnd);
	SetupWorld();

	
	return TRUE;
}

BOOL CloseDDRAWWindow(HWND hWnd)
{
	CloseSoftware(hWnd);
	return TRUE;
}

void OnAbout(HWND hWnd)
{
	char string[1024];
	char copyright[] = "g-Matrix 3D\n\nCopyright (c)2000 Kim Seong Wan\n\n";

	strcpy(string, copyright);

	switch (ModeSelect)
	{
		case MODE_DIB:
			strcat(string, "Mode: Software\n\n");
			break;
		case MODE_DDRAW:
			strcat(string, "Mode: DirectDraw\n\n");
			break;
		case MODE_OPENGL:
			strcat(string, "Mode: OpenGL\n\n");
			wsprintf(&string[strlen(string)], "Vendor: %s\nRenderer: %s\nVersion: %s\nExtensions: %s\n",
					glGetString(GL_VENDOR), glGetString(GL_RENDERER),
					glGetString(GL_VERSION), glGetString(GL_EXTENSIONS));
			MessageBox(hWnd, string, "About", MB_OK | MB_ICONINFORMATION);
			return;
			break;
		case MODE_D3D:
			strcat(string, "Mode: Direct3D\n\n");
			break;
	}
	MessageBox(hWnd, string, "About", MB_OK | MB_ICONINFORMATION);
}

BOOL CALLBACK SelectionDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			MoveWindow(hDlg,
						GetSystemMetrics(SM_CXSCREEN) / 2 - 200,
						GetSystemMetrics(SM_CYSCREEN) / 2 - 100,
						400,
						200,
						FALSE);
			CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
			break;
		case WM_COMMAND:
			switch (wParam)
			{
				case IDC_RADIO1:
					ModeSelect = 0;
					CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO1);
					break;
				
				case IDC_RADIO2:
					ModeSelect = 1;
					CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO2);
					break;
				
				case IDC_RADIO3:
					ModeSelect = 2;
					CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO3);
					break;

				case IDC_RADIO4:
					ModeSelect = 3;
					CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO4, IDC_RADIO4);
					break;

				case IDCANCEL:
					EndDialog(hDlg, IDCANCEL);
					break;

				case IDOK:
					EndDialog(hDlg, IDOK);
					break;
			}
			break;
		
		default:
			return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
	RECT rect;
	char string[256];
	char stringFPS[20];
	int mouse_x, mouse_y;
	static int mouse_sx, mouse_sy;
	static BOOL LButton = FALSE;
	static BOOL RButton = FALSE;
	static int CoordZ;
	static float AngleX, AngleY;
    
	switch(message)
    {
		case WM_CREATE:
			SetTimer(hWnd, 1, 1000, NULL);
			return 0;
	
		case WM_DESTROY:
			KillTimer(hWnd, 1);
			PostQuitMessage(0);
			return 0;

	    case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		
		case WM_PAINT:
			hDC = BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
			return 0;

		case WM_SIZE:
			if(ModeSelect == MODE_D3D
				&& bD3DXReady 
				&& LOWORD(lParam)>0 
				&& HIWORD(lParam)>0)
			{
				HRESULT hr;
            
				if( FAILED(hr = pD3DX->Resize(LOWORD(lParam),HIWORD(lParam))))
				{
					InterpretD3DError(hr);
					bD3DXReady = FALSE;
					PostQuitMessage(0);
				}
            
			}
			if (ModeSelect == MODE_OPENGL)
			{
				GetClientRect(hWnd, &rect);
				glViewport(0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
			}
			break;
		
		case WM_TIMER:
			sprintf(stringFPS, "  FPS: %2d  ", GetFrames());
			LoadString(hThisInst, IDS_TITLE, string, 256);
			strcat(string, stringFPS);
			SetWindowText(hWnd, string);
			SetFrames(0);
			return 0;
		
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_OPEN:
					if ( LoadASE(hWnd) ) bIsASE = TRUE;
					return 0;

				case ID_EXIT:
					PostQuitMessage(0);
					return 0;

				case ID_FOG:
					if (FOG_ENABLE == TRUE)
					{
						FOG_ENABLE = FALSE;
						CheckMenuItem(GetMenu(hWnd), ID_FOG, MF_UNCHECKED);
					}
					else 
					{
						FOG_ENABLE = TRUE;
						CheckMenuItem(GetMenu(hWnd), ID_FOG, MF_CHECKED);
					}
					return 0;

				case ID_ABOUT:
					OnAbout(hWnd);
					return 0;
			}
			break;
		case WM_KEYDOWN:
			{
				switch(wParam)
				{
					case VK_UP:
						ViewCoord.y += 5;
						return 0;

					case VK_DOWN:
						ViewCoord.y -= 5;
						return 0;

					case VK_RIGHT:
						ViewCoord.x += 5;
						return 0;

					case VK_LEFT:
						ViewCoord.x -= 5;
						return 0;

					case VK_SPACE:
						RENDERSTATE++;
						if (RENDERSTATE > 6) RENDERSTATE = 0;
						return 0;
					
					case 0x30:
						RENDERSTATE = 0;
						return 0;
					case 0x31:
						RENDERSTATE = 1;
						return 0;
					case 0x32:
						RENDERSTATE = 2;
						return 0;
					case 0x33:
						RENDERSTATE = 3;
						return 0;
					case 0x34:
						RENDERSTATE = 4;
						return 0;
					case 0x35:
						RENDERSTATE = 5;
						return 0;
					case 0x36:
						RENDERSTATE = 6;
						return 0;
					case VK_ESCAPE:
						PostMessage(hWnd, WM_CLOSE, 0, 0);
						return 0;
				}
			}
			break;

		case WM_LBUTTONUP:
			LButton = FALSE;
			ReleaseCapture();
			return 0;
		
		case WM_RBUTTONUP:
			RButton = FALSE;
			ReleaseCapture();
			return 0;
		
		case WM_LBUTTONDOWN:
			LButton = TRUE;
			SetCapture(hWnd);
			
			mouse_sx = (short)LOWORD(lParam);
			mouse_sy = (short)HIWORD(lParam);
			
			AngleX = ModelAngle.x;
			AngleY = ModelAngle.y;
			
			return 0;
		
		case WM_RBUTTONDOWN:
			RButton = TRUE;
			SetCapture(hWnd);
			
			mouse_sx = (short)LOWORD(lParam);
			mouse_sy = (short)HIWORD(lParam);
			CoordZ = (int)ViewCoord.z;

			return 0;
		
		case WM_MOUSEMOVE:
			if ( LButton == TRUE )
			{
				mouse_x = (short)LOWORD(lParam);
				mouse_y = (short)HIWORD(lParam);
				
				ModelAngle.y = AngleY + (float)(mouse_x - mouse_sx);
				ModelAngle.x = AngleX + (float)(mouse_y - mouse_sy);
			}
			if ( RButton == TRUE )
			{
				mouse_x = (short)LOWORD(lParam);
				mouse_y = (short)HIWORD(lParam);
				
				ViewCoord.z = CoordZ - (float)(mouse_y - mouse_sy) / 3;
			}
			return 0;
    }

	return DefWindowProc(hWnd, message, wParam, lParam);
}



