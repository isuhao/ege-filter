/*
* filterSaturation.h
*
*  Created on: 2016-12-30
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_SATURATION_H_
#define _FILTER_SATURATION_H_

#include "filterCore.h"

template<int ChannelType>
class FilterSaturation : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		const float revIntensity = 1.0f - intensity;
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			const float central = (in[Channel_R] + in[Channel_G] + in[Channel_B]) / 3.0f;
			out[Channel_R] = CLAMP(in[Channel_R] * intensity + revIntensity * central, 0.0f, 255.0f);
			out[Channel_G] = CLAMP(in[Channel_G] * intensity + revIntensity * central, 0.0f, 255.0f);
			out[Channel_B] = CLAMP(in[Channel_B] * intensity + revIntensity * central, 0.0f, 255.0f);
//			out[Channel_A] = in[Channel_A];
		}));
	}
};


#endif // !_FILTER_SATURATION_H_