#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    // Check if a file path was provided as a command-line argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    const std::string filePath = argv[1];

    // Check if the file has the ".sav" extension
    std::string extension = filePath.substr(filePath.find_last_of('.') + 1);
    if (extension != "dsv") {
        std::cerr << "The provided file does not have a '.dsv' extension." << std::endl;
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

    const std::streampos addressToRemove = 0xFFFF;

    // Check if the address to remove is within the file size
    if (addressToRemove < buffer.size()) {
        // Remove bytes after the specified address
        buffer.erase(buffer.begin() + addressToRemove + 1, buffer.end());

        // Create a new file name with the ".sav" extension
        std::string outputFilePath = filePath;
        size_t lastDotPosition = outputFilePath.find_last_of('.');
        if (lastDotPosition != std::string::npos) {
            outputFilePath.erase(lastDotPosition);
        }
        outputFilePath += ".sav";

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

        std::cout << "Bytes after address 0xFFFF successfully deleted. Modified file created: " << outputFilePath << std::endl;
    }
    else {
        std::cerr << "Address 0xFFFF is beyond the file size. Nothing to delete." << std::endl;
    }

    return 0;
}
