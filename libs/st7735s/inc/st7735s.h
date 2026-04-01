/*
 * ST7735S.h
 *
 *  Created on: Mar 9, 2026
 *      Author: i
 */

#ifndef INC_ST7735S_H_
#define INC_ST7735S_H_
#endif

#include "stm32f1xx_hal.h"
#include "spi.h"
#include "math.h"
#include "fonts.h"
#include "usart.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct{
	SPI_HandleTypeDef *Hspi;
	uint8_t Height;
	uint8_t Width;
	uint8_t Rotation;
	uint8_t Color_Mode;
	GPIO_TypeDef *Reset_GPIO_Port;
	GPIO_TypeDef *CS_GPIO_Port;
	GPIO_TypeDef *DC_GPIO_Port;
	uint16_t Reset_GPIO_Pin;
	uint16_t CS_GPIO_Pin;
	uint16_t DC_GPIO_Pin;
} ST7735S_InitTypeDef;


#define ST7735S_COLOR_MODE_444 			0x03
#define ST7735S_COLOR_MODE_565 			0x05
#define ST7735S_COLOR_MODE_666 			0x06

#define ST7735S_ROTATION_0 					0x00
#define ST7735S_ROTATION_90 				0x60
#define ST7735S_ROTATION_180 				0xC0
#define ST7735S_ROTATION_270 				0xA0


#define ST7735S_SLPIN       0x10 // 进入睡眠模式
#define ST7735S_SLPOUT      0x11 // 退出睡眠模式 (待机120ms)
#define ST7735S_NORON       0x13 // 开启正常显示模式
#define ST7735S_INVOFF      0x20 // 关闭显示反转
#define ST7735S_INVON       0x21 // 开启显示反转
#define ST7735S_DISPOFF     0x28 // 关闭显示
#define ST7735S_DISPON      0x29 // 开启显示
#define ST7735S_CASET       0x2A // 设置列地址 (XS, XE)
#define ST7735S_RASET       0x2B // 设置行地址 (YS, YE)
#define ST7735S_RAMWR       0x2C // 写入显存
#define ST7735S_SCRLAR      0x33 // 设置垂直滚动区域
#define ST7735S_MADCTL      0x36 // 存储数据访问控制 (方向控制)
#define ST7735S_VSCSAD      0x37 // 垂直滚动起始地址
#define ST7735S_COLMOD      0x3A // 接口像素格式 (如 16-bit RGB)
#define ST7735S_FRMCTR1     0xB1 // 帧率控制 (正常模式)
#define ST7735S_FRMCTR2     0xB2 // 帧率控制 (闲置模式)
#define ST7735S_FRMCTR3     0xB3 // 帧率控制 (部分模式)
#define ST7735S_INVCTR      0xB4 // 显示反转控制


void ST7735S_Init(ST7735S_InitTypeDef *InitStruct);
void ST7735S_SendCommand(SPI_HandleTypeDef *hspi,const uint8_t pData);
void ST7735S_SendData(SPI_HandleTypeDef *hspi,const uint8_t *pData,uint16_t size);
void ST7735S_SendByte(SPI_HandleTypeDef *hspi,const uint8_t pData);
void ST7735S_SetWindow(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);
void ST7735S_DrawFilledRectangle(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color);
void ST7735S_FillScreen(uint16_t color);
void ST7735S_DrawPixel(uint8_t x,uint8_t y,uint16_t color);
void ST7735S_DrawOutlinedRectangle(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color);
void ST7735S_DrawImage(uint8_t *pData,size_t size);
void ST7735S_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color);
void ST7735S_DrawBitMap(const uint8_t *bitmap,uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color);
void ST7735S_DrawNumbers(uint8_t x,uint8_t y,float_t num,uint8_t scale,uint16_t color);
void ST7735S_DrawString(uint8_t x,uint8_t y,const char* str,uint8_t scale,uint16_t color);
void ST7735S_SetRotation(uint8_t rotation);

void ST7735S_Reset(uint8_t PinState);
void ST7735S_CS(uint8_t PinState);
void ST7735S_DC(uint8_t PinState);

unsigned char* ST7735S_ScaleBitMap(const unsigned char* src, int src_width, int src_height,int scale);


