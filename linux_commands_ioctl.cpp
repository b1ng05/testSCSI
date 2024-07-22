#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/mtio.h>
#include <sys/ioctl.h>
#include <string.h>
#include <scsi/sg.h>

void scanDevices() {
    std::system("lsscsi -g");
}

std::string getInput(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

bool unloadCartridge(const std::string &devicePath) {
    int fd = open(devicePath.c_str(), O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return false;
    }

    sg_io_hdr_t io_hdr;
    unsigned char cdb[6] = {0x1B, 0, 0, 0, 3, 0};  // START STOP UNIT command
    unsigned char sense[32] = {0};

    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));
    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = sizeof(cdb);
    io_hdr.mx_sb_len = sizeof(sense);
    io_hdr.dxfer_direction = SG_DXFER_NONE;
    io_hdr.cmdp = cdb;
    io_hdr.sbp = sense;
    io_hdr.timeout = 20000;  // 20 seconds

    std::cout << "Sending SCSI command to unload cartridge..." << std::endl;
    if (ioctl(fd, SG_IO, &io_hdr) < 0) {
        perror("ioctl SG_IO failed");
        close(fd);
        return false;
    }

    if ((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK) {
        std::cerr << "SCSI command failed" << std::endl;
        std::cerr << "Sense key: " << static_cast<int>(sense[2] & 0x0F) << std::endl;
        std::cerr << "Additional sense code: " << static_cast<int>(sense[12]) << std::endl;
        std::cerr << "Additional sense code qualifier: " << static_cast<int>(sense[13]) << std::endl;
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

int main() {
    scanDevices();
    
    std::string devicePath = getInput("Enter the device path to unload (e.g., /dev/sg4): ");
    
    if (unloadCartridge(devicePath)) {
        std::cout << "Cartridge unloaded successfully." << std::endl;
    } else {
        std::cerr << "Failed to unload cartridge." << std::endl;
    }

    return 0;
}

