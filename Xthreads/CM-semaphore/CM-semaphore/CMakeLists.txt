# CMakeList.txt : CMake project for CM-semaphore, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CM-semaphore "CM-semaphore.cpp" "CM-semaphore.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CM-semaphore PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
