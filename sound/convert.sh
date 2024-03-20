#!/bin/bash

for file in mp3_files/*.mp3; do
    echo "Processing file: $file"
    file=$(basename "$file")
    file=${file%.*}
    ffmpeg -y -i mp3_files/$file.mp3 -ar 8000 -ac 1 temp.wav
    ffmpeg -y -i temp.wav -c:a libopencore_amrnb -b:a 1.8k amr_files/$file.amr
    rm temp.wav
done
