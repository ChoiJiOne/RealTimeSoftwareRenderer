/******************************************************
	g-Matrix3D Engine
	Copyright (c)2000 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.hitel.net/~kaswan

*******************************************************/
#include "stdafx.h"

BOOL ASEFileOpenDlg(HWND hwnd, char * fname);

BOOL ASEFileOpenDlg(HWND hwnd, char * fname)
{ 
	OPENFILENAME ofn;
	char str[] = "3DS MAX ASE Files (*.ASE)\0*.ase\0";
	char lpDir[1024] = ".\\data";
	char lpFile[1024] = "*.ase";
	char lpTitle[1024] = "";

	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= hwnd;
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= str;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 40;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= lpFile;//"*.ase";
	ofn.nMaxFile			= 256;
	ofn.lpstrFileTitle		= lpTitle;
	ofn.nMaxFileTitle		= 1024;
	ofn.lpstrInitialDir		= lpDir;//NULL;
	ofn.lpstrTitle			= "Open ASE";
	ofn.Flags				= 0;
	ofn.nFileOffset			= 0;
	ofn.nFileExtension		= 0;
	ofn.lpstrDefExt			= "ase";
	ofn.lCustData			= 0;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;

	if( !GetOpenFileName(&ofn) )
	{
		fname = NULL;
		return FALSE;
	}
	
	strcpy(fname, ofn.lpstrFileTitle);
	
	return TRUE;
}
