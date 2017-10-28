#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <complex.h>
#include <math.h>

// Default resolution.
#define MAX_ITERATIONS 350

// The closer this is to 1, the more *slowly* this will zoom.
// Use `log(2, multiplier^numFrames)<sizeof(long double)` to work out if we'll hit rounding errors.
#define ZOOM_MULTIPLIER ((long double) 0.9525)

// Which coordinates should we zoom in on?
long double CENTRE_X = 0.3994999999988, CENTRE_Y = 0.195303;


enum COLOR {
  RED, GREEN, BLUE
};


/* Calculate the number of iterations a point takes to leave a bound. */
int iterations(long double cr, long double ci, int max_it) {
  int i = 0;
  long double complex z = 0 + 0*I;
  long double complex c = cr + ci*I;
  for (i=0; i<max_it; i++) {
    z = z*z + c;
    if (cabsl(z) > 4)
      break;
  }
  // printf("%d,", i);
  return i;
}


/* Linearly map a value from one range into another. */
long double linmap(long double val, long double lower1, long double upper1, long double lower2, long double upper2) {
  return ((val - lower1) / (upper1 - lower1)) * (upper2 - lower2) + lower2;
}


/* Calculate the colour for coordinates (x, y) and write into color[]. */
void get_color(int x, int y, int maxX, int maxY, long double ZOOM, unsigned char color[]) {

  long double ASPECT_RATIO = (double)maxX / (double)maxY;
  long double X_LOWER = (CENTRE_X - (ASPECT_RATIO * ZOOM));
  long double X_UPPER = (CENTRE_X + (ASPECT_RATIO * ZOOM));
  long double Y_LOWER = (CENTRE_Y - ZOOM);
  long double Y_UPPER = (CENTRE_Y + ZOOM);

  // Calculate the viewport of our render.
  long double x1 = linmap(x, 0, maxX, X_LOWER, X_UPPER);
  long double y1 = linmap(y, 0, maxY, Y_LOWER, Y_UPPER);

  // How many iterations before this complex value "escapes" our bound of 4?
  int its = iterations(x1, y1, MAX_ITERATIONS);

  // Normalise number of iterations to a value in [0, 1].
  long double t = (long double) its / (long double) MAX_ITERATIONS;

  // Calculate values for Bernstein Polynomials in [0, 255] interval.
  color[RED] = linmap(8.5 * (1-t) * (1-t) * (1-t) * t, 0, 1, 0, 255);
  color[GREEN] = linmap(15 * (1-t) * (1-t) * t*t, 0, 1, 0, 255);
  color[BLUE] = linmap(9 * (1-t) * t*t*t, 0, 1, 0, 255);
}


/* Write the header for a PPM file. */
void write_header(FILE *fp, int dimx, int dimy) {
  fprintf(fp, "P6\n");  // Write magic chars.
  fprintf(fp, "%d %d\n", dimx, dimy);  // Write width and height.
  fprintf(fp, "255\n");  // Write max colour value.
}


/* Generate a Mandelbrot image. */
void generate_file(int dimx, int dimy, double ZOOM, int frameNo) {
  int i, j;

  char filename[0x20];
  snprintf(filename, sizeof(filename), "frames/frame-%d.ppm", frameNo);
  FILE *fp = fopen(filename, "wb");

  write_header(fp, dimx, dimy);

  unsigned char color[3];

  for (j=0; j<dimy; ++j)
    for (i=0; i<dimx; ++i)
      {
	get_color(i, j, dimx, dimy, ZOOM, color);
	fwrite(color, 1, 3, fp);
      }

  fclose(fp);
}


int main(int argc, char *argv[]) {
  // Grab our parameters from the system args.
  int dimx = atoi(argv[1]);
  int dimy = atoi(argv[2]);
  int secs = atoi(argv[3]);
  int fps = atoi(argv[4]);
  int i = atoi(argv[5]);
  int cores = atoi(argv[6]);

  // Initialise the zoom using i to properly interpolate frames.
  long double ZOOM = DBL_EPSILON / pow(ZOOM_MULTIPLIER, i);

  // i should be incremented by the number of parallel processes we have.
  for (; i<secs*fps; i+=cores) {
    // Write a frame with the current zoom.
    generate_file(dimx, dimy, ZOOM, i);

    // Only render every `cores` zoom level.
    ZOOM /= (pow(ZOOM_MULTIPLIER, cores));


    // Get some friendly logging!
    if (i % (fps/4) == 0)
      printf("%.2f%% done!\n", 100 * (double)i/(secs*fps));
  }

  return EXIT_SUCCESS;
}
