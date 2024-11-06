#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd;
    char *myfifo1 = "/tmp/myfifo1"; // FIFO file path
    char *myfifo2 = "/tmp/myfifo2";

    // Creating the named files (FIFOs)
    mkfifo(myfifo1, 0666);
    mkfifo(myfifo2, 0666);

    char input[80], output[80];

    while (1) {
        // Open FIFO1 for write only and get input from user
        fd = open(myfifo1, O_WRONLY);
        printf("Enter a sentence: ");
        fgets(input, 80, stdin); // 80 is maximum length
        write(fd, input, strlen(input) + 1); // Write input to FIFO
        close(fd);

        // Open FIFO2 for read only and read the processed output
        fd = open(myfifo2, O_RDONLY);
        read(fd, output, sizeof(output));
        printf("Statistics from Process 2: %s\n", output); // Print the received message
        close(fd);
    }

    return 0;
}
