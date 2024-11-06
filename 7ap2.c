#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

// Function to count characters, words, and lines
void countStats(const char *str, int *charCount, int *wordCount, int *lineCount) {
    *charCount = *wordCount = *lineCount = 0;
    int inWord = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        (*charCount)++;
        if (str[i] == '\n') (*lineCount)++;
        
        if (isspace(str[i])) {
            inWord = 0;
        } else if (!inWord) {
            inWord = 1;
            (*wordCount)++;
        }
    }

    if (str[strlen(str) - 1] != '\n') (*lineCount)++;
}

int main() {
    int fd;
    char *myfifo1 = "/tmp/myfifo1"; // FIFO file path
    char *myfifo2 = "/tmp/myfifo2";

    // Creating the named files (FIFOs)
    mkfifo(myfifo1, 0666);
    mkfifo(myfifo2, 0666);

    char received[80], stats[80];
    FILE *file;

    while (1) {
        // Open FIFO1 for read only and read the sentence from Process 1
        fd = open(myfifo1, O_RDONLY);
        read(fd, received, 80);
        close(fd);

        printf("Received from Process 1: %s", received);

        // Count characters, words, and lines
        int charCount, wordCount, lineCount;
        countStats(received, &charCount, &wordCount, &lineCount);

        // Write the results to a text file
        file = fopen("stats.txt", "w");
        fprintf(file, "Characters: %d, Words: %d, Lines: %d\n", charCount, wordCount, lineCount);
        fclose(file);

        // Read the file content back into a string
        file = fopen("stats.txt", "r");
        fgets(stats, sizeof(stats), file);
        fclose(file);

        // Open FIFO2 for write only and send the stats back to Process 1
        fd = open(myfifo2, O_WRONLY);
        write(fd, stats, strlen(stats) + 1);
        close(fd);
    }

    return 0;
}
