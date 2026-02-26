#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    char *fifo_path = "/tmp/my_messenger";
    char buffer[1024];
    int fd;

    // Create the named pipe (FIFO) if it doesn't exist
    mkfifo(fifo_path, 0666);

    printf("Waiting for input from other terminals...\n");

    while (1) {
        // Open for reading (this blocks until a client opens it for writing)
        fd = open(fifo_path, O_RDWR);
        
        // Read the data
        int bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Received: %s", buffer);
        }

        char returning_message[] = "hihihi"; 

        if (write(fd, returning_message, sizeof(returning_message)) == -1) {
          perror("write");
        }

        

        close(fd); // Close and loop back to wait for the next "write"
    }

    return 0;
}
