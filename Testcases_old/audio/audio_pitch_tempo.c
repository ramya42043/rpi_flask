#include<string.h>
#include<stdio.h>
#include<gst/gst.h>
#include<glib.h>
#include<stdlib.h>
#include<unistd.h>
static gfloat tempo;

typedef struct _CustomData {
		GstElement *pipeline,*source,*audiopitch,*decoder,*conv,*sink,*test,*conv1;

		GMainLoop *loop;
		GstBus *bus;
		guint bus_watch_id;
		gboolean playing;  /* Playing or Paused */
		gdouble rate;      /* Current playback rate (can be negative) */						
} CustomData;

CustomData data;
static gboolean
bus_call(GstBus *bus,GstMessage *msg,gpointer data)
{
		GMainLoop *loop=(GMainLoop *) data;
		switch (GST_MESSAGE_TYPE(msg))
		{
				case GST_MESSAGE_EOS:
						g_print("End of stream\n");
						g_main_loop_quit(loop);
						break;
				case GST_MESSAGE_ERROR:
						{
								gchar *debug;
								GError *error;
								gst_message_parse_error(msg,&error,&debug);
								g_free(debug);
								g_printerr("Error:%s\n",error->message);
								g_error_free(error);
								g_main_loop_quit(loop);
								break;
						}
				default:
						break;
		}
		return TRUE;
}
static void on_pad_added(GstElement *element, GstPad *pad , gpointer data)
{
		GstPad *sinkpad;
		GstElement *decoder=(GstElement *)data;

		sinkpad=gst_element_get_static_pad(decoder, "sink");

		gst_pad_link(pad,sinkpad);
		gst_object_unref(sinkpad);
}

static gint set_tempo(CustomData *data,gfloat i)
{
	gfloat ltempo;
	tempo = i;
	g_object_set(G_OBJECT(data->audiopitch),"tempo",tempo,NULL);
	g_object_get(G_OBJECT(data->audiopitch),"tempo",&ltempo,NULL);
	if(tempo==ltempo)
		return 1;
	else
		return 0;
}

static void disable_mainloop()
{
	g_main_loop_unref(data.loop);
	g_main_loop_quit(data.loop);

}


gint start()
{

		GstStateChangeReturn ret;

		/*initialization*/
		gst_init(0,NULL);
		memset (&data, 0, sizeof (data));
		data.loop = g_main_loop_new (NULL, FALSE);


		/*creating gstreamer elements*/

		data.pipeline=gst_pipeline_new("mp3player");
		data.source=gst_element_factory_make("filesrc","src");
		if(!data.source){
				g_print("Failed to create source\n");
				return -1;
		}
		data.decoder=gst_element_factory_make("decodebin","audiomp3decoder");
		if(!data.decoder){
				g_print("failed to create decoder\n");
				return -1;
		}

		data.audiopitch=gst_element_factory_make("pitch","characteristics");
		if(!data.audiopitch){
				g_print("failed to create pitch\n");
				return -1;
		}
		data.conv=gst_element_factory_make("audioconvert","audioconverter");
		if(!data.conv){
				g_print("Failed to create audio converter\n");
				return -1;
		}
		data.conv1=gst_element_factory_make("audioconvert","audioconverter1");
		if(!data.conv1){
				g_print("Failed to create audio converter1\n");
				return -1;
		}
		data.sink=gst_element_factory_make("autoaudiosink","audiosink");
		if(!data.sink){
				g_print("Failed to create audiosink\n");
				return -1;
		}

		if(!data.pipeline|| !data.source || !data.decoder || !data.audiopitch	 || !data.conv || !data.sink)
		{
				g_printerr("One element could not be created. Exiting\n");
				return -1;
		}
		/* setting up the pipeline*/
		g_object_set (G_OBJECT (data.source),"location","./song.mp3",NULL);
		/** adding message handler**/
		data.bus=gst_pipeline_get_bus(GST_PIPELINE(data.pipeline));
		
		data.bus_watch_id=gst_bus_add_watch(data.bus,bus_call,data.loop);
		gst_object_unref(data.bus);

		/** adding all elements to the pipeline**/
		gst_bin_add_many(GST_BIN(data.pipeline),data.source,data.decoder,data.audiopitch,data.conv,data.sink,data.conv1,NULL);

		/** Linking all the elements**/

		//gst_element_link(source,decoder);
		gst_element_link(data.source,data.decoder);
		gst_element_link(data.decoder,data.conv);
		gst_element_link(data.conv,data.audiopitch);
		gst_element_link(data.audiopitch,data.conv1);
		gst_element_link(data.conv1,data.sink);
		g_signal_connect(data.decoder, "pad-added", G_CALLBACK(on_pad_added),data.conv);   
		g_signal_connect(data.conv, "pad-added", G_CALLBACK(on_pad_added),data.audiopitch);
		g_signal_connect(data.audiopitch,"pad-added",G_CALLBACK(on_pad_added),data.conv1);
		g_signal_connect(data.conv1,"pad-added",G_CALLBACK(on_pad_added),data.sink);

		/**setting the pipeline for the playing**/
		
		ret=gst_element_set_state(data.pipeline,GST_STATE_PLAYING);
		if (ret == GST_STATE_CHANGE_FAILURE) {
				g_printerr ("Unable to set the pipeline to the playing state.\n");
				gst_object_unref (data.pipeline);
				return -1;
		}
		
		/* Create a GLib Main Loop and set it to run */
		data.loop = g_main_loop_new (NULL, FALSE);

		
		/**Iterate**/
		
		//g_object_set(data.audiopitch,"pitch",2.0,NULL);
		gint p;
		p=set_tempo(&data,2.0);
		
		alarm(10);
		signal(SIGALRM,disable_mainloop);


		g_main_loop_run(data.loop);

		/***removing****/
		
		gst_element_set_state(data.pipeline,GST_STATE_NULL);
		
		g_source_remove(data.bus_watch_id);

		gst_element_set_state (data.pipeline, GST_STATE_NULL);
		gst_object_unref (data.pipeline);
		return p;
}

