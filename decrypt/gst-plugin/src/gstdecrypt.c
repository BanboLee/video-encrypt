/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2017 banbo <<user@hostname.org>>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-decrypt
 *
 * FIXME:Describe decrypt here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! decrypt ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <stdint.h>

#include "gstdecrypt.h"
#include "speck.h"

GST_DEBUG_CATEGORY_STATIC (gst_decrypt_debug);
#define GST_CAT_DEFAULT gst_decrypt_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */
static GstStaticPadTemplate sink_factory = GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

static GstStaticPadTemplate src_factory = GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("ANY")
    );

#define gst_decrypt_parent_class parent_class
G_DEFINE_TYPE (Gstdecrypt, gst_decrypt, GST_TYPE_ELEMENT);

static void gst_decrypt_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec);
static void gst_decrypt_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec);

static gboolean gst_decrypt_sink_event (GstPad * pad, GstObject * parent, GstEvent * event);
static GstFlowReturn gst_decrypt_chain (GstPad * pad, GstObject * parent, GstBuffer * buf);

/* GObject vmethod implementations */

/* initialize the decrypt's class */
static void
gst_decrypt_class_init (GstdecryptClass * klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass *) klass;
  gstelement_class = (GstElementClass *) klass;

  gobject_class->set_property = gst_decrypt_set_property;
  gobject_class->get_property = gst_decrypt_get_property;

  g_object_class_install_property (gobject_class, PROP_SILENT,
      g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
          FALSE, G_PARAM_READWRITE));

  gst_element_class_set_details_simple(gstelement_class,
    "decrypt",
    "FIXME:Generic",
    "FIXME:Generic Template Element",
    "banbo <<user@hostname.org>>");

  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&src_factory));
  gst_element_class_add_pad_template (gstelement_class,
      gst_static_pad_template_get (&sink_factory));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_decrypt_init (Gstdecrypt * filter)
{
  filter->sinkpad = gst_pad_new_from_static_template (&sink_factory, "sink");
  gst_pad_set_event_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_decrypt_sink_event));
  gst_pad_set_chain_function (filter->sinkpad,
                              GST_DEBUG_FUNCPTR(gst_decrypt_chain));
  GST_PAD_SET_PROXY_CAPS (filter->sinkpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);

  filter->srcpad = gst_pad_new_from_static_template (&src_factory, "src");
  GST_PAD_SET_PROXY_CAPS (filter->srcpad);
  gst_element_add_pad (GST_ELEMENT (filter), filter->srcpad);

  filter->silent = FALSE;
}

static void
gst_decrypt_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  Gstdecrypt *filter = GST_DECRYPT (object);

  switch (prop_id) {
    case PROP_SILENT:
      filter->silent = g_value_get_boolean (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_decrypt_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  Gstdecrypt *filter = GST_DECRYPT (object);

  switch (prop_id) {
    case PROP_SILENT:
      g_value_set_boolean (value, filter->silent);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* this function handles sink events */
static gboolean
gst_decrypt_sink_event (GstPad * pad, GstObject * parent, GstEvent * event)
{
  gboolean ret;
  Gstdecrypt *filter;

  filter = GST_DECRYPT (parent);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps * caps;

      gst_event_parse_caps (event, &caps);
      /* do something with the caps */

      /* and forward */
      ret = gst_pad_event_default (pad, parent, event);
      break;
    }
    default:
      ret = gst_pad_event_default (pad, parent, event);
      break;
  }
  return ret;
}

/* chain function
 * this function does the actual processing
 */
static GstFlowReturn
gst_decrypt_chain (GstPad * pad, GstObject * parent, GstBuffer * buf)
{
  Gstdecrypt *filter;

  filter = GST_DECRYPT (parent);

  if (filter->silent == FALSE) {
    gint buf_size;
    buf_size = gst_buffer_get_size(buf);
    static gint buf_num = 0;
    buf_num ++;
    GstMapInfo map;
    if (gst_buffer_map(buf, &map, GST_MAP_WRITE)) {
      guint8 *data = map.data;
      guint8 *after = data;
      uint32_t *point = (uint32_t *)data;
      // g_print("before decrypt buf num = %d size = %d\n", buf_num, map.size);
      // int k;
      // g_print("Data is :");
      // for (k = 0; k < 10; k++) {
      //   g_print(" 0x%02x", *data);
      //   data++;
      // }
      // g_print("\n");

      int groups;
      int rounds;
      KEYTYPE initKey[] = {0x12457863, 0x73194682, 0x97436155};
      KEYTYPE keys[26] = {0};
      key_schedule(initKey, keys);
      groups = map.size / 4;
      int flag = 1;
      if (groups % 2 == 0) {
        rounds = groups / 2;
      } else {
        rounds = groups / 2 + 1;
        flag = 0;
      }
      int i;
      for (i = 0; i < rounds; i++) {
        if(flag == 0 && i == rounds - 1){
          DATATYPE pt[2];
          pt[0] = *point;
          pt[1] = 0x28490022;
          decrypt(pt, pt, keys);
          *point = pt[0];
        } else {
          uint32_t *head;
          uint32_t *tail;
          DATATYPE pt[2];
          head = point;
          pt[0] = *head;
          point++;
          tail = point;
          pt[1] = *tail;
          point++;
          decrypt(pt, pt, keys);
          *head = pt[0];
          *tail = pt[1];
        }
      }
      // g_print("after decrypt buf num = %d size = %d\n", buf_num, map.size);
      // int j;
      // g_print("Data is :");
      // for (j = 0; j < 10; j++) {
      //   g_print(" 0x%02x", *after);
      //   after++;
      // }
      // g_print("\n");
      gst_buffer_unmap(buf, &map);
    }
  }
  /* just push out the incoming buffer without touching it */
  return gst_pad_push (filter->srcpad, buf);
}


/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
decrypt_init (GstPlugin * decrypt)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template decrypt' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_decrypt_debug, "decrypt",
      0, "Template decrypt");

  return gst_element_register (decrypt, "decrypt", GST_RANK_NONE,
      GST_TYPE_DECRYPT);
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstdecrypt"
#endif

/* gstreamer looks for this structure to register decrypts
 *
 * exchange the string 'Template decrypt' with your decrypt description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    decrypt,
    "Template decrypt",
    decrypt_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
