bash
Copy code
sudo apt-get install libmysqlcppconn-dev
对于macOS：

bash
Copy code
brew install mysql-connector-c++


```bash
mac@macs-MacBook-Pro-2 build % cmake ..                                              
CMake Error at CMakeLists.txt:8 (find_package):
  By not providing "FindMySQL.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "MySQL", but
  CMake did not find one.

  Could not find a package configuration file provided by "MySQL" with any of
  the following names:

    MySQLConfig.cmake
    mysql-config.cmake

  Add the installation prefix of "MySQL" to CMAKE_PREFIX_PATH or set
  "MySQL_DIR" to a directory containing one of the above files.  If "MySQL"
  provides a separate development package or SDK, be sure it has been
  installed.
```
