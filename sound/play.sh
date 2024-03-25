#!/bin/bash

for file in amr_files/*.amr; do
    echo "Playing file: $file"
    ffplay -autoexit $file
done
