#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    // Check if a file path was provided as a command-line argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    const std::string filePath = argv[1];

    // Check if the file has the ".sav" extension
    std::string extension = filePath.substr(filePath.find_last_of('.') + 1);
    if (extension != "sav") {
        std::cerr << "The provided file does not have a '.sav' extension." << std::endl;
        return 1;
    }

    // Open the file in binary mode
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile) {
        std::cerr << "Could not open the file: " << filePath << std::endl;
        return 1;
    }

    // Read the content of the file into a vector of bytes
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inputFile), {});

    // Close the input file
    inputFile.close();

    const std::streampos addressToInsert = 0xFFFF;
    const std::vector<unsigned char> bytesToInsert = { 0x7C, 0x3C, 0x2D, 0x2D, 0x53, 0x6E, 0x69,
            0x70, 0x20, 0x61, 0x62, 0x6F, 0x76, 0x65, 0x20, 0x68, 0x65, 0x72, 0x65, 0x20, 0x74, 0x6F,
            0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x20, 0x61, 0x20, 0x72, 0x61, 0x77, 0x20, 0x73,
            0x61, 0x76, 0x20, 0x62, 0x79, 0x20, 0x65, 0x78, 0x63, 0x6C, 0x75, 0x64, 0x69, 0x6E, 0x67,
            0x20, 0x74, 0x68, 0x69, 0x73, 0x20, 0x44, 0x65, 0x53, 0x6D, 0x75, 0x4D, 0x45, 0x20, 0x73,
            0x61, 0x76, 0x65, 0x64, 0x61, 0x74, 0x61, 0x20, 0x66, 0x6F, 0x6F, 0x74, 0x65, 0x72, 0x3A,
            0x11, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x2D, 0x44, 0x45, 0x53, 0x4D,
            0x55, 0x4D, 0x45, 0x20, 0x53, 0x41, 0x56, 0x45, 0x2D, 0x7C };

    // Check if the address to insert is within the file size
    if (addressToInsert < buffer.size()) {
        // Insert bytes after the specified address
        buffer.insert(buffer.begin() + addressToInsert + 1, bytesToInsert.begin(), bytesToInsert.end());

        // Create a new file name with the ".dsv" extension
        std::string outputFilePath = filePath;
        size_t lastDotPosition = outputFilePath.find_last_of('.');
        if (lastDotPosition != std::string::npos) {
            outputFilePath.erase(lastDotPosition);
        }
        outputFilePath += ".dsv";

        // Open the output file in binary mode
        std::ofstream outputFile(outputFilePath, std::ios::binary);

        if (!outputFile) {
            std::cerr << "Could not create the output file: " << outputFilePath << std::endl;
            return 1;
        }

        // Write the modified content back to the output file
        outputFile.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());

        // Close the output file
        outputFile.close();

        std::cout << "Bytes inserted after address 0xFFFF. Modified file created: " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "Address 0xFFFF is beyond the file size. Nothing to insert." << std::endl;
    }

    return 0;
}
