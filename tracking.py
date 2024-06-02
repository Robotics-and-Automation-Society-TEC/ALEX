#!/usr/bin/env python3
#
# Copyright (c) 2020, NVIDIA CORPORATION. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

import sys
import pyfirmata2
import argparse
from jetson_inference import detectNet
from jetson_utils import videoSource, videoOutput, Log

# Set up connection to the board
PORT = pyfirmata2.Arduino.AUTODETECT
board = pyfirmata2.Arduino(PORT)
print("Setting up the connection to the board ...")
servo_5 = board.get_pin('d:12:s')

# Initialize servo position
pos = 90
servo_5.write(pos)

# Parse the command line arguments
parser = argparse.ArgumentParser(description="Locate objects in a live camera stream using an object detection DNN.", 
                                 formatter_class=argparse.RawTextHelpFormatter, 
                                 epilog=detectNet.Usage() + videoSource.Usage() + videoOutput.Usage() + Log.Usage())

parser.add_argument("input", type=str, default="", nargs='?', help="URI of the input stream")
parser.add_argument("output", type=str, default="", nargs='?', help="URI of the output stream")
parser.add_argument("--network", type=str, default="ssd-mobilenet-v2", help="pre-trained model to load (see below for options)")
parser.add_argument("--overlay", type=str, default="box,labels,conf", help="detection overlay flags (e.g. --overlay=box,labels,conf)\nvalid combinations are:  'box', 'labels', 'conf', 'none'")
parser.add_argument("--threshold", type=float, default=0.5, help="minimum detection threshold to use") 

try:
    args = parser.parse_known_args()[0]
except:
    print("")
    parser.print_help()
    sys.exit(0)

# Create video sources and outputs
input = videoSource(args.input, argv=sys.argv)
output = videoOutput(args.output, argv=sys.argv)
    
# Load the object detection network
net = detectNet(args.network, sys.argv, args.threshold)

oldCenter=1280/2
# Process frames until EOS or the user exits
while True:
    # Capture the next image
    img = input.Capture()

    if img is None: # Timeout
        continue  

    try:
	
        # Detect objects in the image (with overlay)
        detections = net.Detect(img, overlay=args.overlay)
        detections=[detections[0]]
        # Print the detections
        print("detected {:d} objects in image".format(len(detections)))
        center=detections[0].Center[0]
        print(center)
        if float(center*0.7+0.3*oldCenter) > 1280*0.3:
            pos -= 1
            servo_5.write(pos)
        elif float(center*0.7+0.3*oldCenter) < 1280*0.7:
            pos += 1
            servo_5.write(pos)
        oldCenter=center
    except Exception as e:
        print("An error occurred:", str(e))
        pos = 90
        servo_5.write(pos)

    # Render the image
    output.Render(img)

    # Update the title bar
    output.SetStatus("{:s} | Network {:.0f} FPS".format(args.network, net.GetNetworkFPS()))

    # Print out performance info
    net.PrintProfilerTimes()

    # Exit on input/output EOS
    if not input.IsStreaming() or not output.IsStreaming():
        break

