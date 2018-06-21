#include <string.h>
#include <stdio.h>
#include <gst/gst.h>
#include<stdlib.h>
#include<glib.h>
#include<unistd.h>
//enum 
gchar *text_show;
gint shade_background;
guint shade_value;
gint hval,vval;
gint autoresize;
guint color;

typedef struct _CustomData {
	GstElement *pipeline,*source,*decoder,*time,*videoconv,*videosink;

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
	//g_print ("Dynamic pad created, linking demuxer/decoder\n");
	sinkpad = gst_element_get_static_pad (decoder, "sink");
	gst_pad_link (pad, sinkpad);
	gst_object_unref (sinkpad);
}

static int show_Time(CustomData *data,const char* text,gint flag,guint shade_val,gint horizont,gint vertical,gchar *font,gint resize,guint clr)
{
	shade_background = flag;
	shade_value = shade_val;
	hval = horizont;
	vval= vertical;
	autoresize = resize;
	color=clr;
	g_object_set(G_OBJECT(data->time),"text",text,"shaded-background",shade_background,"shading-value",shade_value,"halignment",hval,"valignment",vval,"font-desc",font,"auto-resize",autoresize,"color",color,NULL);
	gchar *ltext;
	gint lflag;
	guint lshade_val;
	gint lhal,lver;
	gchar *lfont;
	gint lautoresize;
	guint lclr;
	g_object_get(data->time,"text",&ltext,"shaded-background",&lflag,"shading-value",&lshade_val,"halignment",&lhal,"valignment",&lver,"font-desc",&lfont,"auto-resize",&lautoresize,"color",&lclr,NULL);
	
	if((shade_background==lflag)&&(shade_value==lshade_val)&&(hval==lhal)&&(vval==lver)&&(autoresize==lautoresize)&&(color==lclr))
		return 1;
	else 
		return 0;
		
}

void disable_mainloop()
{
	g_main_loop_unref(data.loop);
	g_main_loop_quit(data.loop);
	gst_element_set_state (data.pipeline, GST_STATE_NULL);
	gst_object_unref (data.pipeline);
	

}

gint start()
{

	GstStateChangeReturn ret;
	GIOChannel *io_stdin;
	gst_init (NULL, NULL);
	memset (&data, 0, sizeof (data));
	data.loop = g_main_loop_new (NULL, FALSE);

	/* Create gstreamer elements */
	data.pipeline = gst_pipeline_new ("video_flip");
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
	
	data.time=gst_element_factory_make("timeoverlay","display video running time");
	if(!data.time){
                g_print("Failed to create clockoverlay\n");
                return -1;
        }


	data.videosink = gst_element_factory_make ("ximagesink", "video output");
	if(!data.videosink){
		g_print("Failed to create videosink\n");
		return -1;
	}

	if (!data.pipeline || !data.source || !data.decoder || !data.time || !data.videoconv || !data.videosink ) {
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
			data.source,data.decoder,data.time,data.videoconv,data.videosink,NULL);
	/* we link the elements together */
	gst_element_link (data.source, data.decoder);
	
//	gst_element_link (data.decoder,data.videoconv);
	gst_element_link (data.videoconv,data.time);
	gst_element_link (data.time,data.videosink);

	
	g_signal_connect (data.decoder, "pad-added", G_CALLBACK (on_pad_added), data.videoconv);
//	g_signal_connect (data.videoconv, "pad-added", G_CALLBACK (on_pad_added), data.time);
//	g_signal_connect(data.time, "pad-added", G_CALLBACK(on_pad_added), data.videosink);   
	
	/* Set the pipeline to "playing" state*/

	ret=gst_element_set_state (data.pipeline, GST_STATE_PLAYING);
	
	if (ret == GST_STATE_CHANGE_FAILURE) {
		g_printerr ("Unable to set the pipeline to the playing state.\n");
		gst_object_unref (data.pipeline);
		return -1;
	}
	gint p;
	/* Create a GLib Main Loop and set it to run */
	data.loop = g_main_loop_new (NULL, FALSE);
	
	p = show_Time(&data,"Video running time is:",1,250,0,3,"Garuda,20",1,4294967295);
	
	alarm(30);
	signal(SIGALRM,disable_mainloop);

	g_main_loop_run (data.loop);
	return p;


	
} 






