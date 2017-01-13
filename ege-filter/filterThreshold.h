/*
* filterThreshold.h
*
*  Created on: 2017-1-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_THRESHOLD_H_
#define _FILTER_THRESHOLD_H_

#include "filterCore.h"

template<int ChannelType>
class FilterThreshold : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		unsigned char v = intensity * 255;
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			unsigned char lum = ((int)in[Channel_R] + in[Channel_G] + in[Channel_B]) / 3 < v ? 0 : 255;
			out[Channel_R] = lum;
			out[Channel_G] = lum;
			out[Channel_B] = lum;
		}));
	}
};


#endif // !_FILTER_THRESHOLD_H_