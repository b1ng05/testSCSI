#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <sys/mtio.h>
#include <sys/stat.h>
#include <sys/types.h>

// Include the linux/version.h before IBM_tape.h to define LINUX_VERSION_CODE and KERNEL_VERSION
#include <linux/version.h>
#include "/home/prouser/Videos/lin_tape-3.0.66/IBM_tape.h"

int main() { 
    int fd = open("/dev/IBMtape0", O_RDONLY);
    if (fd < 0) { 
        std::cerr << "Failed to open tape device: " << strerror(errno) << std::endl;
        return 1;
    }
    
    std::cout << "LINUX_VERSION_CODE: " << LINUX_VERSION_CODE << std::endl;
    
    // Unload tape command using MTIOCTOP
    struct mtop tape_operation;
    tape_operation.mt_op = MTOFFL; // Unload the tape
    tape_operation.mt_count = 1;

    if (ioctl(fd, MTIOCTOP, &tape_operation) < 0) {
        std::cerr << "ioctl MTOFFL failed: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }

    std::cout << "Cartridge successfully unloaded." << std::endl;

    close(fd);
    return 0;
}

