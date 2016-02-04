#pragma once

namespace engine {
namespace gfx {
class Bitmap {
 public:
  enum Format {
    Format_Grayscale = 1,
    Format_GrayscaleAlpha = 2,
    Format_RGB = 3,
    Format_RGBA = 4
  };

  Bitmap(unsigned int width,
         unsigned int height,
         Format format,
         const unsigned char *pixels = nullptr);
  Bitmap();
  ~Bitmap();

  unsigned int width() const;
  unsigned int height() const;
  Format format() const;

  unsigned char *pixelBuffer() const;
  unsigned char *getPixel(unsigned int column, unsigned int row) const;
  void setPixel(unsigned int column, unsigned int row, const unsigned char *pixel);
  void flipVertically();

  void rotate90CounterClockwise();

  void copyRectFromBitmap(const Bitmap &src,
                          unsigned srcCol,
                          unsigned srcRow,
                          unsigned destCol,
                          unsigned destRow,
                          unsigned width,
                          unsigned height);

  Bitmap(const Bitmap &other);
  Bitmap &operator=(const Bitmap &other);

 private:
  Format format_;
  unsigned int width_;
  unsigned int height_;
  unsigned char *pixels_;

  void set_(unsigned width, unsigned height, Format format, const unsigned char *pixels);
  static void getPixelOffset_(unsigned col, unsigned row, unsigned width, unsigned height, Format format);
};
}
}