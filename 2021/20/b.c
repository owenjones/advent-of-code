#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

typedef struct image
{
  uint32_t dimension;
  char *pixels;
} image_t;

char get_pixel(image_t *image, int32_t x, int32_t y)
{
  if (x > (image->dimension - 1) || x < 0 || y > (image->dimension - 1) || y < 0)
  {
    // extend 'infinite image' over the edges of our actually finite image
    return '0';
  }

  return image->pixels[(x + (y * image->dimension))];
}

void set_pixel(image_t *image, int32_t x, int32_t y, char pixel)
{
  if (x > (image->dimension - 1) || x < 0 || y > (image->dimension - 1) || y < 0)
    return;

  if (pixel == '#')
    pixel = '1';
  if (pixel == '.')
    pixel = '0';

  image->pixels[(x + (y * image->dimension))] = pixel;
}

image_t *make_image(uint32_t dimension, char *pixels)
{
  image_t *image = calloc(1, sizeof(image_t));
  if (pixels != NULL)
  {
    image->dimension = (dimension + 500);
    image->pixels = calloc((image->dimension * image->dimension), sizeof(char));

    for (size_t i = 0; i < (image->dimension * image->dimension); i++)
    {
      image->pixels[i] = '0';
    }

    uint32_t offset = (image->dimension / 2) - (dimension / 2);
    size_t i = 0;
    for (size_t y = 0; y < dimension; y++)
    {
      for (size_t x = 0; x < dimension; x++)
      {
        set_pixel(image, (x + offset), (y + offset), pixels[i]);
        i++;
      }
    }
  }
  else
  {
    image->dimension = dimension;
    image->pixels = calloc((image->dimension * image->dimension), sizeof(char));
  }
  return image;
}

void free_image(image_t *image)
{
  free(image->pixels);
  free(image);
}

void load_data(char *file, uint8_t *algorithm, image_t **image)
{
  FILE *fptr;
  if ((fptr = fopen(file, "r")) == NULL)
  {
    printf("Error opening file\n");
    exit(1);
  }

  char *input = NULL;
  size_t bufsize = 0;
  if (getline(&input, &bufsize, fptr) == -1)
    exit(1);

  strcpy(algorithm, input);

  fscanf(fptr, "%s\n", input); // read in first line of grid to get dimension of image
  uint32_t dimension = strlen(input);
  char *pixels = calloc((dimension * dimension), sizeof(char));
  memcpy(pixels, input, dimension);

  for (size_t i = 1; i < dimension; i++)
  {
    fscanf(fptr, "%s\n", input);
    memcpy(&pixels[(i * dimension)], input, dimension);
  }
  fclose(fptr);
  free(input);

  *image = make_image(dimension, pixels);
  free(pixels);
}

uint32_t bin2dec(char *bin)
{
  // convert string of binary characters into decimal
  uint32_t dec = 0;
  uint8_t b;
  for (size_t i = 0; i < strlen(bin); i++)
  {
    b = bin[i] - 48;
    dec += b * pow(2, (strlen(bin) - 1 - i));
  }
  return dec;
}

image_t *apply_algorithm(char *algorithm, image_t *input)
{
  image_t *output = make_image(input->dimension, NULL);
  size_t index;
  char binary[10];
  binary[9] = '\0';

  for (size_t y = 0; y < output->dimension; y++)
  {
    for (size_t x = 0; x < output->dimension; x++)
    {
      binary[0] = get_pixel(input, (x - 1), (y - 1));
      binary[1] = get_pixel(input, (x), (y - 1));
      binary[2] = get_pixel(input, (x + 1), (y - 1));
      binary[3] = get_pixel(input, (x - 1), (y));
      binary[4] = get_pixel(input, (x), (y));
      binary[5] = get_pixel(input, (x + 1), (y));
      binary[6] = get_pixel(input, (x - 1), (y + 1));
      binary[7] = get_pixel(input, (x), (y + 1));
      binary[8] = get_pixel(input, (x + 1), (y + 1));

      index = bin2dec(binary);
      set_pixel(output, x, y, algorithm[index]);
    }
  }

  free_image(input);
  return (output);
}

uint32_t count_lit_pixels(image_t *image)
{
  uint32_t count = 0;
  for (size_t y = 50; y < (image->dimension - 50); y++)
  {
    for (size_t x = 50; x < (image->dimension - 50); x++)
    {
      count += (get_pixel(image, x, y) == '1');
    }
  }
  return count;
}

void print_image(image_t *image)
{
  char c;
  for (size_t y = 0; y < image->dimension; y++)
  {
    for (size_t x = 0; x < image->dimension; x++)
    {
      c = (get_pixel(image, x, y) == '1') ? '#' : '.';
      printf("%c", c);
    }
    printf("\n");
  }
  printf("\n");
}

int main(void)
{
  char *algorithm = calloc(512, sizeof(char));
  image_t *image = NULL;
  load_data("input.txt", algorithm, &image);

  // OK so maybe not 20-trillion, but still more than 2!
  for (size_t i = 0; i < 50; i++)
    image = apply_algorithm(algorithm, image);

  uint32_t total = count_lit_pixels(image);
  free_image(image);
  printf("Lit pixels in the image: %i\n", total); // 18131
  return 0;
}
