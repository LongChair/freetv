#ifndef DISPMANXIMAGE_H
#define DISPMANXIMAGE_H

#include "bcm_host.h"
#include "interface/vmcs_host/vc_dispmanx.h"

class DispmanxImage
{
public:
  DispmanxImage(int32_t width, int32_t height, VC_IMAGE_TYPE_T type = VC_IMAGE_ARGB8888,
                int32_t layer = 0);
  ~DispmanxImage();

  bool create();
  bool destroy();
  void fill(int r, int g, int b, int a);

private:
  int m_screen;
  int m_display;

  VC_IMAGE_TYPE_T m_type;
  int32_t m_width;
  int32_t m_height;
  int32_t m_pitch;
  uint16_t m_bitsPerPixel;
  int32_t m_alignedHeight;
  uint32_t m_size;
  int32_t m_layer;
  void* m_buffer;

  int m_resource;
  uint32_t m_imageptr;
  VC_RECT_T m_srcRect;
  VC_RECT_T m_dstRect;
  DISPMANX_ELEMENT_HANDLE_T m_element;
};

#endif /* DISPMANXIMAGE_H */
