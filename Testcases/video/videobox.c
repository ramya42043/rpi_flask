#include <string.h>
#include <stdio.h>
#include <gst/gst.h>
#include <unistd.h>
static gint left =0;
static gint right =0;
static gint bottom =0;
static gint top =0;
static gint fill=0,f2=0;
//static gdouble contr=1;
typedef struct _CustomData {
	GstElement *pipeline,*source,*decoder,*videoconv,*audioconv,*audiosink,*videosink,*resample,*videobox;

	GMainLoop *loop;
	GstBus *bus;
	guint bus_watch_id;
	gboolean playing;  /* Playing or Paused */
	gdouble rate;      /* Current playback rate (can be negative) */
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
	sinkpad = gst_element_get_static_pad (decoder, "sink");
	gst_pad_link (pad, sinkpad);
	gst_object_unref (sinkpad);
}

static int change_borders(CustomData *data,gint lleft,gint lright,gint ltop,gint lbottom,gint lfill)
{
	gint rleft,rright,rtop,rbottom,rfill;	
	left=lleft;
	right=lright;
	top=ltop;
	bottom=lbottom;
	fill=lfill;
	g_object_set(G_OBJECT(data->videobox),"left",left,"right",right,"top",top,"bottom",bottom,"fill",fill,NULL);
	g_object_get(data->videobox,"left",&rleft,"right",&rright,"top",&rtop,"bottom",&rbottom,"fill",&rfill,NULL);
	
	if((left == rleft) && (right == rright) && (top==rtop) && (bottom==rbottom) && (fill == rfill))
		return 1;
	else 
		return 0;

}

void disable_mainloop()
{
	g_main_loop_unref(data.loop);
	g_main_loop_quit(data.loop);

}


gint start()
{
	GstStateChangeReturn ret;
	GIOChannel *io_stdin;
	gst_init (NULL, NULL);
	memset (&data, 0, sizeof (data));
	data.loop = g_main_loop_new (NULL, FALSE);
	/* Create gstreamer elements */
	data.pipeline = gst_pipeline_new ("videobox");
	data.source = gst_element_factory_make ("filesrc", "src");

	if(!data.source){
		g_print("Failed to create source\n");
		return -1;
	}

	data.decoder = gst_element_factory_make ("decodebin", "video decoder");
	if(!data.decoder){
		g_print("Failed to create  video decoder\n");
		return -1;
	}
	data.videoconv = gst_element_factory_make ("videoconvert", "video conversion");
        if(!data.videoconv){
                g_print("Failed to create videoconverter\n");
                return -1;
        }
	
	data.videobox=gst_element_factory_make("videobox","videoboxresize");
	if(!data.videobox){
                g_print("Failed to create videobox\n");
                return -1;
        }


	data.videosink = gst_element_factory_make ("ximagesink", "video output");
	if(!data.videosink){
		g_print("Failed to create videosink\n");
		return -1;
	}

	data.audioconv = gst_element_factory_make ("audioconvert", "audio conversion");
	if(!data.audioconv){
		g_print("Failed to create audio conv\n");
		return -1;
	}

	data.resample = gst_element_factory_make ("audioresample", "audio sample");
        if(!data.resample){
                g_print("Failed to create audio sample\n");
                return -1;
        }

	
	data.audiosink = gst_element_factory_make ("autoaudiosink", "audio-output");
        if(!data.audiosink){
                g_print("Failed to create audio sink\n");
                return -1;
        }



	if (!data.pipeline || !data.source || !data.decoder || !data.videoconv || !data.videobox || !data.videosink || !data.audioconv || !data.resample || !data.audiosink) {
		g_printerr ("One element could not be created. Exiting.\n");
		return -1;
	}
	/* Set up the pipeline */
	/* we set the input filename to the source element */
	g_object_set (G_OBJECT (data.source), "location", "./video.webm", NULL);
	/* we add a message handler */
	data.bus = gst_pipeline_get_bus (GST_PIPELINE (data.pipeline));
	data.bus_watch_id=gst_bus_add_watch (data.bus, bus_call, data.loop);
	gst_object_unref (data.bus);
	/* we add all elements into the pipeline */
	gst_bin_add_many (GST_BIN (data.pipeline),
			data.source,data.decoder,data.audiosink,data.videobox,data.videosink,data.resample,data.audioconv,data.videoconv,NULL);
	/* we link the elements together */
	gst_element_link (data.source, data.decoder);
	gst_element_link (data.videoconv,data.videobox);
	gst_element_link (data.videobox,data.videosink);

        gst_element_link (data.audioconv, data.resample);
	gst_element_link (data.resample,data.audiosink);


	g_signal_connect (data.decoder, "pad-added", G_CALLBACK (on_pad_added), data.videoconv);

	g_signal_connect(data.decoder,"pad-added",G_CALLBACK(on_pad_added),data.audioconv);



	/* Set the pipeline to "playing" state*/

	ret=gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
	
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (data.pipeline);
		return -1;
	}
	/* Create a GLib Main Loop and set it to run */
	data.loop = g_main_loop_new (NULL, FALSE);
	gint p;
	gint i;
	for(i=0;i<=5;i++)
	{
	p = change_borders(&data,-100,-100,-100,-100,i);
	sleep(2);
	}

	alarm(30);
	signal(SIGALRM,disable_mainloop);
	g_main_loop_run (data.loop);
	gst_element_set_state (data.pipeline, GST_STATE_NULL);
	gst_object_unref (data.pipeline);
	
	return p;


	
} 






