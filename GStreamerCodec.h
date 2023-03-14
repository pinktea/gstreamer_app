#ifndef _GStreamerCodec_H
#define _GStreamerCodec_H

#include <string>
#include <mutex>
#include <iostream>
#include <string>

extern "C" {
#include <glib.h>
#include <gst/gst.h>
#include <gst/base/base.h>
#include <gst/gstbuffer.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/gstmessage.h>
}

using namespace std;

typedef struct _ec_gst_stream_t {
	// CFDataRef							avccData;				// AVCC data for the stream.
    GstElement *ec_pipeline;
    GstElement *ec_appsrc;
    GstPad *ec_appsrcpad;
    GstElement *ec_h264parse;
    GstElement *ec_decoder;
    GstElement *ec_capsfilter;
    GstElement *ec_videosink;
    GstCaps *ec_appsrc_caps;
    GstState ec_state;  
    GstBus *ec_bus;
    guint ec_bus_watch_id;
    GMainLoop *loop;
	// struct CarPlayAppFrameworkPrivate *	frameworkHandle;
	// int32_t								deviceAvailable;

    uint32_t							widthPixels;			// Width of the screen in pixels.
	uint32_t							heightPixels;			// Height of the screen in pixels.
	uint32_t							platformLayer;
    uint8_t *							annexBHeader;        
	size_t								firstStreamLen;

    uint8_t *							annexBHeaderPtr; //If this comes from Apple avCC, you need to transfer from avCC to AnnexB by appending NALU
	size_t								annexBHeaderLen;
	size_t								nalSize;
	bool							    firstStream;
	uint8_t *							convBuffer;
	size_t								convBufferLen;
	FILE*								outputFile;


	// pthread_t 							screenThread;
	// dispatch_semaphore_t				doneSem;
    guint8 *data;
    gsize length;
    guint64 offset;
}EC_GST_STREAM;

class GStreamerCodec
{
public:
    GStreamerCodec(/* args */);
    static GStreamerCodec* getInstance();
    int gst_create_gstreamer_video_pipe();
    void start(int width, int height);
    void play(const void *data, int32_t len);
    void stop();
    ~GStreamerCodec();


private:
    static void createInstance();
    int test_appsrc_by_file();

    
private:
    //self data member
    static GStreamerCodec *pInstance;
    static std::once_flag m_flag;

    EC_GST_STREAM ec_gst_stream;
    FILE *logFile;

};

#endif //_GStreamerCodec_H