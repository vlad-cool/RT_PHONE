#!/bin/python3
import serial
import select
import time
import sys

def is_input_available():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

ser = serial.Serial('/dev/ttyUSB0', 38400, timeout=1)


while True:
    while ser.in_waiting:
        c = ser.read()
        with open("out", "ab") as f:
            f.write(c)
        print(chr(int.from_bytes(c)), end="")
        if c == '\r':
            print()
        # time.sleep(0.04)
    while is_input_available():
        s = input()
        if s == "write":
            i = 0
            print("writing")
            with open("to_write", "rb") as f:
                while True:
                    c = f.read(1)
                    if not c:
                        break
                    print(c, end="")
                    # c = ord(f.read(1)).to_bytes
                    ser.write(c)
                    # time.sleep(0.04)
                    print(i)
                    i += 1
                ser.write("\r".encode())

        else:
            for i in range(len(s)):
                ser.write(s[i].encode())
                time.sleep(0.01)
            ser.write("\r".encode())
