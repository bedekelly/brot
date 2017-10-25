# Brot

Quick 'n' dirty bitmap renderer for a mandelbrot set in C.

![Mandelbrot](example.bmp)
*This image is a 4k render, so it's pretty weighty: 23mb according to GitHub!*

Requirements:
* GNU Make
* C compiler recognised by GNU Make

Soft requirements:
* GraphicsMagick (to convert PPM to BMP)
* OSX (for `open` command)

### Usage:

```
./render.sh 1920 1080
```

For colour mapping, this uses the Bernstein Polynomials implementation found [here](https://solarianprogrammer.com/2013/02/28/mandelbrot-set-cpp-11/). It's just a bit nicer than a linear map.
