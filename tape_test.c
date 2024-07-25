#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <linux/version.h>


// Replace with the path to your ioctl headers
// Include IBM_tape.h and mtio.h from the lin_tape package
#include "/home/prouser/Videos/lin_tape-3.0.66/IBM_tape.h"
#include <linux/mtio.h>



// Function prototypes
int open_device(const char *path);
int close_device(int fd);
int test_ready(int fd);
int get_max_block_size(int fd);

int main(int argc, char *argv[]) {
    int fd = open_device("/dev/IBMtape0"); // Adjust device path as necessary
    if (fd < 0) {
        return 1;
    }

    // Test if device is ready
    if (test_ready(fd) < 0) {
        close_device(fd);
        return 1;
    }

    // Get Maximum Block Size
    if (get_max_block_size(fd) < 0) {
        close_device(fd);
        return 1;
    }

    // Close the device
    if (close_device(fd) < 0) {
        return 1;
    }

    return 0;
}

// Open and reserve the device
int open_device(const char *path) {
    int fd = open(path, O_RDWR | O_EXCL); // Exclusive access
    if (fd < 0) {
        perror("Failed to open device");
        return -1;
    }
    printf("Device %s opened successfully.\n", path);
    return fd;
}

// Close the device
int close_device(int fd) {
    if (close(fd) < 0) {
        perror("Failed to close device");
        return -1;
    }
    printf("Device closed successfully.\n");
    return 0;
}

// Test if device is ready
int test_ready(int fd) {
    // For demonstration, just print a message
    // You might need to implement an actual readiness check depending on your device and driver
    printf("Testing if device is ready...\n");
    // Placeholder for actual readiness check
    printf("Device is ready.\n");
    return 0;
}

// Get the maximum block size of the device
int get_max_block_size(int fd) {
    struct density_data_t density_data;
    if (ioctl(fd, STIOC_GET_DENSITY, &density_data) < 0) {
        perror("Failed to get density data");
        return -1;
    }
    // Example processing, assuming the structure contains max block size information
    printf("Maximum Block Size: %d\n", density_data.default_density); // Adjust field if needed
    return 0;
}

