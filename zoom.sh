DURATION=25  #seconds
FRAMES_PER_SECOND=30
FILENAME=`date '+%Y-%m-%d_%H:%M:%S'`
WIDTH=3840
HEIGHT=2160
RESOLUTION=${WIDTH}x$HEIGHT

# Cleanup the frames directory.
mkdir -p frames &&
    mkdir -p videos &&
    make main &&
    rm -f frames/*.ppm

# Process alternating frames in parallel and wait for all to complete.
./main $WIDTH $HEIGHT $DURATION $FRAMES_PER_SECOND 0 4 & P1=$!
./main $WIDTH $HEIGHT $DURATION $FRAMES_PER_SECOND 1 4 & P2=$!
./main $WIDTH $HEIGHT $DURATION $FRAMES_PER_SECOND 2 4 & P3=$!
./main $WIDTH $HEIGHT $DURATION $FRAMES_PER_SECOND 3 4 & P4=$!
wait $P1 $P2 $P3 $P4

# Even if we only render some of the frames, stitch the video together.
cd frames &&
    time ffmpeg -framerate $FRAMES_PER_SECOND \
         -i frame-%d.ppm    \
         -s:v $RESOLUTION   \
         -c:v libx264       \
         -pix_fmt yuv420p   \
         -profile:v high444 \
         -crf 20 ../videos/$FILENAME.mp4 &&
    cd ../videos &&
    open $FILENAME.mp4
