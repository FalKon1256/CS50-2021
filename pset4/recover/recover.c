#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Define 8bit-unsigned int as BYTE & parameters
typedef uint8_t BYTE;
#define infile "card.raw"
#define FILEBLOCK 512

int main(int argc, char *argv[])
{
    // Ensure command-line argument fixed (1 argument)
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Input file
    FILE *inptr = fopen(infile, "r");
    
    // Output file & File reading count trace parameter
    char outfile[] = "000.jpg";
    int count = 0;
    sprintf(outfile, "%03i.jpg", count);
    FILE *outptr = fopen(outfile, "w");
    
    // Ensure enough memory space
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }    

    if (outptr == NULL)
    {
        fprintf(stderr, "Could not create %s.\n", outfile);
        fclose(inptr);
        return 3;
    }


    // 512 Byte buffer size creation
    BYTE *buffer = calloc(FILEBLOCK, sizeof(BYTE));

    // Ensure enough memory space
    if (buffer == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(inptr);
        fclose(outptr);
        return 4;
    }


    // Find first JPEG data
    char firstJPEG = 'Y';
    int readcount;
    do
    {
        readcount = fread(buffer, FILEBLOCK, 1, inptr);
    }
    while (count == 0 && (buffer[0] != 0xff || buffer[1] != 0xd8 || buffer[2] != 0xff || (buffer[3] & 0xf0) != 0xe0));


    // Input file reading and output image file
    while (true)
    {
        // No input file block remain
        if (readcount != 1)
        {
            break;
        }

        // Starting with JPEG signatures
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // First JPEG file
            if (count == 0 && firstJPEG == 'Y')
            {
                fwrite(buffer, FILEBLOCK, 1, outptr);
                firstJPEG = 'N';
            }
            
            // Not first JPEG file
            else
            {
                // Close current output file & Create next output file
                fclose(outptr);
                
                count++;
                sprintf(outfile, "%03i.jpg", count);
                
                outptr = fopen(outfile, "w");
                fwrite(buffer, FILEBLOCK, 1, outptr);
                
                // Ensure enough memory space
                if (outptr == NULL)
                {
                    fprintf(stderr, "Could not create %s.\n", outfile);
                    free(buffer);
                    fclose(inptr);
                    fclose(outptr);
                    return 5;
                }
            }
        }
        
        // Not starting with JPEG signatures, continue present file writing
        else
        {
            fwrite(buffer, FILEBLOCK, 1, outptr);
        }

        // Read next data block per 512 byte data block
        readcount = fread(buffer, FILEBLOCK, 1, inptr);
    }
    // Free memory for image
    free(buffer);
    
    // Close infile
    fclose(inptr);
    
    return 0;
}