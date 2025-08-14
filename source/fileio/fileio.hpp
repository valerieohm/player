#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>

class BitStream {
public:
    explicit BitStream(const std::string& filename);
    ~BitStream();
    
    // Open a file for reading
    bool open(const std::string& filename);
    
    // Close the file
    void close();
    
    // Check if file is open and ready
    bool is_open() const;
    
    // Get file size in bytes
    size_t file_size() const;
    
    // Get current position in the file
    size_t current_position() const;
    
    // Check if we've reached end of file
    bool eof() const;
    
    // Read a chunk of data (returns number of bytes actually read)
    size_t read_chunk(std::vector<uint8_t>& buffer, size_t chunk_size);
    
    // Read a specific number of bytes into provided buffer
    size_t read_bytes(uint8_t* buffer, size_t num_bytes);
    
    // Seek to a specific position in the file
    bool seek(size_t position);
    
    // Reset to beginning of file
    void reset();
    
    // Write current bitstream to output file
    bool write_to_file(const std::string& output_filename);
    
    // Write a specific range of bytes to output file
    bool write_range_to_file(const std::string& output_filename, size_t start_pos, size_t length);
    
    // Copy entire bitstream to another file
    bool copy_to_file(const std::string& output_filename);

private:
    std::ifstream file_;
    std::string filename_;
    size_t file_size_;
    mutable size_t current_pos_;
    
    void update_file_size();
};

#endif

