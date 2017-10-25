#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define MAX_X 1920
#define MAX_Y 1080
#define ASPECT_RATIO ((double)MAX_X/(double)MAX_Y)

#define ZOOM 0.9
#define CENTRE_X -0.722222
#define CENTRE_Y 0

#define X_LOWER (CENTRE_X - (ASPECT_RATIO * ZOOM)) 
#define X_UPPER (CENTRE_X + (ASPECT_RATIO * ZOOM))
#define Y_LOWER (CENTRE_Y - ZOOM)
#define Y_UPPER (CENTRE_Y + ZOOM)

#define MAX_ITERATIONS 255


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
void get_color(int x, int y, int maxX, int maxY, unsigned char color[]) {
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
void generate_file(int dimx, int dimy) {
  int i, j;
  FILE *fp = fopen("mandelbrot.ppm", "wb");
  write_header(fp, dimx, dimy);

  unsigned char color[3];
  
  for (j=0; j<dimy; ++j)
    for (i=0; i<dimx; ++i)
      {
	get_color(i, j, dimx, dimy, color);
	fwrite(color, 1, 3, fp);
      }

  fclose(fp);
}


int main(int argc, char *argv[]) {
  int dimx = MAX_X, dimy = MAX_Y;

  if (argc == 3) {
    dimx = atoi(argv[1]);
    dimy = atoi(argv[2]);
  }

  printf("Rendering file at %dx%d resolution.\n", dimx, dimy);
  generate_file(dimx, dimy);
  printf("Render complete!\n");
  return EXIT_SUCCESS;
}
