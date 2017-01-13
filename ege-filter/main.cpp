/*
* main.cpp
*
*  Created on: 2016-12-13
*      Author: wysaid
*        Blog: wysaid.org
*/

#define SHOW_CONSOLE

#include <cctype>
#include <cmath>

#include "graphics.h"
#include "filterBrightness.h"
#include "filterContrast.h"
#include "filterSaturation.h"
#include "filterThreshold.h"
#include "filterHue.h"
#include "filterEdge.h"

#include "utilityWin32.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

class Slider
{
public:
	Slider(int x, int y, int w, int h) : m_x(x), m_y(y), m_width(w), m_height(h), m_right(x + w), m_bottom(y + h), m_posY((m_y + m_bottom) / 2), m_value(0.0f) {}

	void render()
	{
		setfillcolor(BLUE);
		bar(m_x, m_y, m_right, m_bottom);
		setcolor(YELLOW);
		line(m_x, m_posY, m_right, m_posY);
		setfillcolor(WHITE);
		fillellipse(m_x + 10 + (m_width - 20) * m_value, m_posY, 10, 10);
	}

	inline bool containsPos(int x, int y)
	{
		return (x >= m_x && x <= m_right && y >= m_y && y <= m_bottom);
	}

	inline float value() { return m_value; }
	inline float convertToValue(float posx)
	{
		float v = (posx - m_x - 10) / (float)(m_width - 20);
		return CLAMP(v, 0.0f, 1.0f);
	}

	inline void setValue(float value)
	{
		if (value < 0.0f)
			value = 0.0f;
		else if (value > 1.0f)
			value = 1.0f;

		m_value = value;
	}
	
	inline bool isFirstResponder() { return m_isFirstResponder; }
	inline void setFirstResponder(bool resp) { m_isFirstResponder = resp; }

protected:
	int m_x, m_y, m_width, m_height, m_right, m_bottom;
	int m_posY;
	float m_value;
	bool m_isFirstResponder;
};

enum FilterType
{
	FilterType_Brightness,
	FilterType_Contrast,
	FilterType_Saturation,
	FilterType_Threshold,
	FilterType_Hue,
	FilterType_Edge,
	FilterType_MaxNum,
};

const char* const s_filterNames[] = {
	"brightness(亮度)",
	"contrast(对比度)",
	"saturation(饱和度)",
	"Threshold(阈值分割)", //极简实现
	"Hue(色相)",
	"Edge(查找边缘)",
	"",
};

class Scene
{
public:
	Scene() : m_originImage(nullptr), m_intensity(0.0f), m_isMoving(false), m_isAdjusting(false), m_filteredImage(nullptr), m_currentType(FilterType_Brightness), m_slider(410, 10, 300, 30)
	{
	}

	~Scene()
	{
		if(m_originImage != nullptr)
			delimage(m_originImage);
		if (m_filteredImage != nullptr)
			delimage(m_filteredImage);
	}

	bool handleKeys()
	{
		if (kbhit())
		{
			int c = getch();

			if (m_originImage == nullptr || c == key_space)
			{
#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
				char filename[MAX_PATH];
				if (readFileNameDlg(getHWnd(), filename, "请选择一张图片", utlFILTER_IMAGE))
				{
					if (m_originImage == nullptr)
						m_originImage = newimage();
					if (getimage(m_originImage, filename))
					{
						MessageBoxA(getHWnd(), "加载图片失败!", "呵呵", MB_OK);
					}
					else
					{
						m_imageWidth = getwidth(m_originImage);
						m_imageHeight = getheight(m_originImage);

						//限制所选图片大小, 若过大将等比缩小
						if (m_imageWidth > 800 || m_imageHeight > 800)
						{
							float scaling = MIN(800.0f / m_imageWidth, 800.0f / m_imageHeight);
							int w = ceilf(m_imageWidth * scaling);
							int h = ceilf(m_imageHeight * scaling);
							PIMAGE img = newimage(w, h);
							
							putimage(img, 0, 0, w, h, m_originImage, 0, 0, m_imageWidth, m_imageHeight);
							delimage(m_originImage);
							m_originImage = img;
							m_imageWidth = w;
							m_imageHeight = h;
						}

						m_scaling = 1.0f;
						m_intensity = 0.0f;
						m_x = 0.0f;
						m_y = 0.0f;
					}

					if (m_filteredImage != nullptr)
						delimage(m_filteredImage);
					m_filteredImage = newimage(m_imageWidth, m_imageHeight);
					putimage(m_filteredImage, 0, 0, m_originImage); //复制一份原始图像.
				}
			}
			else
			{
				//按键盘0-9切换滤镜
				int type = c - '0';
				if (type >= 0 && type < FilterType_MaxNum)
				{
//					m_intensity = 1.0f;
					m_currentType = (FilterType)type;
					runFilter();
				}
			}

			flushkey();
			return true;
		}

		return false;
	}

	bool handleMouse()
	{
		if (mousemsg())
		{
			mouse_msg msg;
			do
			{
				msg = getmouse();

			} while (mousemsg() && msg.msg == mouse_msg_move);

			switch (msg.msg)
			{
			case mouse_msg_up:
				m_slider.setFirstResponder(false);
				m_isMoving = false;
				m_isAdjusting = false;
				break;
			case mouse_msg_down:

				if (msg.is_left())
				{
					if (m_slider.containsPos(msg.x, msg.y))
					{
						m_slider.setFirstResponder(true);
					}
					else
					{
						m_isMoving = true;
					}
				}
				else if (msg.is_right())
				{
					m_isAdjusting = true;
				}

				m_lastX = msg.x;
				m_lastY = msg.y;
				break;
			case mouse_msg_move:

				if (m_isMoving)
				{
					m_x += msg.x - m_lastX;
					m_y += msg.y - m_lastY;
					m_lastX = msg.x;
					m_lastY = msg.y;
				}
				else if (m_isAdjusting || m_slider.isFirstResponder())
				{
					if (m_isAdjusting)
					{
						m_intensity += (msg.y - m_lastY) / (float)SCR_HEIGHT;
						m_intensity += (msg.x - m_lastX) / (float)SCR_WIDTH;
						m_lastX = msg.x;
						m_lastY = msg.y;
						m_intensity = CLAMP(m_intensity, 0.0f, 1.0f);
					}
					else
					{
						m_intensity = m_slider.convertToValue(msg.x);
					}

					m_slider.setValue(m_intensity);
					runFilter();
					printf("当前滤镜强度: %g\n", m_intensity);
				}

				break;
			case mouse_msg_wheel:
				m_scaling *= msg.wheel < 0 ? 1.05f : 0.95f;
				m_scaling = CLAMP(m_scaling, 0.2f, 5.0f);
				printf("图片放大: %g\n", m_scaling);
				break;
			default:
				break;
			}

			return true;
		}
		return false;
	}

	bool update()
	{
		handleKeys();
		handleMouse();

		return true;
	}

	void render()
	{
		cleardevice(0);
		static int hue = 0;
		if (m_filteredImage != nullptr)
		{
			++hue;
			hue %= 360;
			setcolor(hsv2rgb(hue, 1.0f, 1.0f));
			putimage(m_x, m_y, m_imageWidth * m_scaling, m_imageHeight * m_scaling, m_filteredImage, 0, 0, m_imageWidth, m_imageHeight);
			outtextrect(10, 10, 400, 100, "按空格键重新选择图片!\n"
				"使用鼠标左键可以拖拽图片， 鼠标滚轮放大或缩小图片.\n"
				"使用鼠标左键拖动右侧滚动条调节滤镜强度.\n"
				"使用鼠标右键上下左右拖动界面调整滤镜强度.\n"
				"使用数字键0-9切换滤镜效果");

			outtextxy(300, 300, s_filterNames[m_currentType]);
			m_slider.render();
		}
		else
		{
			outtextxy(100, 100, "按键盘任意键选择加载图片!");
		}
	}

	void runFilter()
	{
		if (m_originImage == nullptr || m_filteredImage == nullptr)
			return;

		color_t* srcBuffer = getbuffer(m_originImage);
		color_t* dstBuffer = getbuffer(m_filteredImage);
		int stride = m_imageWidth * 4;
		float intensity;
		switch (m_currentType)
		{
		case FilterType_Brightness:
			intensity = m_intensity * 2.0f - 1.0f;
			FilterBrightness<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, intensity);
			break;
		case FilterType_Contrast:
			intensity = m_intensity * 5.0f - 2.0f;
			FilterContrast<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, intensity);
			break;
		case FilterType_Saturation:
			intensity = m_intensity * 4.5f;
			FilterSaturation<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, intensity);
			break;
		case FilterType_Threshold:
			FilterThreshold<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, m_intensity);
			break;
		case FilterType_Hue:
			intensity = m_intensity * (3.14159f * 2.0f);
			FilterHue<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, intensity);
			break;
		case FilterType_Edge:
			cleardevice(m_filteredImage);
			FilterEdge<FilterCoreChannelType_BGRA>::run(dstBuffer, srcBuffer, m_imageWidth, m_imageHeight, stride, m_intensity);
		default:
			break;
		}
	}

protected:

	PIMAGE m_originImage, m_filteredImage;
	int m_x, m_y;
	int m_lastX, m_lastY;
	int m_imageWidth, m_imageHeight;
	float m_scaling, m_intensity;
	bool m_isMoving, m_isAdjusting;

	FilterType m_currentType;

	Slider m_slider;
};

int main()
{
	initgraph(SCR_WIDTH, SCR_HEIGHT, INIT_RENDERMANUAL);
	setbkmode(TRANSPARENT);
	setcolor(YELLOW);

	Scene scene;

	for (; is_run(); delay_fps(60))
	{
		if (scene.update())
		{
			scene.render();
		}
	}

	getch();
	closegraph();
	return 0;
}