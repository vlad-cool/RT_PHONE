#!/bin/bash

for file in amr_files/*.amr; do
    file=$(basename "$file")
    file=${file%.*}
    echo "Processing file: $file"
    python3 send_file.py amr_files/$file.amr $file.amr
done
