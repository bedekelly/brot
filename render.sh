#!/bin/bash

make main &&
    ./main $1 $2 &&
    gm convert mandelbrot.ppm mandelbrot.bmp &&
    open mandelbrot.bmp

echo "Your file has been saved as ./mandelbrot.bmp"
