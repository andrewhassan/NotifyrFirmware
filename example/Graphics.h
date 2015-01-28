#ifndef GRAPHICS
#define GRAPHICS

#include "stm32l1xx.h"
#include "bitmap_db.h"
#include "stdbool.h"

#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#define X_PADDING 4
#define Y_PADDING 4


void drawCircle (int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void fillRoundRect(int16_t x, int16_t y, int16_t w,	int16_t h, int16_t r, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
void fillTriangle ( int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
void drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color);
void writeString(char *string, bool useAwesomeFonts);
void write(char c, bool useNewFonts);
void drawChar(int16_t x, int16_t y,  char c,uint16_t color, uint16_t bg, uint8_t size);
void setCursor(int16_t x, int16_t y);
void setTextSize(uint8_t s);
void setTextColor(uint16_t c, uint16_t b);
void setFont(const FONT_INFO *font_info, const FONT_CHAR_INFO *font_lookup_table, const uint8_t *font_data);
const FONT_INFO* getFontInfo();

#endif
