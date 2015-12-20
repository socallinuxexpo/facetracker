#include <stdlib.h>
#include <string.h>
#include <gst/gst.h>
#include "message_handler.h"
#include "main.h"

gboolean message_handler (GstBus *bus, GstMessage *message, gpointer data) {
  switch (GST_MESSAGE_TYPE (message)) {
    case GST_MESSAGE_ERROR: {
      GError *err;
      gchar *debug;

      gst_message_parse_error (message, &err, &debug);
      g_print ("Error: %s\n", err->message);
      g_error_free (err);
      g_free (debug);

      g_main_loop_quit (loop);
      break;
    }
    case GST_MESSAGE_ELEMENT: {
       const GstStructure *strc = gst_message_get_structure(message);
       if( strncmp (gst_structure_get_name(strc),"facedetect",10) == 0 ){
         if( VERBOSE > 2 ){
           g_print ("Got %s message.\n", gst_structure_to_string(strc) );
         }
         const GValue *faces = gst_structure_get_value(strc, "faces");
         gint num_faces = gst_value_list_get_size(faces);
         gint i=0;
         if( GST_VALUE_HOLDS_LIST(faces) ){
           if ( num_faces > 0 ){
             g_print("Found [%i] faces.\n", num_faces );
             for(i=0; i < num_faces; i++){
               g_print("face %i\n", i);
               const GstStructure *face = gst_value_get_structure( gst_value_list_get_value (faces, i) );
               guint x, y, height, width;
               gst_structure_get_uint(face, "x", &x); 
               gst_structure_get_uint(face, "y", &y); 
               gst_structure_get_uint(face, "width", &width); 
               gst_structure_get_uint(face, "height", &height); 
               g_print("X=%i. Y=%i, width=%i, height=%i\n", x, y, width, height);

               // RULE of thirds
                // height / 3 
               if ( x < (24-(height/2)) ){
                  g_print("pan down.\n");
               }
             }
           }
         }
         else{
           g_print("not list\n");
         }

       }
       break;
    }
    case GST_MESSAGE_EOS:
      /* end-of-stream */
      g_main_loop_quit (loop);
      break;
    default:
      /* unhandled message */
      break;
  }

  /* we want to be notified again the next time there is a message
   * on the bus, so returning TRUE (FALSE means we want to stop watching
   * for messages on the bus and our callback should not be called again)
   */
  return TRUE;
}

