#include <iostream>

#include <logger/logger.h>

int main() {
    auto &logger = algorithm::util::logger::get_global_logger();
    logger.info("Hello World.");

    return 0;
}