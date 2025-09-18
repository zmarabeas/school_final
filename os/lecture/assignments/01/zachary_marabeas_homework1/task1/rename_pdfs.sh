#!/bin/bash

found=0

for file in *.PDF; do
    if [ -f "$file" ]; then
        mv "$file" "${file%.PDF}.pdf"
        echo "move $file to ${file%.PDF}.pdf"
        found=1
    fi
done

for file in .*.PDF; do
    if [ -f "$file" ]; then
        mv "$file" "${file%.PDF}.pdf"
        echo "move $file to ${file%.PDF}.pdf"
        found=1
    fi
done

if [ $found -eq 0 ]; then
    echo "there are no *.PDF files"
fi
