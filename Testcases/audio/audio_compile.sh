gcc -fpic -c amplification_audioamplify.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o amplification.so amplification_audioamplify.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audioclippingmethod.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o clipping.so audioclippingmethod.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audioinvert.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o invert.so audioinvert.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audiopanorama_left.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_panorama_left.so audiopanorama_left.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audiopanorama_right.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_panorama_right.so audiopanorama_right.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audio_pitch.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_pitch.so audio_pitch.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audio_pitch_rate.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_pitch_rate.so audio_pitch_rate.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audio_pitch_tempo.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_pitch_tempo.so audio_pitch_tempo.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c audio_volume.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o volume.so audio_volume.o `pkg-config --cflags --libs gstreamer-1.0`


gcc -fpic -c wma_format.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_wma.so wma_format.o `pkg-config --cflags --libs gstreamer-1.0`

gcc -fpic -c ra_format.c `pkg-config --cflags --libs gstreamer-1.0`
gcc -shared -o test_ra.so ra_format.o `pkg-config --cflags --libs gstreamer-1.0`
