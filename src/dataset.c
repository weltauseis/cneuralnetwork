#include "dataset.h"
#include <stdio.h>

ImageDataset *load_training_dataset()
{
    ImageDataset *dataset = malloc(sizeof(ImageDataset));

    // IMAGE DATA

    FILE *training_images_files = fopen("data/train-images.idx3-ubyte", "rb");

    // read the header

    u8 img_header[16];
    if (fread(img_header, 16, 1, training_images_files) != 1)
    {
        fprintf(stderr, "Unable to read training images file.\n");
        return false;
    }

    // [offset] [type]          [value]          [description]
    // 0000     32 bit integer  0x00000803(2051) magic number
    // 0004     32 bit integer  60000            number of images
    // 0008     32 bit integer  28               number of rows
    // 0012     32 bit integer  28               number of columns
    // 0016     unsigned byte   ??               pixel
    // 0017     unsigned byte   ??               pixel

    // All the integers in the files are stored in the MSB first (high endian) format used by most non-Intel processors.
    // Users of Intel processors and other low-endian machines must flip the bytes of the header.

    {
        u32 magic_number = (u32)img_header[0] << 24 | (u32)img_header[1] << 16 | (u32)img_header[2] << 8 | (u32)img_header[3];
        u32 number_of_images = (u32)img_header[4] << 24 | (u32)img_header[5] << 16 | (u32)img_header[6] << 8 | (u32)img_header[7];
        u32 rows = (u32)img_header[8] << 24 | (u32)img_header[9] << 16 | (u32)img_header[10] << 8 | (u32)img_header[11];
        u32 columns = (u32)img_header[12] << 24 | (u32)img_header[13] << 16 | (u32)img_header[14] << 8 | (u32)img_header[15];

        printf("-- TRAINING IMAGE DATA LOADED --\n");
        printf("Magic number : %d\n", magic_number);
        printf("Number of images : %d\n", number_of_images);
        printf("Rows : %d\n", rows);
        printf("Columns : %d\n", columns);

        dataset->nb = number_of_images;
        dataset->rows = rows;
        dataset->cols = columns;
        dataset->data = malloc(rows * columns * number_of_images * sizeof(u8));
    }

    // read the pixels
    u32 image_size = dataset->cols * dataset->rows * sizeof(u8);
    for (u32 i = 0; i < dataset->nb; i++)
    {
        fread(&dataset->data[i * image_size], image_size, 1, training_images_files);
    }
    fclose(training_images_files);

    // LABEL DATA

    FILE *training_labels_file = fopen("data/train-labels.idx1-ubyte", "rb");

    // read the header

    u8 label_header[8];
    if (fread(label_header, 8, 1, training_labels_file) != 1)
    {
        fprintf(stderr, "Unable to read training labels file.\n");
        return false;
    }

    // [offset] [type]          [value]          [description]
    // 0000     32 bit integer  0x00000801(2049) magic number (MSB first)
    // 0004     32 bit integer  60000            number of items
    // 0008     unsigned byte   ??               label
    // 0009     unsigned byte   ??               label

    {
        u32 magic_number = (u32)label_header[0] << 24 | (u32)label_header[1] << 16 | (u32)label_header[2] << 8 | (u32)label_header[3];
        u32 number_of_items = (u32)label_header[4] << 24 | (u32)label_header[5] << 16 | (u32)label_header[6] << 8 | (u32)label_header[7];

        printf("-- TRAINING LABEL DATA LOADED --\n");
        printf("Magic number : %d\n", magic_number);
        printf("Number of items : %d\n", number_of_items);

        dataset->labels = malloc(number_of_items * sizeof(u8));
    }

    // read the labels
    for (u32 i = 0; i < dataset->nb; i++)
    {
        fread(&dataset->labels[i], sizeof(u8), 1, training_labels_file);
    }

    // visualize the first few images
    for (u32 i = 0; i < 5; i++)
    {
        for (u32 y = 0; y < 28; y++)
        {
            for (u32 x = 0; x < 28; x++)
            {
                printf("%c", dataset->data[i * image_size + y * 28 + x] > 127 ? '#' : '.');
            }
            printf("\n");
        }
        printf("Label : %d\n", dataset->labels[i]);
    }

    return dataset;
}