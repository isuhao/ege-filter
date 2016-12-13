/*
* main.cpp
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#define SHOW_CONSOLE

#include "graphics.h"
#include "filterBrightness.h"
#include "filterContrast.h"

int main()
{
	initgraph(640, 480, INIT_RENDERMANUAL);
	PIMAGE img = newimage();
	getimage(img, "Z:/download/5.png");
	color_t* buffer = getbuffer(img);
	int w = getwidth(img);
	int h = getheight(img);
	int stride = w * 4;
	PIMAGE img2 = newimage(w, h);
	color_t* buffer2 = getbuffer(img2);
//	FilterBrightness<FilterCoreChannelType_BGRA>::run(buffer2, buffer, w, h, stride, 0.5f);
	FilterContrast<FilterCoreChannelType_BGRA>::run(buffer2, buffer, w, h, stride, 2.5f);
	putimage(0, 0, img2);

	getch();
	closegraph();
	return 0;
}