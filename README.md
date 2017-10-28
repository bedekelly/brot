# Brot

Brot is a bad, bad parallel renderer for zooms into Mandelbrot fractals. It outputs videos in h.264 MP4 format with configurable resolution, framerate and duration..

![Mandelbrot](example.bmp)
*Shown above: one frame from a zoom.*

Requirements:
* GNU Make
* C compiler recognised by GNU Make

Soft requirements:
* GraphicsMagick (to convert PPM to BMP)
* OSX (for `open` command)

### Usage:

```
# Edit constants defined in zoom.sh, then:
./zoom.sh
```

For colour mapping, this uses the Bernstein Polynomials implementation found [here](https://solarianprogrammer.com/2013/02/28/mandelbrot-set-cpp-11/). It's just a bit nicer than a linear map.

For parallel processing, this uses Bash jobs: four renderers (main.c) are started. The four renderers are responsible for four consecutive frames of the zoom.