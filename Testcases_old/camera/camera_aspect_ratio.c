#include <string.h>
#include <stdio.h>
#include <gst/gst.h>
#include<stdlib.h>
#include<unistd.h>
char ratio[10];
typedef struct _CustomData {
	GstElement *pipeline,*source,*videoconv,*mux,*videosink,*rate,*enc;

	GMainLoop *loop;
	GstBus *bus;
	guint bus_watch_id;
	gboolean playing;  /* Playing or Paused */
	//gdouble rate;      /* Current playback rate (can be negative) */
} CustomData;

CustomData data;
static gboolean
bus_call (GstBus *bus,GstMessage *msg,gpointer data)

{
	GMainLoop *loop = (GMainLoop *) data;
	switch (GST_MESSAGE_TYPE (msg)) {
		case GST_MESSAGE_EOS:
			g_print ("End of stream\n");
			g_main_loop_quit (loop);
			break;
		case GST_MESSAGE_ERROR: {
						gchar *debug;
						GError *error;
						gst_message_parse_error (msg, &error, &debug);
						g_free (debug);
						g_printerr ("Error: %s\n", error->message);
						g_error_free (error);
						g_main_loop_quit (loop);
						break;
					}
		default:
					break;
	}
	return TRUE;
}

static void on_pad_added (GstElement *element,GstPad *pad,gpointer data)
{
	GstPad *sinkpad;

	GstElement *decoder = (GstElement *) data;


	/* We can now link this pad with the vorbis-decoder sink pad */
	//g_print ("Dynamic pad created, linking demuxer/decoder\n");
	sinkpad = gst_element_get_static_pad (decoder, "sink");
	gst_pad_link (pad, sinkpad);
	gst_object_unref (sinkpad);
}



static int change_aspectratio(CustomData *data,char *i)
{
	
	char *lratio;
	strcpy(ratio,i);
	//g_print("%s\n",ratio);
	g_object_set(G_OBJECT(data->source),"pixel-aspect-ratio",ratio,NULL);
	g_object_get(G_OBJECT(data->source),"pixel-aspect-ratio",&lratio,NULL);
	if(strcmp(ratio,lratio)==0)
		return 1;
	else
		return 0;

}

static void disable_mainloop()
{
	g_main_loop_unref (data.loop);
	g_main_loop_quit (data.loop);

}

/* Process keyboard input */

int start()
//int main()
{
	GstStateChangeReturn ret;
	GIOChannel *io_stdin;
	gst_init (0, NULL	);
	memset (&data, 0, sizeof (data));
	data.loop = g_main_loop_new (NULL, FALSE);

	/* Create gstreamer elements */
	data.pipeline = gst_pipeline_new ("videoplayer");
	data.source = gst_element_factory_make ("v4l2src", "src");

	if(!data.source){
		g_print("Failed to create source\n");
		return -1;
	}

	
	data.enc=gst_element_factory_make("theoraenc","enc");
	if(!data.enc){
                g_print("Failed to create enc\n");
                return -1;
        }

	data.rate=gst_element_factory_make("videorate","rate");
	if(!data.rate){
                g_print("Failed to create rate\n");
                return -1;
        }

	data.videoconv = gst_element_factory_make ("videoconvert", "video conversion");
        if(!data.videoconv){
                g_print("Failed to create videoconverter\n");
                return -1;
        }

	data.mux = gst_element_factory_make ("oggmux", "mux");
        if(!data.mux){
                g_print("Failed to create oggmux\n");
                return -1;
        }
	data.videosink = gst_element_factory_make ("filesink", "sink");
        if(!data.videoconv){
                g_print("Failed to create filesink\n");
                return -1;
        }
	if (!data.pipeline || !data.source || !data.enc || !data.videoconv || !data.rate || !data.videosink || !data.mux) {

		g_printerr ("One element could not be created. Exiting.\n");
		return -1;
	}
	/* Set up the pipeline */
	/* we set the input filename to the source element */
	
	/* we add a message handler */
	data.bus = gst_pipeline_get_bus (GST_PIPELINE (data.pipeline));
	
	data.bus_watch_id=gst_bus_add_watch (data.bus, bus_call, data.loop);
	gst_object_unref (data.bus);
	g_object_set(data.videosink,"location","./sample.ogg",NULL);
	/* we add all elements into the pipeline */
	gst_bin_add_many (GST_BIN (data.pipeline),
			data.source,data.enc,data.rate,data.videosink,data.videoconv,data.mux,NULL);
	/* we link the elements together */
	gst_element_link (data.source, data.videoconv);
	gst_element_link (data.videoconv,data.rate);
	gst_element_link (data.rate,data.enc);
	gst_element_link (data.enc,data.mux);
	gst_element_link (data.mux,data.videosink);

	//g_signal_connect (data.decoder, "pad-added", G_CALLBACK (on_pad_added), data.videoconv);
	g_signal_connect (data.videoconv, "pad-added", G_CALLBACK (on_pad_added), data.rate);
	g_signal_connect (data.rate, "pad-added", G_CALLBACK (on_pad_added), data.enc);
	g_signal_connect(data.enc, "pad-added", G_CALLBACK(on_pad_added), data.mux);   
	g_signal_connect (data.mux, "pad-added", G_CALLBACK (on_pad_added), data.videosink);

	gint p;
	p=change_aspectratio(&data,"1/1");

	/* Set the pipeline to "playing" state*/
	ret=gst_element_set_state (data.pipeline, GST_STATE_PLAYING);

	
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (data.pipeline);
		return -1;
	}

	/* Create a GLib Main Loop and set it to run */
	data.loop = g_main_loop_new (NULL, FALSE);
	alarm(15);
	signal(SIGALRM,disable_mainloop);
	g_main_loop_run (data.loop);
	
		
	


	/* Free resources */
	//g_main_loop_unref (data.loop);

	gst_element_set_state (data.pipeline, GST_STATE_NULL);
	gst_object_unref (data.pipeline);
	//g_print("%d\n",p);
	return p;


	
} 






