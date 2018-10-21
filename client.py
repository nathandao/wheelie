#!/usr/bin/env python3

import sys
import serial

def toSignedByte(i):
    return i.to_bytes(1, byteorder='big', signed=True)

def moveCommand(x, y):
    return toSignedByte(x) + toSignedByte(y)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: client.py <serial port> <x> <y>" +
              "Send x y move command to wheelie")

    ser = serial.Serial(sys.argv[1])
    x = int(sys.argv[2])
    y = int(sys.argv[3])

    ser.write(moveCommand(x,y))
