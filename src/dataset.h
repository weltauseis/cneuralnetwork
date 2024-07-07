#include "types.h"
struct Image
{
    u8 pixels[28 * 28];
    u8 label;
};
typedef struct Image Image;

b32 load_training_dataset();