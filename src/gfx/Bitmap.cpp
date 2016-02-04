#include "Bitmap.h"

#include <stdexcept>
#include <cstdlib>
#include <stdio.h>
#include <string>

using namespace engine::gfx;

inline unsigned char AverageRGB(unsigned char rgb[3]) {
    return (unsigned char)(((double)rgb[0] + (double)rgb[1] + (double)rgb[2]) / 3.0);
}

static void grayToGrayAlpha(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = 255;
}

static void grayToRGB(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
}

static void grayToRGBA(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
    dest[3] = 255;
}

static void grayAlphaToGray(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
}

static void grayAlphaToRGB(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
}

static void grayAlphaToRGBA(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[0];
    dest[2] = src[0];
    dest[3] = src[1];
}

static void RGBToGray(unsigned char* src, unsigned char* dest){
    dest[0] = AverageRGB(src);
}

static void RGBToGrayAlpha(unsigned char* src, unsigned char* dest){
    dest[0] = AverageRGB(src);
    dest[1] = 255;
}

static void RGB2RGBA(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    dest[3] = 255;
}

static void RGBAToGray(unsigned char* src, unsigned char* dest){
    dest[0] = AverageRGB(src);
}

static void RGBAToGrayAlpha(unsigned char* src, unsigned char* dest){
    dest[0] = AverageRGB(src);
    dest[1] = src[3];
}

static void RGBA2RGB(unsigned char* src, unsigned char* dest){
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
}

typedef void(*FormatConverterFunc)(unsigned char*, unsigned char*);

static FormatConverterFunc ConverterFuncForFormats(Bitmap::Format srcFormat, Bitmap::Format destFormat){
    if(srcFormat == destFormat)
        throw std::runtime_error("Just use memcpy if pixel formats are the same");

    switch(srcFormat){

        case Bitmap::Format_Grayscale:
            switch(destFormat){
                case Bitmap::Format_GrayscaleAlpha: return grayToGrayAlpha;
                case Bitmap::Format_RGB:            return grayToRGB;
                case Bitmap::Format_RGBA:           return grayToRGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;

        case Bitmap::Format_GrayscaleAlpha:
            switch(destFormat){
                case Bitmap::Format_Grayscale: return grayAlphaToGray;
                case Bitmap::Format_RGB:       return grayAlphaToRGB;
                case Bitmap::Format_RGBA:      return grayAlphaToRGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;

        case Bitmap::Format_RGB:
            switch(destFormat){
                case Bitmap::Format_Grayscale:      return RGBToGray;
                case Bitmap::Format_GrayscaleAlpha: return RGBToGrayAlpha;
                case Bitmap::Format_RGBA:           return RGB2RGBA;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;

        case Bitmap::Format_RGBA:
            switch(destFormat){
                case Bitmap::Format_Grayscale:      return RGBAToGray;
                case Bitmap::Format_GrayscaleAlpha: return RGBAToGrayAlpha;
                case Bitmap::Format_RGB:            return RGBA2RGB;
                default:
                    throw std::runtime_error("Unhandled bitmap format");
            }
            break;

        default:
            throw std::runtime_error("Unhandled bitmap format");
    }
}


/*
 * Misc funcs
 */

inline unsigned GetPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Bitmap::Format format) {
    return (row*width + col)*format;
}

inline bool RectsOverlap(unsigned srcCol, unsigned srcRow, unsigned destCol, unsigned destRow, unsigned width, unsigned height){
    unsigned colDiff = srcCol > destCol ? srcCol - destCol : destCol - srcCol;
    if(colDiff < width)
        return true;

    unsigned rowDiff = srcRow > destRow ? srcRow - destRow : destRow - srcRow;
    if(rowDiff < height)
        return true;

    return false;
}


/*
 * Bitmap class
 */

Bitmap::Bitmap(unsigned width,
               unsigned height,
               Format format,
               const unsigned char* pixels) :
pixels_(NULL)
{
    set_(width, height, format, pixels);
    flipVertically();
}

Bitmap::Bitmap() : Bitmap(0,0,Format_Grayscale){}

Bitmap::~Bitmap() {
    if(pixels_) free(pixels_);
}

Bitmap::Bitmap(const Bitmap& other) :
pixels_(NULL)
{
    set_(other.width_, other.height_, other.format_, other.pixels_);
}

Bitmap& Bitmap::operator = (const Bitmap& other) {
    set_(other.width_, other.height_, other.format_, other.pixels_);
    return *this;
}

unsigned int Bitmap::width() const {
    return width_;
}

unsigned int Bitmap::height() const {
    return height_;
}

Bitmap::Format Bitmap::format() const {
    return format_;
}

unsigned char* Bitmap::pixelBuffer() const {
    return pixels_;
}

unsigned char* Bitmap::getPixel(unsigned int column, unsigned int row) const {
    if(column >= width_ || row >= height_)
        throw std::runtime_error("Pixel coordinate out of bounds");

    return pixels_ + GetPixelOffset(column, row, width_, height_, format_);
}

void Bitmap::setPixel(unsigned int column, unsigned int row, const unsigned char* pixel) {
    unsigned char* myPixel = getPixel(column, row);
    memcpy(myPixel, pixel, format_);
}

void Bitmap::flipVertically() {
    unsigned long rowSize = format_*width_;
    unsigned char* rowBuffer = new unsigned char[rowSize];
    unsigned halfRows = height_ / 2;

    for(unsigned rowIdx = 0; rowIdx < halfRows; ++rowIdx){
        unsigned char* row = pixels_ + GetPixelOffset(0, rowIdx, width_, height_, format_);
        unsigned char* oppositeRow = pixels_ + GetPixelOffset(0, height_ - rowIdx - 1, width_, height_, format_);

        memcpy(rowBuffer, row, rowSize);
        memcpy(row, oppositeRow, rowSize);
        memcpy(oppositeRow, rowBuffer, rowSize);
    }

    delete rowBuffer;
}

void Bitmap::rotate90CounterClockwise() {
    unsigned char* newPixels = (unsigned char*) malloc(format_*width_*height_);

    for(unsigned row = 0; row < height_; ++row){
        for(unsigned col = 0; col < width_; ++col){
            unsigned srcOffset = GetPixelOffset(col, row, width_, height_, format_);
            unsigned destOffset = GetPixelOffset(row, width_ - col - 1, height_, width_, format_);
            memcpy(newPixels + destOffset, pixels_ + srcOffset, format_); //copy one pixel
        }
    }

    free(pixels_);
    pixels_ = newPixels;

    unsigned swapTmp = height_;
    height_ = width_;
    width_ = swapTmp;
}

void Bitmap::copyRectFromBitmap(const Bitmap& src,
                                unsigned srcCol,
                                unsigned srcRow,
                                unsigned destCol,
                                unsigned destRow,
                                unsigned width,
                                unsigned height)
{
    if (srcCol == 0 && srcRow == 0 && width == 0 && height == 0){
        width = src.width();
        height = src.height();
    }

    if(width == 0 || height == 0){
        throw std::runtime_error("Can't copy zero height/width rectangle");
    }
    if(srcCol + width >= src.width() || srcRow + height >= src.height()){
        throw std::runtime_error("Rectangle doesn't fit within source bitmap");
    }
    if(destCol + width >= width_ || destRow + height >= height_){
        throw std::runtime_error("Rectangle doesn't fit within destination bitmap");
    }
    if(pixels_ == src.pixels_ && RectsOverlap(srcCol, srcRow, destCol, destRow, width, height)){
        throw std::runtime_error("Source and destination are the same bitmap, and rects overlap. Not allowed!");
    }
    FormatConverterFunc converter = NULL;
    if(format_ != src.format_){
        converter = ConverterFuncForFormats(format_, src.format_);
    }
    for(unsigned row = 0; row < height; ++row){
        for(unsigned col = 0; col < width; ++col){
            unsigned char* srcPixel = src.pixels_ + GetPixelOffset(srcCol + col, srcRow + row, src.width_, src.height_, src.format_);
            unsigned char* destPixel = pixels_ + GetPixelOffset(destCol + col, destRow + row, width_, height_, format_);

            if(converter){
                converter(srcPixel, destPixel);
            } else {
                memcpy(destPixel, srcPixel, format_);
            }
        }
    }
}

void Bitmap::set_(unsigned width,
                  unsigned height,
                  Format format,
                  const unsigned char* pixels)
{
    if(width == 0) throw std::runtime_error("Zero width bitmap");
    if(height == 0) throw std::runtime_error("Zero height bitmap");
    if(format <= 0 || format > 4) throw std::runtime_error("Invalid bitmap format");

    width_ = width;
    height_ = height;
    format_ = format;

    size_t newSize = width_ * height_ * format_;
    if(pixels_){
        pixels_ = (unsigned char*)realloc(pixels_, newSize);
    } else {
        pixels_ = (unsigned char*)malloc(newSize);
    }

    if(pixels){
        memcpy(pixels_, pixels, newSize);
    }
}