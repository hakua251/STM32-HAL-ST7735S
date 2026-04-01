/*
 * ST7735S.c
 *
 *  Created on: Mar 9, 2026
 *      Author: i
 */
#include "st7735s.h"

ST7735S_InitTypeDef ST7735S_InitStruct;
uint8_t X_Offset;
uint8_t Y_Offset;

static const unsigned char* bitmap_numbers_8x16[] = {
		bitmap_number_0_8x16,
		bitmap_number_1_8x16,
		bitmap_number_2_8x16,
		bitmap_number_3_8x16,
		bitmap_number_4_8x16,
		bitmap_number_5_8x16,
		bitmap_number_6_8x16,
		bitmap_number_7_8x16,
		bitmap_number_8_8x16,
		bitmap_number_9_8x16,
};

static const unsigned char* bitmap_letters_8x16[] = {
		bitmap_letter_a_8x16,
		bitmap_letter_b_8x16,
		bitmap_letter_c_8x16,
		bitmap_letter_d_8x16,
		bitmap_letter_e_8x16,
		bitmap_letter_f_8x16,
		bitmap_letter_g_8x16,
		bitmap_letter_h_8x16,
		bitmap_letter_i_8x16,
		bitmap_letter_j_8x16,
		bitmap_letter_k_8x16,
		bitmap_letter_l_8x16,
		bitmap_letter_m_8x16,
		bitmap_letter_n_8x16,
		bitmap_letter_o_8x16,
		bitmap_letter_p_8x16,
		bitmap_letter_q_8x16,
		bitmap_letter_r_8x16,
		bitmap_letter_s_8x16,
		bitmap_letter_t_8x16,
		bitmap_letter_u_8x16,
		bitmap_letter_v_8x16,
		bitmap_letter_w_8x16,
		bitmap_letter_x_8x16,
		bitmap_letter_y_8x16,
		bitmap_letter_z_8x16,
};


void ST7735S_Init(ST7735S_InitTypeDef *InitStruct){

	ST7735S_InitStruct = *InitStruct;
	ST7735S_Reset(0);
	HAL_Delay(20);
	ST7735S_Reset(1);
	HAL_Delay(120);

	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_SLPOUT);
	HAL_Delay(120);

	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_COLMOD);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi,ST7735S_InitStruct.Color_Mode);

	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_MADCTL);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, ST7735S_InitStruct.Rotation);

	if (ST7735S_InitStruct.Rotation == ST7735S_ROTATION_0 || ST7735S_InitStruct.Rotation == ST7735S_ROTATION_180) {
		X_Offset = (132 - ST7735S_InitStruct.Width)/2;
		Y_Offset = (162 - ST7735S_InitStruct.Height)/2;
	} else {
		X_Offset = (162 - ST7735S_InitStruct.Height)/2;
		Y_Offset = (132 - ST7735S_InitStruct.Width)/2;
		uint8_t temp;
		temp = ST7735S_InitStruct.Width;
		ST7735S_InitStruct.Width = ST7735S_InitStruct.Height;
		ST7735S_InitStruct.Height = temp;
	}
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_FRMCTR1);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi,0x01);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi,0x01);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi,0x01);

	ST7735S_SetWindow(0, 0, ST7735S_InitStruct.Width, ST7735S_InitStruct.Height);
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_DISPON);



}

void ST7735S_SendCommand(SPI_HandleTypeDef *hspi,const uint8_t pData){
	ST7735S_CS(0);
	ST7735S_DC(0);
	HAL_SPI_Transmit(hspi, &pData, 1,HAL_MAX_DELAY);
	ST7735S_CS(1);
}

void ST7735S_SendData(SPI_HandleTypeDef *hspi,const uint8_t *pData,uint16_t size){
	ST7735S_CS(0);
	ST7735S_DC(1);
	HAL_SPI_Transmit(hspi, pData, size, HAL_MAX_DELAY);
	ST7735S_CS(1);
}
void ST7735S_SendByte(SPI_HandleTypeDef *hspi,const uint8_t pData){
	ST7735S_CS(0);
	ST7735S_DC(1);
	HAL_SPI_Transmit(hspi, &pData, 1, HAL_MAX_DELAY);
	ST7735S_CS(1);
}

void ST7735S_FillScreen(uint16_t color){
	ST7735S_DrawFilledRectangle(0, 0, ST7735S_InitStruct.Width, ST7735S_InitStruct.Height, color);
}

void ST7735S_SetWindow(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2){
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_CASET);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, 0x00);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, x1+X_Offset);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, 0x00);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, x2+X_Offset);

	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_RASET);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, 0x00);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, y1+Y_Offset);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, 0x00);
	ST7735S_SendByte(ST7735S_InitStruct.Hspi, y2+Y_Offset);

}

void ST7735S_DrawImage(uint8_t *pData,size_t size){
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_RAMWR);
	ST7735S_SendData(ST7735S_InitStruct.Hspi, pData, size);
}

void ST7735S_DrawOutlinedRectangle(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color){
	ST7735S_DrawFilledRectangle(x, y, width, height, color);
	ST7735S_DrawFilledRectangle(x+1, y+1, width-2, height-2, 0xFFFF);
}

void ST7735S_DrawFilledRectangle(uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color){
	uint8_t buffer[width*sizeof(uint16_t)];

	for(uint8_t i = 0;i < width;i++){
		buffer[i*2] = color >> 8;
		buffer[i*2+1] = color & 0xFF;
	}

	ST7735S_SetWindow(x, y, x+width-1, y+height-1);

	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_RAMWR);

	for(uint8_t i = 0;i < height;i++){
		ST7735S_SendData(ST7735S_InitStruct.Hspi, buffer, width*sizeof(uint16_t));
	}

	ST7735S_SetWindow(0, 0, ST7735S_InitStruct.Width, ST7735S_InitStruct.Height);
}

void ST7735S_DrawPixel(uint8_t x,uint8_t y,uint16_t color){
	uint8_t buffer[sizeof(uint16_t)] = {0};
	buffer[0] = color >> 8;
	buffer[1] = color & 0xFF;
	ST7735S_SetWindow(x, y, x, y);
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_RAMWR);
	ST7735S_SendData(ST7735S_InitStruct.Hspi, buffer, sizeof(uint16_t));
	ST7735S_SetWindow(0, 0, ST7735S_InitStruct.Width-1, ST7735S_InitStruct.Height-1);
}

void ST7735S_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint16_t color){
	uint8_t _x1=0,_y1=0,_x2=0,_y2=0;

	_x1 = x1>x2?x2:x1;
	_x2 = x1<x2?x2:x1;
	_y1 = y1>y2?y2:y1;
	_y2 = y1<y2?y2:y1;

	uint8_t width = (_x2-_x1)+1;
	uint8_t height = (_y2-_y1+1);
	for(uint8_t y = 0;y < height;y++){
		for(uint8_t x = 0;x < width;x++){
			if((double)fabs(x*height-y*width)/(double)sqrt(height*height+width*width)<0.5){
				ST7735S_DrawPixel(x+_x1, y+_y2, color);
			}

		}
	}
	ST7735S_SetWindow(0, 0, ST7735S_InitStruct.Width-1, ST7735S_InitStruct.Height-1);
}

void ST7735S_DrawBitMap(const uint8_t *bitmap,uint8_t x,uint8_t y,uint8_t width,uint8_t height,uint16_t color){

	uint8_t buffer[2];
	uint8_t backgroundcolor[2] = {0xFF,0xFF};

	buffer[0] = color >> 8;
	buffer[1] = color & 0xFF;


	ST7735S_SetWindow(x, y, x+width-1, y+height-1);
	ST7735S_SendCommand(ST7735S_InitStruct.Hspi, ST7735S_RAMWR);
	for (size_t i = 0; i < width*height/8; i++){
		for (int bit = 7; bit >= 0; bit--){
			if (bitmap[i] & (1 << bit)) {
				ST7735S_SendData(ST7735S_InitStruct.Hspi, buffer, 2);
			}else{
				ST7735S_SendData(ST7735S_InitStruct.Hspi, backgroundcolor, 2);
			}
		}
	}
	ST7735S_SetWindow(0, 0, ST7735S_InitStruct.Width-1, ST7735S_InitStruct.Height-1);
}

unsigned char* ST7735S_ScaleBitMap(const unsigned char* src, int src_width, int src_height,int scale) {

    uint8_t dst_width = src_width * scale;
    uint8_t dst_height = src_height * scale;

    int dst_row_bytes = (dst_width) / 8;
    int dst_total_bytes = dst_row_bytes * (dst_height);

    unsigned char* dst = (unsigned char*)calloc(dst_total_bytes, sizeof(unsigned char));
    if (dst == NULL) {
        return NULL;
    }

    for (int y = 0; y < src_height; y++) {
        for (int x = 0; x < src_width; x++) {

            int src_byte_index = y * ((src_width + 7) / 8) + (x / 8);
            int src_bit_index = 7 - (x % 8);
            unsigned char pixel = (src[src_byte_index] >> src_bit_index) & 0x01;

            if (pixel) {

                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        int dst_x = x * scale + dx;
                        int dst_y = y * scale + dy;

                        int dst_byte_index = dst_y * dst_row_bytes + (dst_x / 8);
                        int dst_bit_index = 7 - (dst_x % 8);
                        dst[dst_byte_index] |= (1 << dst_bit_index);
                    }
                }
            }
        }
    }

    return dst;
}

void ST7735S_DrawNumbers(uint8_t x,uint8_t y,float_t num,uint8_t scale,uint16_t color){

		 	 int idx = 0;
		 	 int output[10] = {0};

			if (num < 0) num = -num;

			int integerPart = (int)num;
			float fractionalPart = num - (float)integerPart + 0.000001f;

			int tempInt = integerPart;
			int intDigits[10];
			int intLen = 0;

			if (tempInt == 0) {
				intDigits[intLen++] = 0;
			} else {
				while (tempInt > 0) {
					intDigits[intLen++] = tempInt % 10;
					tempInt /= 10;
				}
			}

			for (int i = intLen - 1; i >= 0; i--) {
				output[idx++] = intDigits[i];
			}

			output[idx++] = -1;

			for (int i = 0; i < 3; i++) {
				fractionalPart *= 10;
				int digit = (int)fractionalPart;
				output[idx++] = digit;
				fractionalPart -= digit;

				if (fractionalPart < 0.0001f) break;
			}

	    int value = 0;
	    unsigned char* bitmap;

	    for (uint8_t i = 0; i < idx; i++) {
	    	value = output[i];
	    	bitmap = ST7735S_ScaleBitMap(value>=0?bitmap_numbers_8x16[value]:bitmap_char_point_8x16,8,16,scale);
	    	ST7735S_DrawBitMap(bitmap, x+scale*8*i, y, scale*8,scale*16, color);
	    	free(bitmap);
		}
}

void ST7735S_DrawString(uint8_t x,uint8_t y,const char* str,uint8_t scale,uint16_t color){
	uint8_t output[10];
	uint8_t *ptr = output;
	while(*str){
		*ptr = *str - 97;
		str++;
		ptr++;

	}
	 unsigned char* bitmap;
	for (uint8_t i = 0; i < sizeof(str)+1; i++) {
		bitmap = ST7735S_ScaleBitMap(bitmap_letters_8x16[output[i]],8,16,scale);
		ST7735S_DrawBitMap(bitmap, x+scale*8*i, y, scale*8,scale*16, color);
		free(bitmap);
	}


}

void ST7735S_CS(uint8_t PinState){
	HAL_GPIO_WritePin(ST7735S_InitStruct.CS_GPIO_Port, ST7735S_InitStruct.CS_GPIO_Pin, PinState);
}
void ST7735S_Reset(uint8_t PinState){
	HAL_GPIO_WritePin(ST7735S_InitStruct.Reset_GPIO_Port, ST7735S_InitStruct.Reset_GPIO_Pin, PinState);
}
void ST7735S_DC(uint8_t PinState){
	HAL_GPIO_WritePin(ST7735S_InitStruct.DC_GPIO_Port, ST7735S_InitStruct.DC_GPIO_Pin, PinState);
}




