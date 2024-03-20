#!/bin/python3
import sys

src = ""
dest = ""

if len(sys.argv) < 2:
    print("No input file")
    exit(-1)
elif len(sys.argv) == 2:
    src = sys.argv[1]
    dest = sys.argv[1]
elif len(sys.argv) > 2:
    src = sys.argv[1]
    dest = sys.argv[2]

import serial
import select
import time

def is_input_available():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

ser = serial.Serial('/dev/ttyUSB0', 38400, timeout=1)

with open(src, "rb") as f:
    binary_data = f.read()

stage = 0

ser.write(f"AT+FSCREATE=C:\\{dest}\r".encode())
time.sleep(1)
while ser.in_waiting:
    c = ser.read()
    print(chr(int.from_bytes(c)), end="")
    if c == '\r':
        print()
ser.write(f"AT+FSWRITE=C:\\{dest},0,{len(binary_data)},600\r".encode())
time.sleep(1)
while ser.in_waiting:
    c = ser.read()
    print(chr(int.from_bytes(c)), end="")
    if c == '\r':
        print()
ser.write(binary_data)
