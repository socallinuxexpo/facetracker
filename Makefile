PROGRAM=facetrack videotrack
COMMON=message_handler.o
GSTREAMER=`pkg-config --cflags --libs gstreamer-1.0`

all: $(PROGRAM)

%.o: %.c main.h
	gcc -Wall -c -o $@ $< $(GSTREAMER)

facetrack: facetrack.c $(COMMON)
	gcc -Wall facetrack.c $(COMMON) -o facetrack $(GSTREAMER)

videotrack: videotrack.c $(COMMON)
	gcc -Wall videotrack.c $(COMMON) -o videotrack $(GSTREAMER)

clean:
	rm -f *.o *.ts
	rm -f $(PROGRAM)

