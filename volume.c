// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    /*
    The general syntax of fread() is as follows

        size_t fread(void * buffer, size_t size, size_t count, FILE * stream)

        Parameters
            The fread() takes in a number of parameters. Let’s look at each of them in detail:

                Buffer: Pointer to the buffer where data will be stored. A buffer is a region of memory used to temporarily store data
                Size: The size of each element to be read in bytes
                Count: Number of elements to be read
                Stream: Pointer to the FILE object from where data is to be read

        Return value
            Upon successful execution, the function returns an integer value equivalent to count. In case of an error or EOF, a value less than count is returned.
    */

    /*
    The fwrite() function writes binary and text data from an array to a given data stream. It is available in both C and C++.

        size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);

        Parameters
            buffer: points to the first object in the array to be written
            size: the item size in byte
            count: the number of the objects to be written
            stream: points to a FILE object that specifies an output stream

        Return value
            The function returns the total number of elements that were successfully written. If this number is less than the count it signifies that an error has prevented all the characters from being written.
    */

    // Create an array of bytes to store the data from the WAV file header that you’ll read from the input file
    uint8_t header[HEADER_SIZE];

    // You can then use header as an argument to fread to read into the header.
    fread(header, 1, HEADER_SIZE, input);

    // Copy the header from input into output
    // You can then use header as an argument to fwrite to write from the header into the output.
    fwrite(header, 1, HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file
    // You’ll likely want to create a “buffer” in which to store audio samples that you read from the WAV file.
    int16_t buffer;

    // Read the rest of the data from the WAV file, one 16-bit (2-byte) sample at a time.
    //Your program should multiply each sample by the factor and write the new sample to the output file.
    while (fread(&buffer, 2, 1, input))
    {
        int mod = factor * buffer;
        fwrite(&mod, 2, 1, output);
    }


    // Close files
    fclose(input);
    fclose(output);
    return 0;
}
