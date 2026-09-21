#include "barcode.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Left-side digit encodings
 *
 * 1 = white
 * 0 = black
 */
static const unsigned char L_CODES[10] = {
    0b1110010,  // 0
    0b1100110,  // 1
    0b1101100,  // 2
    0b1010000,  // 3
    0b1011100,  // 4
    0b1001110,  // 5
    0b1000010,  // 6
    0b1000100,  // 7
    0b1001000,  // 8
    0b1110100   // 9
};


/*
 * Return one bit from a binary value.
 */
int get_bit(unsigned char value, int position) {
    return (value >> position) & 1;
}


/*
 * Fill one vertical column.
 *
 * color = 0   -> black
 * color = 255 -> white
 */
void fill_column(struct image *img, int x, int color) {

    for (int y = 0; y < img->height; y++) {

        int index = x + y * img->width;

        img->pixels[index].r = color;
        img->pixels[index].g = color;
        img->pixels[index].b = color;
    }
}


/*
 * Draw one barcode digit.
 *
 * right_side = 0 -> left encoding
 * right_side = 1 -> inverted/right encoding
 */
void fill_digit(struct image *img,
                int start_x,
                int digit,
                int right_side) {

    unsigned char code = L_CODES[digit];

    for (int i = 0; i < 7; i++) {

        int bit = get_bit(code, 6 - i);

        /* Invert encoding on right side */
        if (right_side) {
            bit = !bit;
        }

        if (bit == 0) {
            fill_column(img, start_x + i, 0);
        } else {
            fill_column(img, start_x + i, 255);
        }
    }
}


/*
 * Draw start, middle, or end pattern.
 */
void fill_pattern(struct image *img,
                  int start_x,
                  unsigned int pattern,
                  int length) {

    for (int i = 0; i < length; i++) {

        int bit =
            (pattern >> (length - 1 - i)) & 1;

        if (bit == 0) {
            fill_column(img, start_x + i, 0);
        } else {
            fill_column(img, start_x + i, 255);
        }
    }
}


/*
 * Generate barcode image.
 */
struct image *barcode(char *data, int width, int height) {

    /* Allocate image structure */
    struct image *img =
        malloc(sizeof(struct image));

    if (img == NULL) {
        return NULL;
    }

    img->width = width;
    img->height = height;


    /* Allocate width * height pixels */
    img->pixels =
        malloc(width * height * sizeof(struct pixel));

    if (img->pixels == NULL) {
        free(img);
        return NULL;
    }


    /* Initialize entire image to white */
    for (int i = 0; i < width * height; i++) {

        img->pixels[i].r = 255;
        img->pixels[i].g = 255;
        img->pixels[i].b = 255;
    }


    /*
     * Skip 9-pixel left quiet zone.
     */
    int x = 9;


    /* Start pattern: 010 */
    fill_pattern(img, x, 0b010, 3);
    x += 3;


    /* First six digits */
    for (int i = 0; i < 6; i++) {

        int digit = data[i] - '0';

        fill_digit(img, x, digit, 0);

        x += 7;
    }


    /* Middle pattern: 10101 */
    fill_pattern(img, x, 0b10101, 5);
    x += 5;


    /* Last six digits */
    for (int i = 6; i < 12; i++) {

        int digit = data[i] - '0';

        fill_digit(img, x, digit, 1);

        x += 7;
    }


    /* End pattern: 010 */
    fill_pattern(img, x, 0b010, 3);


    return img;
}