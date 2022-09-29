project(NanoPK CXX)
cmake_minimum_required(VERSION 3.14)

include(FindPkgConfig)

pkg_check_modules(LIBMODBUS REQUIRED libmodbus)
find_package(Boost REQUIRED COMPONENTS headers log log_setup)
find_package(mqtt_cpp_iface REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(nanopkif
    src/main.cpp
    src/modbus_nanopk.cpp
    src/modbus_nanopk.h
    src/dataentry.cpp
    src/dataentry.h
    src/nanopk_visualizer.h
)

target_compile_features(nanopkif 
  PUBLIC
    cxx_std_17
)

target_include_directories(nanopkif
 PRIVATE 
    ${LIBMODBUS_INCLUDE_DIRS}
)

target_link_libraries(nanopkif 
  ${LIBMODBUS_LIBRARIES}
  mqtt_cpp_iface::mqtt_cpp_iface
  Boost::log
  Boost::log_setup
)
