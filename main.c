#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

// Default resolution.
#define MAX_ITERATIONS 255

// double CENTRE_X = -0.722222;
// double CENTRE_Y = 0;
// double CENTRE_X = -1.7400623825;
// double CENTRE_Y = 0.028175339779;

double CENTRE_X = -0.74834786263068993619;
double CENTRE_Y = -0.063037465497871903373;

enum COLOR {
  RED, GREEN, BLUE
};


/* Calculate the number of iterations a point takes to leave a bound. */
int iterations(double cr, double ci, int max_it) {
  int i = 0;
  double complex z = 0 + 0*I;
  double complex c = cr + ci*I;
  for (i=0; i<max_it; i++) {
    z = z*z + c;
    if (cabs(z) > 4)
      break;
  }
  return i;
}


/* Linearly map a value from one range into another. */
double linmap(double val, double lower1, double upper1, double lower2, double upper2) {
  return ((val - lower1) / (upper1 - lower1)) * (upper2 - lower2) + lower2;
}


/* Calculate the colour for coordinates (x, y) and write into color[]. */
void get_color(int x, int y, int maxX, int maxY, double ZOOM, unsigned char color[]) {

  double ASPECT_RATIO = (double)maxX / (double)maxY;
  double X_LOWER = (CENTRE_X - (ASPECT_RATIO * ZOOM));
  double X_UPPER = (CENTRE_X + (ASPECT_RATIO * ZOOM));
  double Y_LOWER = (CENTRE_Y - ZOOM);
  double Y_UPPER = (CENTRE_Y + ZOOM);

  // Calculate the viewport of our render.
  double x1 = linmap(x, 0, maxX, X_LOWER, X_UPPER);
  double y1 = linmap(y, 0, maxY, Y_LOWER, Y_UPPER);

  // How many iterations before this complex value "escapes" our bound of 4?
  int its = iterations(x1, y1, MAX_ITERATIONS);

  // Normalise number of iterations to a value in [0, 1].
  double t = (double) its / (double) MAX_ITERATIONS;

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
  int dimx = atoi(argv[1]);
  int dimy = atoi(argv[2]);
  int secs = atoi(argv[3]);
  int fps = atoi(argv[4]);

  double ZOOM = 2;
  
  for (int i=0; i<secs*fps; i++) {
    generate_file(dimx, dimy, ZOOM, i);
    ZOOM *= 0.97;
    if (i % (fps/4) == 0)
      printf("%.2f%% done!\n", 100 * (double)i/(secs*fps));
  }

  return EXIT_SUCCESS;
}
