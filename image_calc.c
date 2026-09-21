#include "barcode.h"
#include "loader.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv){
     if(argc != 5) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <barcode_value> <width> <height> <output_image_path>\n");
        return -1;
    }
    // TODO: parse the arguments in argv. 
    char *barcode_value = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    char *output_image_path = argv[4];
    
    struct image *img = barcode(barcode_value, width, height);
     if(img == NULL){
        return -1;
    }

     // TODO: save result to file
    int result = saveimage(output_image_path, img);
     if(result != 0){
        printf("Failed to save image\n");
        return -1;
    }
    /* prevent memory leak*/
    free(img->pixels);
    free(img);

   

    return 0;
}
