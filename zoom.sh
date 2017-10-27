DURATION=30  #seconds
FRAMES_PER_SECOND=30
FILENAME=`date '+%Y-%m-%d_%H:%M:%S'`
WIDTH=1920
HEIGHT=1080
RESOLUTION=${WIDTH}x$HEIGHT

mkdir -p frames &&
    make main &&
    rm -f frames/*.ppm && 
    time ./main 1920 1080 $DURATION $FRAMES_PER_SECOND &&
    cd frames &&
    
    time ffmpeg -framerate $FRAMES_PER_SECOND \
	 -i frame-%d.ppm    \
	 -s:v $RESOLUTION   \
	 -c:v libx264       \
	 -pix_fmt yuv420p   \
	 -profile:v high444 \
	 -crf 20 $FILENAME.mp4 &&

    open $FILENAME.mp4
