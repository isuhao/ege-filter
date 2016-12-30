/*
* filterContrast.h
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_CONTRAST_H_

#include "filterCore.h"

template<int ChannelType>
class FilterContrast : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			out[Channel_R] = CLAMP((in[Channel_R] - 128.0f) * intensity + 128.0f, 0.0f, 255.0f);
			out[Channel_G] = CLAMP((in[Channel_G] - 128.0f) * intensity + 128.0f, 0.0f, 255.0f);
			out[Channel_B] = CLAMP((in[Channel_B] - 128.0f) * intensity + 128.0f, 0.0f, 255.0f);
//			out[Channel_A] = in[Channel_A];
		}));
	}
};


#endif // !_FILTER_BRIGHTNESS_H_