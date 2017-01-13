/*
 * utilityWin32.cpp
 *
 *  Created on: 2013-12-11
 *      Author: Wang Yang
 */

#include "utilityWin32.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#include <windows.h>
#include <Commdlg.h>
#include <ShlObj.h>

#ifndef BIF_USENEWUI
#define BIF_USENEWUI 0x0050
#endif
#ifndef BIF_UAHINT
#define BIF_UAHINT 0x0100
#endif



const static char *s_defaultFilters[] = 
{
	"All Files(*.*)\0*.*\0\0",
	"Image Files(*.jpg;*.png;*.bmp;*.gif)\0*.jpg;*.jpeg;*.png;*.bmp;*.gif\0All Files(*.*)\0*.*\0\0",
	"Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0",
	"Audio Files(*.mp3;*.wav;*.wma;*.3ga)\0*.mp3;*.wav;*.wma;*.3ga\0All Files(*.*)\0*.*\0\0",
	"Video Files(*.mp4;*.avi;*.flv;*.rmvb)\0*.mp4;*.avi;*.flv;*.rmvb\0All Files(*.*)\0*.*\0\0"
};

// bool getFolderPathDlg(HWND hwnd, LPSTR folderPath, LPCSTR title)
// {
// 	BROWSEINFOA bfo;
// 	memset(&bfo, 0, sizeof(bfo));
// 	bfo.hwndOwner = hwnd;
// 	bfo.lpszTitle = title;
// 	bfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_UAHINT;
// 
// 	LPITEMIDLIST lpDlist = SHBrowseForFolderA(&bfo);
// 	if(lpDlist == NULL)
// 		return false;
// 	SHGetPathFromIDListA(lpDlist, folderPath);
// 	return true;
// }
// 
// bool saveFolderPathDlg(HWND hwnd, LPSTR folderPath, LPCSTR title)
// {
// 	BROWSEINFOA bfo;
// 	memset(&bfo, 0, sizeof(bfo));
// 	bfo.hwndOwner = hwnd;
// 	bfo.lpszTitle = title;
// 	bfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_UAHINT;
// 
// 	LPITEMIDLIST lpDlist = SHBrowseForFolderA(&bfo);
// 	if(lpDlist == NULL)
// 		return false;
// 	SHGetPathFromIDListA(lpDlist, folderPath);
// 	return true;
// }
// int readMultiFileNameDlg(HWND hwnd, char* filenames, const char* title, UTL_FileFilter filter)
// {
// 	if(filter >= utlFilter_ERROR || filter < utlFILTER_ALL) return filter = utlFILTER_ALL;
// 	return readMultiFileNameDlg(hwnd, filenames, title, s_defaultFilters[filter]);
// }
// 
// int readMultiFileNameDlg(HWND hwnd, char* filenames, const char* title, const char* filter)
// {
// 	*filenames = 0;
// 	OPENFILENAMEA ofn;
// 	memset(&ofn, 0, sizeof(ofn));
// 	ofn.hwndOwner = hwnd;
// 	ofn.lStructSize = sizeof(ofn);
// 	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
// 	ofn.lpstrFile = filenames;
// 	ofn.nMaxFile = BUFFER_SIZE;
// 	if(filter == NULL)
// 		ofn.lpstrFilter = s_defaultFilters[utlFILTER_ALL];
// 	else ofn.lpstrFilter = filter;
// 	ofn.lpstrTitle = title;
// 	if(!GetOpenFileNameA(&ofn)) return -1;
// 	return ofn.nFileOffset;
// }

bool readFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, UTL_FileFilter filter)
{
	if(filter >= utlFilter_ERROR || filter < utlFILTER_ALL) filter = utlFILTER_ALL;
	return readFileNameDlg(hwnd, filename, title, s_defaultFilters[filter]);
}

bool readFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, LPCSTR filter)
{
	OPENFILENAMEA ofna;
	*filename = 0;
	memset(&ofna, 0, sizeof(OPENFILENAMEA));
	ofna.lStructSize = sizeof(OPENFILENAMEA);
	ofna.hwndOwner = hwnd;
	ofna.hInstance = NULL;
	if(filter == NULL)
		ofna.lpstrFilter = s_defaultFilters[utlFILTER_ALL];
	else ofna.lpstrFilter = filter;
	ofna.nMaxFile = MAX_PATH;
	ofna.lpstrDefExt = "txt";
	ofna.lpstrFile = filename;
	ofna.lpstrTitle = title;
	ofna.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	return !!GetOpenFileNameA(&ofna);
}
// 
// bool saveFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, UTL_FileFilter filter)
// {
// 	if(filter >= utlFilter_ERROR || filter < utlFILTER_ALL) filter = utlFILTER_ALL;
// 	return saveFileNameDlg(hwnd, filename, title, s_defaultFilters[filter]);
// }
// 
// bool saveFileNameDlg(HWND hwnd, LPSTR filename, LPCSTR title, LPCSTR filter)
// {
// 	OPENFILENAMEA ofna;
// 	*filename = 0;
// 	memset(&ofna, 0, sizeof(OPENFILENAMEA));
// 	ofna.lStructSize = sizeof(OPENFILENAMEA);
// 	ofna.hwndOwner = hwnd;
// 	ofna.hInstance = NULL;
// 	if(filter == NULL)
// 		ofna.lpstrFilter = s_defaultFilters[utlFILTER_ALL];
// 	else ofna.lpstrFilter = filter;
// 	ofna.nMaxFile = MAX_PATH;
// 	ofna.lpstrFile = filename;
// 	ofna.lpstrTitle = title;
// 	ofna.Flags = OFN_OVERWRITEPROMPT;
// 	return !!GetSaveFileNameA(&ofna);
// }
// 
// bool extractResource(LPCSTR strDstFile, LPCSTR strResType, LPCSTR strResName)
// {
// 	HRSRC hRes = FindResourceA(NULL, strResName, strResType);
// 	if (hRes == NULL) return false;
// 
// 	HGLOBAL hMem = LoadResource(NULL, hRes);
// 	DWORD dwSize = SizeofResource(NULL, hRes);
// 	DWORD dwWrite = 0;
// 
// 	HANDLE hFile = CreateFileA(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
// 	if (hFile == INVALID_HANDLE_VALUE)
// 		return false;
// 	
// 	WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
// 	CloseHandle(hFile);
// 	return true;
// }

// CImage* genCImageFromResource(UINT nResID, LPCTSTR lpTyp)
// {
// 	// 查找资源
// 	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(nResID), lpTyp);
// 	if (hRsrc == NULL) return false;
// 
// 	// 加载资源
// 	HGLOBAL hImgData = LoadResource(NULL, hRsrc);
// 	if (hImgData == NULL)
// 	{
// 		FreeResource(hImgData);
// 		return NULL;
// 	}
// 
// 	// 锁定内存中的指定资源
// 	LPVOID lpVoid = LockResource(hImgData);
// 
// 	LPSTREAM pStream = NULL;
// 	DWORD dwSize = SizeofResource(NULL, hRsrc);
// 	HGLOBAL hNew = GlobalAlloc(GHND, dwSize);
// 	LPBYTE lpByte = (LPBYTE)GlobalLock(hNew);
// 	memcpy(lpByte, lpVoid, dwSize);
// 
// 	// 解除内存中的指定资源
// 	GlobalUnlock(hNew);
// 
// 	CImage* pImage = new CImage;
// 
// 	// 从指定内存创建流对象
// 	HRESULT ht = CreateStreamOnHGlobal(hNew, TRUE, &pStream);
// 	if ( ht != S_OK )
// 	{
// 		GlobalFree(hNew);
// 		return NULL;
// 	}
// 	else
// 	{
// 		// 加载图片
// 		pImage->Load(pStream);
// 		GlobalFree(hNew);
// 	}
// 
// 	// 释放资源
// 	FreeResource(hImgData);
// 	return pImage;
// }