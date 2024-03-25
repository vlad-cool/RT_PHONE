#!/bin/bash

for file in mp3_files/*.mp3; do
    echo "Processing file: $file"
    file=$(basename "$file")
    file=${file%.*}
    ffmpeg -y -i mp3_files/$file.mp3 -ar 8000 -ac 1 temp1.wav
    ffmpeg -y -i temp1.wav -af "silenceremove=start_periods=1:start_duration=0.1:start_threshold=-50dB:detection=peak,aformat=dblp,areverse,silenceremove=start_periods=1:start_duration=0.1:start_threshold=-50dB:detection=peak,aformat=dblp,areverse" temp2.wav
    ffmpeg -y -i temp2.wav -c:a libopencore_amrnb -b:a 1.8k amr_files/$file.amr
    rm temp1.wav temp2.wav
done
