#define BOOST_TEST_MODULE PlayerTests
#include <boost/test/included/unit_test.hpp>
#include <fstream>
#include <memory>
#include "source/alsaout/alsaout.hpp" // Include the ALSA output header

// Logging config for Boost.Test
struct LogConfig {
  LogConfig() {
    boost::unit_test::unit_test_log.set_stream(std::cout);
    boost::unit_test::unit_test_log.set_threshold_level(
        boost::unit_test::log_messages);
  }
} logConfig;

BOOST_AUTO_TEST_CASE(test01_ingest_input) {
  alsaoutrun();
  BOOST_TEST_MESSAGE("ALSA output run successfully.");
}
