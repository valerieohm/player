#define BOOST_TEST_MODULE BitstreamTests
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>
#include "source/fileio/fileio.hpp"

// Logging config for Boost.Test
struct LogConfig {
  LogConfig() {
    boost::unit_test::unit_test_log.set_stream(std::cout);
    boost::unit_test::unit_test_log.set_threshold_level(
        boost::unit_test::log_messages);
  }
} logConfig;

BOOST_AUTO_TEST_CASE(test_bitstream_basic_operations) {
  // Create a test input file
  std::string test_input = "test_input.txt";
  std::string test_content = "Hello, World! This is a test file for BitStream functionality.";
  
  // Write test content to input file
  std::ofstream input_file(test_input, std::ios::binary);
  input_file.write(test_content.c_str(), test_content.size());
  input_file.close();
  
  // Test BitStream basic operations
  BitStream bitstream(test_input);
  BOOST_REQUIRE(bitstream.is_open());
  BOOST_REQUIRE_EQUAL(bitstream.file_size(), test_content.size());
  BOOST_REQUIRE_EQUAL(bitstream.current_position(), 0);
  BOOST_REQUIRE(!bitstream.eof());
  
  // Test chunked reading
  std::vector<uint8_t> buffer;
  size_t bytes_read = bitstream.read_chunk(buffer, 5); // Read "Hello"
  BOOST_REQUIRE_EQUAL(bytes_read, 5);
  BOOST_REQUIRE_EQUAL(buffer.size(), 5);
  std::string chunk_content(buffer.begin(), buffer.end());
  BOOST_CHECK_EQUAL(chunk_content, "Hello");
  BOOST_CHECK_EQUAL(bitstream.current_position(), 5);
  
  // Test seek and reset
  BOOST_REQUIRE(bitstream.seek(7)); // Position at "World"
  BOOST_CHECK_EQUAL(bitstream.current_position(), 7);
  
  bytes_read = bitstream.read_chunk(buffer, 5); // Read "World"
  chunk_content = std::string(buffer.begin(), buffer.end());
  BOOST_CHECK_EQUAL(chunk_content, "World");
  
  // Test reset
  bitstream.reset();
  BOOST_CHECK_EQUAL(bitstream.current_position(), 0);
  
  // Clean up
  std::remove(test_input.c_str());
  
  BOOST_TEST_MESSAGE("BitStream basic operations tested successfully.");
}

BOOST_AUTO_TEST_CASE(test_bitstream_write_operations) {
  // Create a test input file
  std::string test_input = "test_input_write.txt";
  std::string test_content = "Hello, World! This is a test file for BitStream write functionality.";
  
  // Write test content to input file
  std::ofstream input_file(test_input, std::ios::binary);
  input_file.write(test_content.c_str(), test_content.size());
  input_file.close();
  
  // Test BitStream write functionality
  BitStream bitstream(test_input);
  BOOST_REQUIRE(bitstream.is_open());
  BOOST_REQUIRE_EQUAL(bitstream.file_size(), test_content.size());
  
  // Test write_to_file
  std::string output_file = "test_output.txt";
  BOOST_REQUIRE(bitstream.write_to_file(output_file));
  
  // Verify the full output
  std::ifstream verify_full(output_file, std::ios::binary);
  std::string full_content((std::istreambuf_iterator<char>(verify_full)),
                          std::istreambuf_iterator<char>());
  verify_full.close();
  BOOST_CHECK_EQUAL(full_content, test_content);
  
  // Test write_range_to_file
  std::string range_output = "test_range_output.txt";
  BOOST_REQUIRE(bitstream.write_range_to_file(range_output, 7, 5)); // Extract "World"
  
  // Verify the range output
  std::ifstream verify_file(range_output, std::ios::binary);
  std::string range_content((std::istreambuf_iterator<char>(verify_file)),
                           std::istreambuf_iterator<char>());
  verify_file.close();
  BOOST_CHECK_EQUAL(range_content, "World");
  
  // Test copy_to_file
  std::string copy_output = "test_copy_output.txt";
  BOOST_REQUIRE(bitstream.copy_to_file(copy_output));
  
  // Verify the copy output
  std::ifstream verify_copy(copy_output, std::ios::binary);
  std::string copy_content((std::istreambuf_iterator<char>(verify_copy)),
                          std::istreambuf_iterator<char>());
  verify_copy.close();
  BOOST_CHECK_EQUAL(copy_content, test_content);
  
  // Clean up test files
  std::remove(test_input.c_str());
  std::remove(output_file.c_str());
  std::remove(range_output.c_str());
  std::remove(copy_output.c_str());
  
  BOOST_TEST_MESSAGE("BitStream write functionality tested successfully.");
}

BOOST_AUTO_TEST_CASE(test_bitstream_edge_cases) {
  // Test with non-existent file
  BitStream invalid_stream("non_existent_file.txt");
  BOOST_CHECK(!invalid_stream.is_open());
  
  // Test empty file
  std::string empty_file = "empty_test.txt";
  std::ofstream create_empty(empty_file, std::ios::binary);
  create_empty.close();
  
  BitStream empty_stream(empty_file);
  BOOST_REQUIRE(empty_stream.is_open());
  BOOST_CHECK_EQUAL(empty_stream.file_size(), 0);
  BOOST_CHECK(empty_stream.eof());
  
  std::vector<uint8_t> buffer;
  size_t bytes_read = empty_stream.read_chunk(buffer, 10);
  BOOST_CHECK_EQUAL(bytes_read, 0);
  
  // Clean up
  std::remove(empty_file.c_str());
  
  BOOST_TEST_MESSAGE("BitStream edge cases tested successfully.");
}
