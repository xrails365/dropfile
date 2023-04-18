#include <Windows.h>
#include <iostream>
#include "resource1.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>

HMODULE GMS() {
    HMODULE hModule = NULL;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
        (LPCTSTR)GMS,
        &hModule
    );
    return hModule;
}

int main2() {
    // Open file for binary output
    std::ofstream outfile("output.bin", std::ios::binary);

    // Example binary data
    //std::vector<uint8_t> binary_data = { 0x5E, 0x3B, 0x64, 0x3B, 0x6A, 0x3B, 0x70, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    std::vector<uint8_t> binary_data = { 0x4D, 0x5A, 0x90, 0x00, 0x03, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00 };
    // Print out the binary data as bytes
    for (size_t i = 0; i < 10; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(binary_data[i]) << " ";
        // Write the binary data to the file
    }
    // Write the binary data to the file
    outfile.write(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());

    // Close the file
    outfile.close();

    return 0;
}

int main() {
    std::string hex_string;
    HRSRC h = FindResource(GMS(), MAKEINTRESOURCE(DEMO_TEXT), MAKEINTRESOURCE(TEXTFILE));
    HGLOBAL hData = LoadResource(GMS(), h);
    DWORD hSize = SizeofResource(GMS(), h);
    char* hFinal = (char*)LockResource(hData);
    hex_string.assign(hFinal, hSize);
    // Print the response headers
    //std::cout << hex_string << std::endl;
     // Convert the hex string to binary data
    std::vector<uint8_t> binary_data;
    for (size_t i = 0; i < hex_string.length(); i += 2) {
        std::string byte_string = hex_string.substr(i, 2);
        
        // Check if each character in byte_string is a valid hex character
        if (byte_string.length() != 2 ||
            !isxdigit(byte_string[0]) || !isxdigit(byte_string[1])) {
            // Throw an error if an invalid character is found
            throw std::invalid_argument("Invalid hex string: " + byte_string);
        }
        uint8_t byte_data = 0;
        try {
            byte_data = static_cast<uint8_t>(std::stoi(byte_string, nullptr, 16));
        }
        catch (const std::out_of_range&) {
            // Handle the out_of_range exception
            std::cerr << "Value out of range for hex string: " << byte_string << std::endl;
            // You can either throw a new exception or skip this byte
            continue; // skip this byte
        }

        //uint8_t byte_data = static_cast<uint8_t>(std::stoi(byte_string, nullptr, 16));
        binary_data.push_back(byte_data);
    }
    // Open file for binary output
    std::ofstream outfile("output.bin", std::ios::binary);

    // Check if the file was opened successfully
    if (!outfile) {
        std::cerr << "Failed to open output file" << std::endl;
        return 1; // or throw an exception, depending on your needs
    }

    // Write the binary data to the file
    outfile.write(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());
    outfile.close();
    
    return 0;
}
