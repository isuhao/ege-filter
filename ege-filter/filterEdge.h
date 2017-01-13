/*
* filterEdge.h
*
*  Created on: 2017-1-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_EDGE_H_
#define _FILTER_EDGE_H_

#include "filterCore.h"

template<int ChannelType>
class FilterEdge : public FilterCore<ChannelType>
{
public:

	static inline void run(void* dataOut, const void* dataIn, int width, int height, int stride, float intensity)
	{
		const unsigned char* dtIn = (const unsigned char*)dataIn;
		unsigned char* dtOut = (unsigned char*)dataOut;
		const int w = width * Channels, h = height;
		const float revIntensity = 1.0f - intensity;
		for (int i = 2; i < h; ++i)
		{
			const int indexH = stride * i;
			const unsigned char* lineAddrIn = dtIn + indexH;
			unsigned char* lineAddrOut = dtOut + indexH;
			for (int j = Channels * 2; j < stride; j += Channels)
			{
				const unsigned char* in1 = lineAddrIn + j;
				const unsigned char* in2 = lineAddrIn + j - Channels * 2 - stride * 2;
				unsigned char* out = lineAddrOut + j;
				out[Channel_R] = abs(in1[Channel_R] - in2[Channel_R]) * intensity + in1[Channel_R] * revIntensity;
				out[Channel_G] = abs(in1[Channel_G] - in2[Channel_G]) * intensity + in1[Channel_G] * revIntensity;
				out[Channel_B] = abs(in1[Channel_B] - in2[Channel_B]) * intensity + in1[Channel_B] * revIntensity;
			}
		}
	}
};


#endif // !_FILTER_EDGE_H_