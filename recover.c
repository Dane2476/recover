#include <stdio.h>
#include <stdint.h>

int main (int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Please provide a single command line argument\n");
        return 1;
    }

    FILE *raw_file = fopen(argv[1], "r");

    if (raw_file == NULL)
    {
        fprintf(stderr, "Invalid file\n");
        return 2;
    }

    typedef uint8_t  BYTE;

    BYTE buffer[512];

    char filename[sizeof(raw_file)];

    int filecounter = 0;
    FILE *img;

    while (fread(&buffer, 512, 1, raw_file) == 1)
    {
        if (buffer[0] == 0xff &&
        buffer[1] == 0xd8 &&
        buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0)
        {
            if (filecounter >= 1)
            {
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", filecounter);
            img = fopen(filename, "w");
            fwrite(&buffer, 512, 1, img);
            filecounter++;
        }
        else
        {
            if (filecounter >= 1)
            {
                fwrite(&buffer, 512, 1, img);
            }

        }
    }

    printf("%i\n", filecounter);

    fclose(img);
    fclose(raw_file);


}



// while I read in 512 bytes
//       if it's a signature
//             if there's a jpg open, close it.
//             open a new jpg
//             write the buffer to the new jpg
//       else  // it's not a signature
//           if a jpg is already open
//               write the buffer to that jpg
//   // end while
//  close the last jpg
//  close the rawfile