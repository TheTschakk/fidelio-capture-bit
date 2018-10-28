#!/bin/bash

filename=$1
EXT="${filename##*.}"
FILE="${filename%.*}"

echo $FILE

#ffmpeg -f rawvideo -pix_fmt gray -s 720x480 -i ${FILE}.bwv -c:v ffv1 -level 3 ${FILE}.mkv
#ffmpeg -f rawvideo -pix_fmt gray -s 720x480 -i ${FILE}.bwv -c:v libx264 -crf 0 -preset veryslow ${FILE}.mp4
ffmpeg -f rawvideo -pix_fmt gray -s 720x480 -i ${FILE}.bwv -c:v libx264 -pix_fmt yuv420p -crf 20 -preset veryslow ${FILE}.mp4
