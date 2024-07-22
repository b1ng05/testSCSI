#include <cstdlib>
#include <iostream>
#include <string>

void scanDevices() {
    std::system("lsscsi");
}

std::string getInput(const std::string &prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main() {
    scanDevices();
    
    std::string devicePath = getInput("Enter the device path to unload (e.g., /dev/st0): ");
    
    // Unload cartridge
    std::string command = "mt -f " + devicePath + " eject";
    int result = std::system(command.c_str());
    if (result == 0) {
        std::cout << "Cartridge unloaded successfully." << std::endl;
    } else {
        std::cerr << "Failed to unload cartridge." << std::endl;
    }

    return 0;
}

