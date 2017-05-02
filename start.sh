#! /bin/bash

case "$1" in
	-r) gst-launch-1.0 -v udpsrc port=5000 caps="application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264,payload=(int)96" ! rtpjitterbuffer ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink ;;
	-s) gst-launch-1.0 -v v4l2src ! queue ! videorate ! videoconvert ! 'video/x-raw,width=(int)640,height=(int)480,framerate=(fraction)20/1' ! x264enc ! "video/x-h264,stream-format=byte-stream" ! rtph264pay ! udpsink host=127.0.0.1 port=5000 ;;
    -sw) gst-launch-1.0 -v v4l2src ! queue ! videorate ! videoconvert ! 'video/x-raw,width=(int)640,height=(int)480,framerate=(fraction)20/1' ! x264enc ! "video/x-h264,stream-format=byte-stream" ! watch ! rtph264pay ! udpsink host=127.0.0.1 port=5000 ;;
    -rw) gst-launch-1.0 -v udpsrc port=5000 caps="application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264,payload=(int)96" ! rtpjitterbuffer ! rtph264depay ! watch ! avdec_h264 ! videoconvert ! autovideosink ;;
	-e) gst-launch-1.0 -v v4l2src ! queue ! videorate ! videoconvert ! 'video/x-raw,width=(int)640,height=(int)480,framerate=(fraction)20/1' ! x264enc ! "video/x-h264,stream-format=byte-stream" ! encrypt ! rtph264pay ! udpsink host=127.0.0.1 port=5000 ;;
	-d) gst-launch-1.0 -v udpsrc port=5000 caps="application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264,payload=(int)96" ! rtpjitterbuffer ! rtph264depay ! decrypt ! avdec_h264 ! videoconvert ! autovideosink ;;
	*) echo "failed"
	    exit 1
	    ;;
esac
exit $?
