#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Error: Usage ./recover card.raw\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[512];
    FILE *imagen = NULL;
    char filename[3];
    int i = 0;

    // Read 512 B into a buffer
    while (fread(buffer, 512, 1, card))
    {
        // If start of a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            // If first jpeg
            if (imagen == NULL)
            {
                sprintf(filename, "%.03i.jpg", i);
                imagen = fopen(filename, "w");
                fwrite(buffer, 512, 1, imagen);
                i++;
            }
            // Not the first jpeg
            // You find the new jpeg but first you have to close the previous jpeg we were writing to.
            else
            {
                fclose(imagen);
                sprintf(filename, "%.03i.jpg", i);
                imagen = fopen(filename, "w");
                fwrite(buffer, 512, 1, imagen);
                i++;
            }
        }
        // Else not the start of a new jpeg
        else
        {
            // If already found a jpeg
            if (imagen != NULL)
            {
                fwrite(buffer, 512, 1, imagen);
            }
        }
    }

    fclose(imagen);
    fclose(card);
}