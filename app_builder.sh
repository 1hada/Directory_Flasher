#!/bin/bash

gcc -Wall -Wno-format -o directory_flasher_app directory_flasher.c -Wno-deprecated-declarations -Wno-format-security -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

./directory_flasher_app
