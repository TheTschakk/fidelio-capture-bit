#!/bin/bash

for VID in $@; do

    filename=$VID
    EXT="${filename##*.}"
    FILE="${filename%.*}"

    echo $FILE

    ffmpeg -r 1 -f rawvideo -pix_fmt gray -s 720x480 -i ${FILE}.bwv -r 1 -c:v pam -f image2pipe pipe:1 |
    convert - -evaluate-sequence max \
	   -background "white" label:"${FILE%.*}" -gravity center -append ${FILE}.png
done

#NUM=$(ls -1 *png | wc -l)
#DIM=$(bc <<< "scale=0; sqrt($NUM-1)+1")

rm "summary.png"
montage "*png" -geometry "300x200+2+2" "summary.png"
