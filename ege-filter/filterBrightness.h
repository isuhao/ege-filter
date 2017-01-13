/*
* filterBrightness.h
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_BRIGHTNESS_H_
#define _FILTER_BRIGHTNESS_H_

#include "filterCore.h"

//使用最简单的加法.
template<int ChannelType>
class FilterBrightness : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		int plusValue = intensity * 128;
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			out[Channel_R] = CLAMP(in[Channel_R] + plusValue, 0, 255);
			out[Channel_G] = CLAMP(in[Channel_G] + plusValue, 0, 255);
			out[Channel_B] = CLAMP(in[Channel_B] + plusValue, 0, 255);
//			out[Channel_A] = in[Channel_A];
		}));
	}
};


#endif // !_FILTER_BRIGHTNESS_H_