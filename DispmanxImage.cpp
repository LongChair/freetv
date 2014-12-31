#include "DispmanxImage.h"
#include <QDebug>

#ifndef ALIGN_TO_16
#define ALIGN_TO_16(x) ((x + 15) & ~15)
#endif

#define DISPMANX_LOGDEBUG qDebug() << "Displamanx log :"
#define DISPMANX_LOGERROR qDebug() << "Displamanx error :"

///////////////////////////////////////////////////////////////////////////////////////////////////
DispmanxImage::DispmanxImage(int32_t width, int32_t height, VC_IMAGE_TYPE_T type, int32_t layer)
{
  m_screen = 0;
  m_display = 0;
  m_width = width;
  m_height = height;
  m_bitsPerPixel = 32;
  m_pitch = (ALIGN_TO_16(m_width) * m_bitsPerPixel) / 8;
  m_alignedHeight = ALIGN_TO_16(m_height);
  m_size = m_pitch * m_alignedHeight;
  m_type = type;
  m_layer = layer;
  m_buffer = NULL;

  m_resource = 0;
  m_element = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
DispmanxImage::~DispmanxImage()
{
  destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool DispmanxImage::create()
{
  int ret;

  m_buffer = calloc(1, m_size);
  if (!m_buffer)
  {
    DISPMANX_LOGERROR << "Out of memory";
    return false;
  }

  fill(0, 0, 0, 255);

  // openthe display
  m_display = vc_dispmanx_display_open(m_screen);
  if (!m_display)
  {
    DISPMANX_LOGERROR << "Failed to open display";
    return false;
  }

  // create the ressource
  m_resource = vc_dispmanx_resource_create(m_type, m_width | (m_pitch << 16),
                                           m_height | (m_alignedHeight << 16), &m_imageptr);

  if (!m_resource)
  {
    DISPMANX_LOGERROR << "Failed to create ressource";
    return false;
  }

  // create the source and destination rects
  vc_dispmanx_rect_set(&m_srcRect, 0 << 16, 0 << 16, m_width << 16, m_height << 16);

  vc_dispmanx_rect_set(&m_dstRect, 0, 0, m_width, m_height);

  // write source data to ressource
  ret = vc_dispmanx_resource_write_data(m_resource, m_type, m_pitch, m_buffer, &m_dstRect);

  if (ret)
  {
    DISPMANX_LOGERROR << "Failed to write to ressource";
    return false;
  }

  // prepare the update
  DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(10);

  if (!update)
  {
    DISPMANX_LOGERROR << "Failed to start update";
    return false;
  }

  // create the element
  VC_DISPMANX_ALPHA_T alpha;
  alpha.flags = (DISPMANX_FLAGS_ALPHA_T)(DISPMANX_FLAGS_ALPHA_FROM_SOURCE);
  alpha.opacity = 255;
  alpha.mask = 0x0;

  m_element = vc_dispmanx_element_add(update, m_display, m_layer, // layer
                                      &m_dstRect, m_resource, &m_srcRect, DISPMANX_PROTECTION_NONE,
                                      &alpha, NULL, // clamp
                                      DISPMANX_NO_ROTATE);

  if (!m_element)
  {
    DISPMANX_LOGERROR << "Failed to add the element";
    return false;
  }

  // now do the update
  ret = vc_dispmanx_update_submit_sync(update);
  if (ret)
  {
    DISPMANX_LOGERROR << "Failed to submit the update";
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool DispmanxImage::destroy()
{
  int ret;

  if (m_buffer)
  {
    free(m_buffer);
    m_buffer = NULL;
  }

  // prepare the update
  DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);

  if (!update)
  {
    DISPMANX_LOGERROR << "Failed to start update";
    return false;
  }

  // remove the element
  if (m_element)
  {
    ret = vc_dispmanx_element_remove(update, m_element);
    if (ret)
    {
      DISPMANX_LOGERROR << "Failed to remove the element";
    }
    m_element = 0;
  }

  // now do the update
  ret = vc_dispmanx_update_submit_sync(update);
  if (ret)
  {
    DISPMANX_LOGERROR << "Failed to submit the update";
  }

  // delete the ressource
  if (m_resource)
  {
    ret = vc_dispmanx_resource_delete(m_resource);
    if (ret)
    {
      DISPMANX_LOGERROR << "Failed to delete the ressource";
    }
    m_resource = 0;
  }

  // close the display
  if (m_display)
  {
    ret = vc_dispmanx_display_close(m_display);
    if (ret)
    {
      DISPMANX_LOGERROR << "Failed to close display";
    }
    m_display = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void DispmanxImage::fill(int r, int g, int b, int a)
{
  unsigned int value = (a << 24) + (r << 16) + (g << 8) + b;

  if (m_buffer)
  {
    int* ptr = (int*)m_buffer;
    for (int i = 0; i < m_size / 4; i++)
      *ptr++ = value;
  }
}
