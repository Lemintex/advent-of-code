#!/bin/bash

# Check if a destination directory was provided
if [ $# -eq 0 ]; then
    echo "Error: Please provide a destination directory as an argument."
    exit 1
fi

for i in {1..25}
do
  # Copy contents of day-xx to day-i
  cp -r day-xx/ "$1/day-$i/"
done

echo "Files copied successfully."
