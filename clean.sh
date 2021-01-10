#!/bin/bash
FILE=$PWD/tmp
if [ -d "$FILE" ]; then
  echo "Remove tmp"
   rm -r tmp/
fi # remove tmp folder if exists
