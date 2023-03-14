#include "EC_GStreamerCodec.h"

#include "ECSDKTypes.h"
#include "Utils.h"


using namespace std;

EC_GStreamerCodec *EC_GStreamerCodec::pInstance = NULL;
std::once_flag EC_GStreamerCodec::m_flag;
// std::once_flag EC_GStreamerCodec::m_libload_flag;

//AVCodecID EC_FFmpegCodec::defaultCodecID = AV_CODEC_ID_H264;

typedef int  (*gst_handle_message)(GstBus *bus,GstMessage *	msg,gpointer data);

typedef struct
{
  const gint type;
    const gchar *name;
  gst_handle_message  handle;
  GQuark quark;
} GstMessageQuarks;

static int gst_handle_unknown(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}
static int gst_handle_eos(GstBus *bus,GstMessage *	msg,gpointer data)
{
    printf("### %s:%d\n", __FUNCTION__, __LINE__);
    EC_GST_STREAM *me = (EC_GST_STREAM *)(data);
    g_main_loop_quit(me->loop);
    return 0;

}

static int gst_handle_error(GstBus *bus,GstMessage *	msg,gpointer data)
{
    	gchar  *debug = NULL;
	    GError *error = NULL;
	    EC_GST_STREAM *me = (EC_GST_STREAM *)(data);

    	gst_message_parse_error(msg, &error, &debug);
		g_free(debug);

		printf("### %s:%d, Error: %s\n", __FUNCTION__, __LINE__,error->message);
		g_error_free(error);
		g_main_loop_quit(me->loop);

        return 0;
    
}

static int gst_handle_warning(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_info(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_tag(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_buffering(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_state_changed(GstBus *bus,GstMessage *	msg,gpointer data)
{
    gchar  *debug = NULL;
	GError *error = NULL;
	EC_GST_STREAM *me = (EC_GST_STREAM *)(data);

    if (GST_MESSAGE_SRC (msg) == GST_OBJECT (me->ec_pipeline)) {
		GstState old_state, new_state;
		gst_message_parse_state_changed (msg, &old_state, &new_state, NULL);
		printf("Pipeline changed state from %s to %s.\n",
                gst_element_state_get_name (old_state),
		        gst_element_state_get_name (new_state));
		if (new_state == GST_STATE_PLAYING) {
				// Assigned "Now GStreamer Playing"
				// if (me->frameworkHandle && me->frameworkHandle->platformDelegate.screenStreamPlayed_f != NULL) {
				// 	me->frameworkHandle->platformDelegate.screenStreamPlayed_f(
				// 			me->frameworkHandle->airPlaySession,
				// 			me,
				// 			me->frameworkHandle);
				// }
                printf("### %s:%d I am playing: %d\n", __FUNCTION__,__LINE__,me->ec_pipeline->current_state);

		}
	}  
    return 0;
}

static int gst_handle_state_dirty(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_step_done(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_clock_provide(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_clock_lost(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_new_clock(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_structure_change(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_stream_status(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_application(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}
static int gst_handle_element(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_segment_start(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_segment_done(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_duration_changed(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_latency(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_async_start(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_async_done(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_request_state(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_step_start(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_qos(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_progress(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_toc(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_reset_time(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_stream_start(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_need_context(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_have_context(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_device_added(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_device_removed(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_device_changed(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_property_notify(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_stream_collection(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_streams_selected(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_redirect(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}

static int gst_handle_instant_rate_request(GstBus *bus,GstMessage *	msg,gpointer data)
{
    return 0;
}



static GstMessageQuarks message_quarks[] = {
  {GST_MESSAGE_UNKNOWN, "unknown",gst_handle_unknown, 0},
  {GST_MESSAGE_EOS, "eos", gst_handle_eos,0},
  {GST_MESSAGE_ERROR, "error", gst_handle_error,0},
  {GST_MESSAGE_WARNING, "warning", gst_handle_warning,0},
  {GST_MESSAGE_INFO, "info", gst_handle_info,0},
  {GST_MESSAGE_TAG, "tag", gst_handle_tag,0},
  {GST_MESSAGE_BUFFERING, "buffering",gst_handle_buffering, 0},
  {GST_MESSAGE_STATE_CHANGED, "state-changed", gst_handle_state_changed,0},
  {GST_MESSAGE_STATE_DIRTY, "state-dirty", gst_handle_state_dirty,0},
  {GST_MESSAGE_STEP_DONE, "step-done", gst_handle_step_done,0},
  {GST_MESSAGE_CLOCK_PROVIDE, "clock-provide",gst_handle_clock_provide, 0},
  {GST_MESSAGE_CLOCK_LOST, "clock-lost", gst_handle_clock_lost,0},
  {GST_MESSAGE_NEW_CLOCK, "new-clock",gst_handle_new_clock, 0},
  {GST_MESSAGE_STRUCTURE_CHANGE, "structure-change", gst_handle_structure_change,0},
  {GST_MESSAGE_STREAM_STATUS, "stream-status",gst_handle_stream_status, 0},
  {GST_MESSAGE_APPLICATION, "application", gst_handle_application,0},
  {GST_MESSAGE_ELEMENT, "element", gst_handle_element,0},
  {GST_MESSAGE_SEGMENT_START, "segment-start",gst_handle_segment_start, 0},
  {GST_MESSAGE_SEGMENT_DONE, "segment-done", gst_handle_segment_done,0},
  {GST_MESSAGE_DURATION_CHANGED, "duration-changed", gst_handle_duration_changed,0},
  {GST_MESSAGE_LATENCY, "latency", gst_handle_latency,0},
  {GST_MESSAGE_ASYNC_START, "async-start", gst_handle_async_start,0},
  {GST_MESSAGE_ASYNC_DONE, "async-done", gst_handle_async_done,0},
  {GST_MESSAGE_REQUEST_STATE, "request-state", gst_handle_request_state,0},
  {GST_MESSAGE_STEP_START, "step-start", gst_handle_step_start,0},
  {GST_MESSAGE_QOS, "qos", gst_handle_qos,0},
  {GST_MESSAGE_PROGRESS, "progress",gst_handle_progress, 0},
  {GST_MESSAGE_TOC, "toc", gst_handle_toc,0},
  {GST_MESSAGE_RESET_TIME, "reset-time", gst_handle_reset_time,0},
  {GST_MESSAGE_STREAM_START, "stream-start", gst_handle_stream_start,0},
  {GST_MESSAGE_NEED_CONTEXT, "need-context", gst_handle_need_context,0},
  {GST_MESSAGE_HAVE_CONTEXT, "have-context", gst_handle_have_context,0},
  {GST_MESSAGE_DEVICE_ADDED, "device-added", gst_handle_device_added,0},
  {GST_MESSAGE_DEVICE_REMOVED, "device-removed", gst_handle_device_removed,0},
  {GST_MESSAGE_DEVICE_CHANGED, "device-changed",gst_handle_device_changed, 0},
  {GST_MESSAGE_PROPERTY_NOTIFY, "property-notify", gst_handle_property_notify,0},
  {GST_MESSAGE_STREAM_COLLECTION, "stream-collection", gst_handle_stream_collection,0},
  {GST_MESSAGE_STREAMS_SELECTED, "streams-selected", gst_handle_streams_selected,0},
  {GST_MESSAGE_REDIRECT, "redirect", gst_handle_redirect,0},
//   {GST_MESSAGE_INSTANT_RATE_REQUEST, "instant-rate-request", gst_handle_instant_rate_request,0},
  {GST_MESSAGE_ANY, NULL, NULL,0}
};


gboolean gst_bus_callback(GstBus *		bus,GstMessage *	msg, gpointer 		data)
{
    gboolean bRet = false;
	gchar  *debug = NULL;
	GError *error = NULL;
	EC_GST_STREAM *me = (EC_GST_STREAM *)(data);

	(void) bus;
    for (int i = 0; message_quarks[i].type != GST_MESSAGE_ANY; i++) {
        if(GST_MESSAGE_TYPE(msg) == message_quarks[i].type) {
            if(message_quarks[i].name)
            {
                printf("### %s: %s\n", __FUNCTION__,message_quarks[i].name);
            }
            if(NULL != message_quarks[i].handle){
                message_quarks[i].handle(bus,msg,data);
            }
            bRet = true;
            break;
        }
    }
	return bRet;
}


static FILE * appSrcFile = NULL;
static int  read_counter = 0;
static char read_buffer[4096];
int ss_size = 0;
int read_size = 0;

static void test_gst_appsrc_need_data_callback (GstElement *source, guint size, gpointer    user_data)
{
    GstBuffer *buffer;
    GstFlowReturn ret;
    // GstMapInfo map;

    printf("### %s:%d, callback %d\n", __FUNCTION__, __LINE__,size);
    for(;;)
    {
        if (appSrcFile == NULL)
        {
            appSrcFile = fopen("/run/media/sda1/carbit/000.h264", "rb");
        }
        fseek(appSrcFile,ss_size,SEEK_SET);
        read_size= fread(read_buffer, 1, size, appSrcFile);
        printf("### %s:%d,read_data() read_counter=%d, ssize = %d size=%d, %02X\n", __FUNCTION__, __LINE__,read_counter++, ss_size,size,read_buffer[5]);
        ss_size +=read_size;
        if(read_size == 0)
        {
            ret = gst_app_src_end_of_stream((GstAppSrc *)source);
            printf("### %s: %deos returned %d\n", __FUNCTION__, __LINE__,ret);
            fclose(appSrcFile);
            appSrcFile = NULL;
            break;
        }

        buffer = gst_buffer_new_wrapped((gpointer)((read_buffer)),(gsize)read_size);
        g_signal_emit_by_name (source, "push-buffer", buffer, &ret);
        printf("### %s:%d, emit\n", __FUNCTION__, __LINE__);
        //gst_buffer_unref (buffer);  
        // buffer = gst_buffer_new_allocate (NULL, ss_size, NULL);
//#if 0
        // gst_buffer_fill(buffer, 0, read_buffer, ss_size);
//#else
    //gst_buffer_map (buffer, &map, GST_MAP_WRITE);
    //memcpy( (guchar *)map.data, read_buffer, gst_buffer_get_size( buffer ) );
//#endif

        
    }
}

static void gst_appsrc_need_data_callback (GstElement *source, guint  size, EC_GST_STREAM *app)
{
    GstBuffer *buffer;
    GstFlowReturn ret;

    if (app->offset >= app->length) {
        /* we are EOS, send end-of-stream */
        g_signal_emit_by_name (app->ec_appsrc, "end-of-stream", &ret);
        return;
    }

    /* read the amount of data, we are allowed to return less if we are EOS */
    buffer = gst_buffer_new ();

    if (app->offset + size > app->length)
        size = app->length - app->offset;

    gst_buffer_append_memory (buffer,
        gst_memory_new_wrapped (GST_MEMORY_FLAG_READONLY,
            app->data, app->length, app->offset, size, NULL, NULL));

    /* we need to set an offset for random access */
    GST_BUFFER_OFFSET (buffer) = app->offset;
    GST_BUFFER_OFFSET_END (buffer) = app->offset + size;

    printf ("###%s:%d,feed buffer %p, offset %" G_GUINT64_FORMAT "-%u", __FUNCTION__,__LINE__,buffer,
        app->offset, size);
    g_signal_emit_by_name (app->ec_appsrc, "push-buffer", buffer, &ret);
    gst_buffer_unref (buffer);

    app->offset += size;

    return;
}

static void gst_appsrc_enough_data_callback (GstElement * appsrc,EC_GST_STREAM *app)
{
    GstBuffer *buffer;
    GstFlowReturn ret;
    GstMapInfo map;
    printf("### %s: %d enough-data is called \n", __FUNCTION__, __LINE__);
    return;
}

static gboolean gst_appsrc_seek_data_callback (GstElement * appsrc,guint64 position,EC_GST_STREAM *app){

    GST_DEBUG ("seek to offset %" G_GUINT64_FORMAT, position);
    app->offset = position;
    printf("### %s: %d seek-data is called \n", __FUNCTION__, __LINE__);
    return true;
}

int EC_GStreamerCodec::test_appsrc_by_file() {
    // Start playing
    GstState state;
    GstState next;
    GstStateChangeReturn stateChanged = gst_element_set_state(ec_gst_stream.ec_pipeline, GST_STATE_PLAYING);
    if (stateChanged == GST_STATE_CHANGE_FAILURE) {
        printf ("Unable to set the pipeline to the playing state.\n");
    }

    FILE *pFile = fopen("/run/media/sda1/carbit/0.h264", "rb");
    void *data = (void *)malloc(65536);
    int32_t len = fread(data,1,65536,pFile);
    fclose(pFile);
    ec_gst_stream.data = (guint8 *)data;
    ec_gst_stream.length = len;
    ec_gst_stream.offset = 0;
//     // GstBuffer *buf = gst_buffer_new_wrapped((gpointer)(data),(gsize)len);
    GstBuffer *buf = gst_buffer_new_allocate (NULL, len, NULL);
    gst_buffer_fill(buf, 0, data, len);
    GstFlowReturn ret;
    g_signal_emit_by_name (ec_gst_stream.ec_appsrc, "push-buffer", buf, &ret);
    //gst_app_src_push_buffer (ec_appsrc,  buf);
    if(GST_FLOW_OK == ret)
    {
        printf("--------------------------send ok 1------------\n");
        gst_buffer_unref(buf);
    }
    gst_app_src_end_of_stream((GstAppSrc *)ec_gst_stream.ec_appsrc);
    free(data);

    g_main_loop_run (ec_gst_stream.loop);

    printf("### %s: %deos returned\n", __FUNCTION__, __LINE__);

    pFile = fopen("/run/media/sda1/carbit/1.h264", "rb");
    data = (void *)malloc(65536);
    len = fread(data,1,65536,pFile);
    fclose(pFile);
    ec_gst_stream.data = (guint8 *)data;
    ec_gst_stream.length = len;
    ec_gst_stream.offset = 0;
    buf = gst_buffer_new_allocate (NULL, len, NULL);
    gst_buffer_fill(buf, 0, data, len);
    g_signal_emit_by_name (ec_gst_stream.ec_appsrc, "push-buffer", buf, &ret);
    //gst_app_src_push_buffer (ec_appsrc,  buf);
    if(GST_FLOW_OK == ret)
    {
        printf("--------------------------send ok 2------------\n");
        gst_buffer_unref(buf);
    }
    gst_app_src_end_of_stream((GstAppSrc *)ec_gst_stream.ec_appsrc);
    free(data);
    
    printf("### %s: %deos returned\n", __FUNCTION__, __LINE__);
   
    g_main_loop_run (ec_gst_stream.loop);

    return 0;

 }

int EC_GStreamerCodec::gst_create_gstreamer_video_pipe()
{

    int ret = 0;
    memset(&ec_gst_stream,0x00,sizeof(ec_gst_stream));
    //The following is to test appsrc
    gchar* pipeline_description = g_strdup_printf("appsrc name=ecappsrc !"
        " h264parse ! "
        " v4l2h264dec  ! video/x-raw,format=NV12 ! waylandsink name=ecsink sync=false surfaceid=1 overlay-width=800");

    //The following is to test filesrc
//    gchar* pipeline_description = g_strdup_printf("filesrc name=ecappsrc !"
//         " video/x-h264, width=800, height=480, framerate=60/1 ! h264parse name=ecparse! "
//         " v4l2h264dec name=ecdecoder! video/x-raw,format=NV12 ! waylandsink name=ecsink");

    if (pipeline_description == NULL)
    {
        return -1;
    }
    printf("pipeline_description is %s\n",pipeline_description);
    ec_gst_stream.ec_pipeline = gst_parse_launch(pipeline_description, NULL);
    g_free (pipeline_description);

    if (NULL == ec_gst_stream.ec_pipeline)
    {
        return -1;
    }

    ec_gst_stream.ec_appsrc = gst_bin_get_by_name (GST_BIN (ec_gst_stream.ec_pipeline), "ecappsrc");
    ec_gst_stream.ec_videosink = gst_bin_get_by_name (GST_BIN (ec_gst_stream.ec_pipeline), "ecsink");


    if(NULL == ec_gst_stream.ec_appsrc || NULL ==  ec_gst_stream.ec_videosink )
    {
        printf("some elements are NOT allocated\n");
        return -1;
    }
   
    // 2. Set to watch pipeline event
	ec_gst_stream.ec_bus = gst_pipeline_get_bus(GST_PIPELINE(ec_gst_stream.ec_pipeline));
	ec_gst_stream.ec_bus_watch_id = gst_bus_add_watch(ec_gst_stream.ec_bus, gst_bus_callback, &ec_gst_stream);

    ec_gst_stream.loop = g_main_loop_new (NULL, FALSE);
	
    return ret;
}

EC_GStreamerCodec::EC_GStreamerCodec()
{
    gst_init(NULL, NULL);
    gst_create_gstreamer_video_pipe();
    printf("### %s:%d I am initializing ......\n", __FUNCTION__, __LINE__); 
    /* setup appsrc */
    // This function is to set "size" param as callback
    // g_object_set (ec_gst_stream.ec_appsrc, "blocksize", 8192, NULL);
    // g_object_set (ec_gst_stream.ec_appsrc, "size", (gint64) ec_gst_stream.length, NULL);
    // g_object_set(     ec_gst_stream.ec_appsrc, "stream-type", GST_APP_STREAM_TYPE_STREAM, NULL );

    // g_signal_connect (ec_gst_stream.ec_appsrc, "need-data",   G_CALLBACK (test_gst_appsrc_need_data_callback), NULL);
    //g_signal_connect (ec_gst_stream.ec_appsrc, "need-data",   G_CALLBACK (gst_appsrc_need_data_callback), &ec_gst_stream);
    printf("### %s: %d need-data is setup-callback \n", __FUNCTION__, __LINE__);
    g_signal_connect (ec_gst_stream.ec_appsrc, "enough-data",   G_CALLBACK (gst_appsrc_enough_data_callback), &ec_gst_stream);
    printf("### %s: %d enough-data is setup-callback \n", __FUNCTION__, __LINE__);
    g_signal_connect (ec_gst_stream.ec_appsrc, "seek-data",   G_CALLBACK (gst_appsrc_seek_data_callback), &ec_gst_stream);
    printf("### %s: %d seek-data is setup-callback \n", __FUNCTION__, __LINE__);
    g_object_set( ec_gst_stream.ec_appsrc, "stream-type", GST_APP_STREAM_TYPE_STREAM, NULL );

    // test_appsrc_by_file();
}



EC_GStreamerCodec* EC_GStreamerCodec::getInstance()
{
    //若实例不存在，则尝试创建实例对象
    if (NULL == pInstance)
    {
        //call_once object makes sure calling createInstance function only one time;
         //it will be safe without lock;
        try 
        {
            std::call_once(m_flag, createInstance);
            // Register all formats and codecs.
            // This step may move to main function
            //in higher version, this step may be ignored. This function is Deprecated.
            //std::call_once(m_libload_flag,av_register_all);
        }
        catch (...) 
        {
            //std::cout << "CreateInstance error\n";
        }

    }
        //实例已经存在，直接该实例对象
    return pInstance;
}


void EC_GStreamerCodec::start(int width, int height)
{
    
    //todo 初始化解码器
    printf("### %s:%d start\n", __FUNCTION__, __LINE__); 
    GstStateChangeReturn stateChanged = gst_element_set_state(ec_gst_stream.ec_pipeline, GST_STATE_PLAYING);
    if (stateChanged == GST_STATE_CHANGE_FAILURE) {
        printf ("Unable to set the pipeline to the playing state.\n");
    }

}

void EC_GStreamerCodec::play(const void *data, int32_t len)
{
    // todo 视频数据传输给解码器
    //The following code to dump H264 to file to check 
    static int iFile = 0;
    char filename[100];
    memset(filename,sizeof(filename),0x00);
    snprintf(filename,sizeof(filename),"%s%d%s","/run/media/sda1/carbit/",iFile,".h264");
    printf("I am outputting logfile %s\n",filename);
    iFile++;
    logFile = fopen(filename, "wb");
    fwrite(data, 1, len, logFile);
    fflush(logFile);
    fclose(logFile);

    printf("### %s:%d play\n", __FUNCTION__, __LINE__); 
    ec_gst_stream.firstStream = true;
    //For this version gstream, it is NOT available
    //GstBuffer *buf = gst_buffer_new_memdup((gconstpointer)data,(gsize)len);
    //This function is not adoptable, because data is NOT created by us.
    //GstBuffer *buf = gst_buffer_new_wrapped((gpointer)((data)),(gsize)len);
    GstBuffer *buf = gst_buffer_new_allocate (NULL, len, NULL);
    gst_buffer_fill(buf, 0, data, len);
    GstFlowReturn ret;
    g_signal_emit_by_name (ec_gst_stream.ec_appsrc, "push-buffer", buf, &ret);
    //You can the following function, not signal_emit
    //gst_app_src_push_buffer (ec_appsrc,  buf);
    if(GST_FLOW_OK == ret)
    {
        printf("### %s:%d g_signal_emit_by_name SUCCEED\n", __FUNCTION__, __LINE__);  
    }
    else
    {
        printf("### %s:%d g_signal_emit_by_name FAILED\n", __FUNCTION__, __LINE__);
    }

    gst_buffer_unref(buf);
    /*
    当应用程序完成推送数据到appsrc，其应该调用gst_app_src_end_of_stream函数，或者发送end-of-stream响应信号。
    在调用该函数后，不应该再由缓存被推送到appsrc，直到立即定位发生或者是appsrc切换到了REAY状态。*/
    gst_app_src_end_of_stream((GstAppSrc *)ec_gst_stream.ec_appsrc);

    printf("### %s: %d eos returned\n", __FUNCTION__, __LINE__);

    //This is endless loop, you must to call quit to exit 
    g_main_loop_run (ec_gst_stream.loop);
  
}

void EC_GStreamerCodec::stop()
{
	gboolean				retErr = false;
	GstStateChangeReturn	gstErr = GST_STATE_CHANGE_FAILURE;

    printf("### %s:%d EC_GStreamerCodec::stop\n", __FUNCTION__, __LINE__);  
    if (NULL != ec_gst_stream.ec_pipeline) {
		gstErr = gst_element_set_state(ec_gst_stream.ec_pipeline, GST_STATE_NULL);
        if(GST_STATE_CHANGE_FAILURE == gstErr)
        {
            printf("stop-pipeline failed\n");
        }
		gst_object_unref(ec_gst_stream.ec_pipeline);
		ec_gst_stream.ec_pipeline = NULL;
	}

	if (NULL != ec_gst_stream.loop) {
        
		g_main_loop_unref(ec_gst_stream.loop);
		ec_gst_stream.loop = NULL;
	}

	if (NULL != ec_gst_stream.ec_appsrc_caps){
	    gst_caps_unref(ec_gst_stream.ec_appsrc_caps);
    }
	
    if (NULL != ec_gst_stream.ec_bus){
        gst_object_unref(ec_gst_stream.ec_bus);
    }

	if (0 != ec_gst_stream.ec_bus_watch_id) {
		retErr = g_source_remove(ec_gst_stream.ec_bus_watch_id);
        if(TRUE == retErr)
        {
            printf("stop-remove source failed \n");
        }
		ec_gst_stream.ec_bus_watch_id =  0;
	}

	// if (signaled == false) {
	// 	dispatch_semaphore_signal(me->doneSem);
	// }

	// pthread_exit(NULL);
    
}


GStreamerCodec::~GStreamerCodec()
{
    printf("### %s:%d GStreamerCodec::~GStreamerCodec\n", __FUNCTION__, __LINE__);  
    if (pInstance != NULL)
    {
        delete pInstance;
        pInstance = NULL;
    }
    // gst_object_unref(ec_gst_stream.ec_pipeline);

}

void GStreamerCodec::createInstance()
{
    pInstance = new(std::nothrow) GStreamerCodec();//分配失败，是返回NULL;
    if (NULL == pInstance)
    {
        throw std::exception();
    }
}




