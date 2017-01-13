/*
* filterCore.h
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#ifndef _FILTER_CORE_H_
#define _FILTER_CORE_H_

#include <cmath>

enum FilterCoreChannelType
{
	FilterCoreChannelType_Luminance = 0, //暂不支持
	FilterCoreChannelType_LuminanceAlpha = 1, //暂不支持
	FilterCoreChannelType_RGB = 120, //暂不支持
	FilterCoreChannelType_RGBA = 123, //支持
	FilterCoreChannelType_BGRA = 2103, //支持
	FilterCoreChannelType_ARGB = 1230, //支持
	FilterCoreChannelType_ABGR = 3210, //支持
};

template<int ChannelType>
class FilterCore
{
public:
	enum //索引值
	{
		Channel_R = ChannelType / 1000 % 10,
		Channel_G = ChannelType / 100 % 10,
		Channel_B = ChannelType / 10 % 10,
		Channel_A = ChannelType % 10,

		Channels = (ChannelType >= FilterCoreChannelType_Luminance && ChannelType <= FilterCoreChannelType_RGB) ? (ChannelType == FilterCoreChannelType_RGB ? 3 : ChannelType + 1) : 4,
	};


protected:

	//适合用于处理无领域采样的情况。
// 	template<typename Func>
// 	static inline void _run(void* dataOut, const void* dataIn, int w, int h, int stride, Func func)
// 	{
// 		const char* dtIn = (const char*)dataIn;
// 		char* dtOut = (char*)dataOut;
// 
// 		typedef char(&DataType)[Channels];
// 
// 		for (int i = 0; < != h; ++i)
// 		{
// 			const int indexH = stride * i;
// 			const unsigned char* lineAddrIn = dtIn + indexH;
// 			unsigned char* lineAddrOut = dtOut + indexH;
// 			for (int j = 0; j < w; j += Channels)
// 			{
// 				const DataType in = lineAddrIn[j];
// 				DataType out = lineAddrOut[j];
// 				func(in, out);
// 			}
// 		}
// 	}

};

#define _FILTER_CODE(...) __VA_ARGS__

//适合用于处理无邻域采样的情况。 参数如下
//void* dataOut, const void* dataIn, int w, int h, int stride
#define _FILTER_RUN(dataOut, dataIn, w, h, stride, code) \
const unsigned char* dtIn = (const unsigned char*)dataIn; \
unsigned char* dtOut = (unsigned char*)dataOut; \
const int w = width * Channels, h = height; \
for (int i = 0; i < h; ++i) \
{ \
	const int indexH = stride * i; \
	const unsigned char* lineAddrIn = dtIn + indexH; \
	unsigned char* lineAddrOut = dtOut + indexH; \
	for (int j = 0; j < stride; j += Channels) \
	{ \
		const unsigned char* in = lineAddrIn + j; \
		unsigned char* out = lineAddrOut + j; \
		code; \
	} \
}\

#ifndef MIN
template <typename T>
static inline T MIN(T a, T b)
{
	return a < b ? a : b;
}
#endif

#ifndef MAX
template <typename T>
static inline T MAX(T a, T b)
{
	return a > b ? a : b;
}
#endif

#ifndef CLAMP
template <typename T>
static inline T CLAMP(T a, T b, T c)
{
	if (a < b)
		return b;
	if (a > c)
		return c;
	return a;
}
#endif

#endif