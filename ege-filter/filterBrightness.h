/*
* filterBrightness.h
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_BRIGHTNESS_H_

#include "filterCore.h"

//使用最简单的加法.
template<int ChannelType>
class FilterBrightness : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		int plusValue = intensity * 255;
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			out[Channel_R] = MIN(in[Channel_R] + plusValue, 255);
			out[Channel_G] = MIN(in[Channel_G] + plusValue, 255);
			out[Channel_B] = MIN(in[Channel_B] + plusValue, 255);
//			out[Channel_A] = MIN(in[Channel_A] + plusValue, 255);
		}));
	}
};


#endif // !_FILTER_BRIGHTNESS_H_