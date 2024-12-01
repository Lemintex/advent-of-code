#!/bin/bash

# Check if a destination directory was provided
if [ $# -eq 0 ]; then
    echo "Error: Please provide a destination directory as an argument."
    exit 1
fi

for i in {1..25}
do
  if [ $i -lt 10 ]; then
    cp -r day-xx/ "$1/day-0$i/"
  else
    cp -r day-xx/ "$1/day-$i/"
  fi
done

echo "Files copied successfully."
