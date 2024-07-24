#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/mtio.h>

void unloadTape(const std::string &devicePath) {
    int fd = open(devicePath.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Failed to open device");
        return;
    }

    struct mtop tapeOperation;
    tapeOperation.mt_op = MTUNLOAD;
    tapeOperation.mt_count = 1;

    if (ioctl(fd, MTIOCTOP, &tapeOperation) == -1) {
        perror("ioctl MTUNLOAD failed");
        close(fd);
        return;
    }

    std::cout << "Tape unloaded successfully." << std::endl;

    close(fd);
}

int main() {
    std::string devicePath;
    std::cout << "Enter the device path to unload (e.g., /dev/nst0): ";
    std::cin >> devicePath;

    unloadTape(devicePath);

    return 0;
}

