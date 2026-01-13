add_library(asio STATIC)

target_include_directories(asio PUBLIC asio-1.36.0/include)
target_sources(asio PUBLIC asio-1.36.0/src/asio.cpp)