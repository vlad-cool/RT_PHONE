#!/bin/bash

for file in mp3_files/b_*.mp3; do
    echo "Processing file: $file"
    file=$(basename "$file")
    file=${file%.*}
    ffplay -autoexit mp3_files/$file.mp3
    read name
    mv mp3_files/$file.mp3 mp3_files/$name.mp3
done
