gcc -fpic -c video_brightness.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_brightness.so video_brightness.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c clock_overlay.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o clock.so clock_overlay.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c timeoverlay.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o time.so timeoverlay.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c video_aspect_ratio.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_aspect_ratio.so video_aspect_ratio.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c videobox.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_videobox.so videobox.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c video_brightness.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_brightness.so video_brightness.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c video_contrast.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_contrast.so video_contrast.o `pkg-config --cflags --libs gstreamer-1.0`

gcc -fpic -c videocrop.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_videocrop.so videocrop.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c videoflip.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o flip.so videoflip.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c video_hue.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_hue.so video_hue.o `pkg-config --cflags --libs gstreamer-1.0`

gcc -fpic -c videorate.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o rate.so videorate.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c video_saturation.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_saturation.so video_saturation.o `pkg-config --cflags --libs gstreamer-1.0`
