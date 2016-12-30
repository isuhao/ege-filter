/*
 * utilityWin32.h
 *
 *  Created on: 2013-12-10
 *      Author: Wang Yang
 */

#ifndef _UTILITYWIN32_H_
#define _UTILITYWIN32_H_

#include <windows.h>
#include <atlimage.h>

enum UTL_FileFilter{
	utlFILTER_ALL = 0,
	utlFILTER_IMAGE,
	utlFILTER_TEXT, 
	utlFILTER_AUDIO, 
	utlFILTER_VIDEO,
	utlFilter_ERROR
};

bool getFolderPathDlg(HWND hwnd, LPSTR folderPath, LPCSTR title);
bool saveFolderPathDlg(HWND hwnd, LPSTR folderPath, LPCSTR title);
int readMultiFileNameDlg(HWND hwnd, char* filenames, const char* title, UTL_FileFilter filter);
int readMultiFileNameDlg(HWND hwnd, char* filenames, const char* title, const char* filter = NULL);
bool readFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, UTL_FileFilter filter);
bool readFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, LPCSTR filter = NULL);
bool saveFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, UTL_FileFilter filter);
bool saveFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, LPCSTR filter = NULL);
bool extractResource(LPCSTR strDstFile, LPCSTR strResType, LPCSTR strResName);
CImage* genCImageFromResource(UINT nResID, LPCTSTR lpTyp);

#endif