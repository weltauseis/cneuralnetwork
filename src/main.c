#include <stdio.h>
#include "types.h"
#include "dataset.h"

int main(int argc, char *argv[])
{
    if (!load_training_dataset())
        return EXIT_FAILURE;
}