#!/usr/bin/python3

import gi
gi.require_version('Gst', '1.0')
from gi.repository import GObject, Gst, GstVideo, Gtk
from pprint import pprint

GObject.threads_init()
Gst.init(None)
loop=0

class Webcam():
  playing=True
  def __init__(self):
    # Create GStreamer pipeline
    self.pipeline = Gst.Pipeline()

    # Create bus to get events from GStreamer pipeline
    self.bus = self.pipeline.get_bus()
    self.bus.add_signal_watch()
    self.bus.connect('message::error', self.on_error)
    
    self.bus.enable_sync_message_emission()
    self.bus.connect('sync-message::element', self.on_sync_message)
    
    # Create GStreamer elements
    self.src = Gst.ElementFactory.make('autovideosrc', None)
    self.face = Gst.ElementFactory.make('facedetect', "face")
    self.convert = Gst.ElementFactory.make('videoconvert', None)
    self.sink = Gst.ElementFactory.make('fakesink', None)

    #video/x-raw, framerate=15/1, width=640, height=480 !  jpegenc ! multipartmux boundary=spionisto ! tcpclientsink port=8082 host=127.0.0.1

    # Add elements to the pipeline
    self.pipeline.add(self.src)
    self.pipeline.add(self.convert)
    self.pipeline.add(self.face)
    self.pipeline.add(self.sink)
    
    self.src.link(self.convert)
    self.convert.link(self.face)
    self.face.link(self.sink)

  def run(self):
    print("running")
    self.pipeline.set_state(Gst.State.PLAYING)
    try:
      while (self.playing == True):
        pass
    except KeyboardInterrupt:
      print("Shutdown facedetect server ...")

  def on_sync_message(self, bus, msg):
    print("Got message from: ", msg.src.name)
    t = msg.type
    if t == Gst.MessageType.EOS:
      self.player.set_state(Gst.State.NULL)
      self.playing = False
    elif t == Gst.MessageType.INFO:
      print("Got info message.")
    elif t == Gst.MessageType.ELEMENT:
      print("Got element message.")
      mstruct=Gst.Message.get_structure_get_value(msg, "faces")
      print(Gst.Structure.to_string(mstruct))
    else:
      print("Other")


  def on_error(self, bus, msg):
    print('on_error():', msg.parse_error())

if __name__ == '__main__':
  webcam = Webcam()
  webcam.run()
