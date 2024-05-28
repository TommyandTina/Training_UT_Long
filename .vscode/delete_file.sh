#!/bin/bash
# Tìm và xóa file
find "$1" -name "$2.c.o" -type f -delete
find "$1" -name "$2_table.c.o" -type f -delete
find "$1" -name "$2.c.d" -type f -delete
