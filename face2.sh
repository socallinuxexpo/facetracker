#!/bin/bash

gst-launch-1.0 -e -v -m \
  v4l2src device=/dev/video0 ! \
   video/x-raw, format=YUY2, width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! \
  videoconvert ! \
   video/x-raw, format=RGB, width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! \
  videoscale ! \
   video/x-raw, format=RGB, width=128, height=72, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! \
  facedetect display=true ! \
  videoconvert ! \
   video/x-raw, format=RGBA, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! \
  videoscale ! \
   video/x-raw,  width=1280, height=720, pixel-aspect-ratio=1/1, interlace-mode=progressive, framerate=10/1 ! \
  autovideosink


exit
