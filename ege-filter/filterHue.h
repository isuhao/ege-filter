/*
* filterHue.h
*
*  Created on: 2011-1-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_HUE_H_
#define _FILTER_HUE_H_

#include "filterCore.h"

template<int ChannelType>
class FilterHue : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		_FILTER_RUN(dataOut, dataIn, w, h, stride, _FILTER_CODE(
		{
			const float r = in[Channel_R] / 255.0f, g = in[Channel_G] / 255.0f, b = in[Channel_B] / 255.0f;
			
			const float y = r * 0.299f + g * 0.587f + b * 0.114f;
			const float i = r * 0.595716f - g * 0.274453f - b * 0.321263f;
			const float q = r * 0.211456f - g * 0.522591f + b * 0.31135f;

			const float hue = atan2(q, i) - intensity;
			const float chroma = sqrtf(q*q + i*i);
			const float ii = cosf(hue) * chroma;
			const float qq = sinf(hue) * chroma;

			out[Channel_R] = CLAMP(y + 0.9563f * ii + 0.621f * qq, 0.0f, 1.0f) * 255.0f;
			out[Channel_G] = CLAMP(y - 0.2721f * ii - 0.6474f * qq, 0.0f, 1.0f) * 255.0f;
			out[Channel_B] = CLAMP(y - 1.107f * ii - 1.7046f * qq, 0.0f, 1.0f) * 255.0f;
		}));
	}
};


#endif // !_FILTER_HUE_H_