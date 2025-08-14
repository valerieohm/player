#include <types.hpp>
#include "source/fileio/fileio.hpp"
#include <iostream>

BitStream::BitStream(const std::string& filename) 
    : filename_(filename), file_size_(0), current_pos_(0) {
    open(filename);
}

BitStream::~BitStream() {
    close();
}

bool BitStream::open(const std::string& filename) {
    close(); // Close any existing file
    
    filename_ = filename;
    file_.open(filename_, std::ios::binary | std::ios::in);
    
    if (!file_.is_open()) {
        std::cerr << "Failed to open file: " << filename_ << std::endl;
        return false;
    }
    
    update_file_size();
    current_pos_ = 0;
    return true;
}

void BitStream::close() {
    if (file_.is_open()) {
        file_.close();
    }
    file_size_ = 0;
    current_pos_ = 0;
}

bool BitStream::is_open() const {
    return file_.is_open();
}

size_t BitStream::file_size() const {
    return file_size_;
}

size_t BitStream::current_position() const {
    return current_pos_;
}

bool BitStream::eof() const {
    return current_pos_ >= file_size_ || file_.eof();
}

size_t BitStream::read_chunk(std::vector<uint8_t>& buffer, size_t chunk_size) {
    if (!is_open() || eof()) {
        return 0;
    }
    
    buffer.resize(chunk_size);
    file_.read(reinterpret_cast<char*>(buffer.data()), chunk_size);
    
    size_t bytes_read = file_.gcount();
    buffer.resize(bytes_read); // Resize to actual bytes read
    current_pos_ += bytes_read;
    
    return bytes_read;
}

size_t BitStream::read_bytes(uint8_t* buffer, size_t num_bytes) {
    if (!is_open() || eof() || !buffer) {
        return 0;
    }
    
    file_.read(reinterpret_cast<char*>(buffer), num_bytes);
    size_t bytes_read = file_.gcount();
    current_pos_ += bytes_read;
    
    return bytes_read;
}

bool BitStream::seek(size_t position) {
    if (!file_.is_open()) {
        return false;
    }
    
    if (position > file_size_) {
        position = file_size_;
    }
    
    file_.clear(); // Clear any error flags (like EOF)
    file_.seekg(position, std::ios::beg);
    current_pos_ = position;
    
    return file_.good();
}

void BitStream::reset() {
    seek(0);
}

bool BitStream::write_to_file(const std::string& output_filename) {
    if (!is_open()) {
        std::cerr << "BitStream is not open for writing" << std::endl;
        return false;
    }
    
    std::ofstream output_file(output_filename, std::ios::binary | std::ios::out);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file: " << output_filename << std::endl;
        return false;
    }
    
    // Save current position
    size_t saved_pos = current_pos_;
    
    // Reset to beginning
    reset();
    
    // Copy all data
    const size_t buffer_size = 8192; // 8KB buffer
    std::vector<uint8_t> buffer;
    size_t total_written = 0;
    
    while (!eof()) {
        size_t bytes_read = read_chunk(buffer, buffer_size);
        if (bytes_read == 0) break;
        
        output_file.write(reinterpret_cast<const char*>(buffer.data()), bytes_read);
        if (!output_file.good()) {
            std::cerr << "Error writing to output file" << std::endl;
            output_file.close();
            seek(saved_pos); // Restore position
            return false;
        }
        total_written += bytes_read;
    }
    
    output_file.close();
    seek(saved_pos); // Restore original position
    
    std::cout << "Successfully wrote " << total_written << " bytes to " << output_filename << std::endl;
    return true;
}

bool BitStream::write_range_to_file(const std::string& output_filename, size_t start_pos, size_t length) {
    if (!is_open()) {
        std::cerr << "BitStream is not open for writing" << std::endl;
        return false;
    }
    
    if (start_pos >= file_size_) {
        std::cerr << "Start position is beyond file size" << std::endl;
        return false;
    }
    
    // Adjust length if it would go beyond file end
    if (start_pos + length > file_size_) {
        length = file_size_ - start_pos;
    }
    
    std::ofstream output_file(output_filename, std::ios::binary | std::ios::out);
    if (!output_file.is_open()) {
        std::cerr << "Failed to open output file: " << output_filename << std::endl;
        return false;
    }
    
    // Save current position
    size_t saved_pos = current_pos_;
    
    // Seek to start position
    if (!seek(start_pos)) {
        std::cerr << "Failed to seek to start position" << std::endl;
        output_file.close();
        return false;
    }
    
    // Write the specified range
    const size_t buffer_size = 8192; // 8KB buffer
    std::vector<uint8_t> buffer;
    size_t remaining = length;
    size_t total_written = 0;
    
    while (remaining > 0 && !eof()) {
        size_t chunk_size = std::min(remaining, buffer_size);
        size_t bytes_read = read_chunk(buffer, chunk_size);
        if (bytes_read == 0) break;
        
        output_file.write(reinterpret_cast<const char*>(buffer.data()), bytes_read);
        if (!output_file.good()) {
            std::cerr << "Error writing to output file" << std::endl;
            output_file.close();
            seek(saved_pos); // Restore position
            return false;
        }
        
        remaining -= bytes_read;
        total_written += bytes_read;
    }
    
    output_file.close();
    seek(saved_pos); // Restore original position
    
    std::cout << "Successfully wrote " << total_written << " bytes (range " << start_pos << "-" << (start_pos + total_written - 1) << ") to " << output_filename << std::endl;
    return true;
}

bool BitStream::copy_to_file(const std::string& output_filename) {
    return write_to_file(output_filename);
}

void BitStream::update_file_size() {
    if (!file_.is_open()) {
        file_size_ = 0;
        return;
    }
    
    // Save current position
    auto current = file_.tellg();
    
    // Seek to end to get file size
    file_.seekg(0, std::ios::end);
    file_size_ = file_.tellg();
    
    // Restore position
    file_.seekg(current);
}

