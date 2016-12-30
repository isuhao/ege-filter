/*
* filterSaturation.h
*
*  Created on: 2016-12-30
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_CONTRAST_H_

#include "filterCore.h"

template<int ChannelType>
class FilterSaturation : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		float revIntensity = 1.0f - intensity;
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			float central = (MAX(MAX(out[Channel_R], out[Channel_G]), out[Channel_B]) + MIN(MIN(out[Channel_R], out[Channel_G]), out[Channel_B])) / 2.0f;
			out[Channel_R] = CLAMP(in[Channel_R] * intensity + revIntensity * central, 0.0f, 255.0f);
			out[Channel_G] = CLAMP(in[Channel_G] * intensity + revIntensity * central, 0.0f, 255.0f);
			out[Channel_B] = CLAMP(in[Channel_B] * intensity + revIntensity * central, 0.0f, 255.0f);
//			out[Channel_A] = in[Channel_A];
		}));
	}
};


#endif // !_FILTER_BRIGHTNESS_H_