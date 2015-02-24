#!/usr/bin/env bash

pathToFound=`echo $1 |tr '.' '/'`

for jar in `find . -name "*jar"`; do
  result=`jar tvf $jar | grep -i $pathToFound | awk '{print $8}'`
  if [ -n "$result" ]; then
    echo $jar
    for line in $result; do
      echo $line
    done
  fi
done
