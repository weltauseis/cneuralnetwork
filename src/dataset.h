#include "types.h"
struct ImageDataset
{
    u8 *data;
    u8 *labels;
    u32 nb;
    u32 cols;
    u32 rows;
};
typedef struct ImageDataset ImageDataset;

ImageDataset *load_training_dataset();