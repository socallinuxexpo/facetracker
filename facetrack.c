#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gst/gst.h>
#include "main.h"
#include "message_handler.h"

GMainLoop *loop;
int VIDEO_WIDTH = 128;
int VIDEO_HEIGHTt= 72;
int VERBOSE = 0;

int main (int argc, char *argv[]) {
  GstElement *pipeline;
  GstBus *bus;
  guint bus_watch_id;
  GError *error = NULL;
  char launch[5000];
  char framerate[] = "2/1";


  gst_init (&argc, &argv);

//  if (argc != 2) {
//    g_print ("usage: %s <filename>\n", argv[0]);
//    return -1;
//  }
  snprintf( launch, 5000, "v4l2src device=/dev/video0 ! "
             "video/x-raw, format=YUY2, width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! "
              "videoconvert ! "
              "video/x-raw, format=RGB, width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! "
              "videoscale ! "
              "video/x-raw, format=RGB, width=128, height=72, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! "
              "facedetect display=true ! "
              "videoconvert ! "
              "video/x-raw, format=RGBA, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! "
              "videoscale ! "
              "video/x-raw,  width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! "
              "autovideosink") ;
  pipeline = gst_parse_launch (launch , &error);
  if (!pipeline) {
    g_print ("Parse error: %s\n", error->message);
    exit (1);
  }
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  bus = gst_element_get_bus (pipeline);
  bus_watch_id = gst_bus_add_watch (bus, message_handler, NULL);
  gst_object_unref (bus);

  loop = g_main_loop_new (NULL, FALSE);
  g_main_loop_run (loop);

  /* clean up */
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}
