#!/bin/bash

OBJECTS=`ar t $1 | wc -l`
if [ $OBJECTS -ne 2 ]; then
    echo "Invalid number of objects in the library"
    exit 1
fi

VOLUME=`nm -C $1 | grep 'T TradeIndex::TotalVolume' | wc -l`
if [ $VOLUME -ne 1 ]; then
    echo "TotalVolume isn't in the library"
    exit 1
fi
