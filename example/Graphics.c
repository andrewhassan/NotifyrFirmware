#include "font.h"
#include "LCD_Driver.h"
#include "Graphics.h"

int16_t cursor_x = X_PADDING;
int16_t cursor_y = 0;
uint16_t textcolor = BLACK;
uint16_t textbgcolor = WHITE;

uint8_t textsize = 1;
uint8_t wrap = 1; // If set, 'wrap' text at right edge of display
uint8_t rotation = 0;

const uint8_t *fontBitmaps;
const FONT_INFO *fontInfo;
const FONT_CHAR_INFO *fontLookupTable;

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

// Used to do circles and roundrects
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
		int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

// Bresenham's algorithm - thx wikpedia
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		SWAP(int, x0, y0);
		SWAP(int, x1, y1);
	}

	if (x0 > x1) {
		SWAP(int, x0, x1);
		SWAP(int, y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

// Draw a rectangle
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	// Update in subclasses if desired!
	drawLine(x, y, x, y + h - 1, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	// Update in subclasses if desired!
	drawLine(x, y, x + w - 1, y, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	// Update in subclasses if desired!
	int16_t i;
	for (i = x; i < x + w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

void fillScreen(uint16_t color) {
	clearMem(color);
}

void fill_circle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Draw a rounded rectangle
void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
		uint16_t color) {
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r, color); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
	drawFastVLine(x, y + r, h - 2 * r, color); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
	// draw four corners
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

// Fill a rounded rectangle
void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
		uint16_t color) {
	// smarter version
	fillRect(x + r, y, w - 2 * r, h, color);

	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color) {
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
		int16_t y2, uint16_t color) {

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1) {
		SWAP(int, y0, y1);
		SWAP(int, x0, x1);
	}
	if (y1 > y2) {
		SWAP(int, y2, y1);
		SWAP(int, x2, x1);
	}
	if (y0 > y1) {
		SWAP(int, y0, y1);
		SWAP(int, x0, x1);
	}

	if (y0 == y2) { // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
			a = x1;
		else if (x1 > b)
			b = x1;
		if (x2 < a)
			a = x2;
		else if (x2 > b)
			b = x2;
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
			dx12 = x2 - x1, dy12 = y2 - y1, sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for (y = y0; y <= last; y++) {
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		 a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			SWAP(int, a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for (; y <= y2; y++) {
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		 a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		 b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		 */
		if (a > b)
			SWAP(int, a, b);
		drawFastHLine(a, y, b - a + 1, color);
	}
}

void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w,
		int16_t h, uint16_t color) {

	int16_t i, j, byteWidth = (w + 7) / 8;

	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			if (bitmap[(j * byteWidth + i / 8)] & (128 >> (i & 7))) {
				drawPixel(x + i, y + j, color);
			}else{
				drawPixel(x + i, y + j, !color);
			}
		}
	}
}

void writeString(char *string, bool useAwesomeFonts) {
	int i;
	for (i = 0; string[i] > 1; i++) {
		write(string[i], useAwesomeFonts);
	}
}

void write(char c, bool useNewFonts) {
	if (c == '\n') {
		cursor_y +=  useNewFonts ? fontInfo->height : textsize*8;
		cursor_x = X_PADDING;
	} else if (c == '\r') {
		// skip em
	} else {
		if (useNewFonts) {
			drawFontChar(cursor_x, cursor_y, c, textcolor, textbgcolor,
					textsize);
			cursor_x += fontLookupTable[c - fontInfo->startChar].widthBits+2;
			if (wrap
					&& (cursor_x
							> (LCD_WIDTH - X_PADDING
									- fontLookupTable[c - fontInfo->startChar].widthBits
									- 2))) {
				cursor_y += fontInfo->height;
				cursor_x = X_PADDING;
			}
		} else {
			drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
			cursor_x += textsize * 6;
			if (wrap && (cursor_x > (LCD_WIDTH - X_PADDING - textsize * 6))) {
				cursor_y += textsize * 8;
				cursor_x = X_PADDING;
			}
		}
	}
}


void drawFontChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg) {
	uint8_t i, j;
	uint16_t index = fontLookupTable[c - fontInfo->startChar].offset;
	uint8_t width = fontLookupTable[c - fontInfo->startChar].widthBits;
	int16_t byteWidth = (width + 7) / 8;
	if ((x >= LCD_WIDTH) || // Clip right
			(y >= LCD_HEIGHT))
		return;

	const uint8_t *charPointer = (fontBitmaps + index);

	for (j = 0; j < fontInfo->height; j++) {
		for (i = 0; i < width; i++) {
			if (charPointer[(j * byteWidth + i / 8)] & (128 >> (i & 7))) {
				drawPixel(x + i, y + j, color);
			} else {
				drawPixel(x + i, y + j, bg);
			}
		}

	}
}

// Draw a character
void drawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg,
		uint8_t size) {

	int8_t i, j;
	if ((x >= LCD_WIDTH) || // Clip right
			(y >= LCD_HEIGHT) || // Clip bottom
			((x + 6 * size - 1) < 0) || // Clip left
			((y + 8 * size - 1) < 0)) // Clip top
		return;

	for (i = 0; i < 6; i++) {
		uint8_t line;
		if (i == 5) {
			line = 0x0;
		} else {
			line = font[(c * 5) + i];
		}
		for (j = 0; j < 8; j++) {
			if (line & 0x1) {
				if (size == 1) // default size
					drawPixel(x + i, y + j, color);
				else { // big size
					fillRect(x + (i * size), y + (j * size), size, size, color);
				}
			} else if (bg != color) {
				if (size == 1) // default size
					drawPixel(x + i, y + j, bg);
				else { // big size
					fillRect(x + i * size, y + j * size, size, size, bg);
				}
			}
			line >>= 1;
		}
	}
}

void setCursor(int16_t x, int16_t y) {
	cursor_x = x;
	cursor_y = y;
}

void setTextSize(uint8_t s) {
	textsize = (s > 0) ? s : 1;
}

void setTextColor(uint16_t c, uint16_t b) {
	textcolor = c ? BLACK : WHITE;
	textbgcolor = b ? BLACK : WHITE;
}

void setFont(const FONT_INFO *font_info, const FONT_CHAR_INFO *font_lookup_table,
		const uint8_t *font_data) {
	fontInfo = font_info;
	fontLookupTable = font_lookup_table;
	fontBitmaps = font_data;
}

const FONT_INFO* getFontInfo(){
	return fontInfo;
}
