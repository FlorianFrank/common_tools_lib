# Common Tools Lib

Common tools lib is a C/C++ library that provides an abstraction layer for frequently used functions, e.g. sockets, file-handling, exception-handling, or logging. It provides an easy-to-use API to enable fast platform-independent development.  
The library can be configured so that only the required parts are compiled leading to a very small footprint. Overall the library is designed to be used on small embedded systems efficiently.

## Installation 

The easiest way to install the library is to download the latest release and simply link the library to your project.
There exist pre-compiled versions for Windows (x64), Linux (amd64, i1386, armhf), and MAC OS (x64).

Otherwise, the library can be compiled manually, by following commands:

#### On Linux and MAC OS
```shell
git clone git@github.com:FlorianFrank/common_tools_lib.git
cd common_tools_lib
./build_<architecture>.sh
```
#### On Windows
```shell
git clone git@github.com:FlorianFrank/common_tools_lib.git
cd common_tools_lib
./build_<architecture>.bat
```

This installation command results in a folder **build** where you can find the compiled library as well as the required header files for the differnt functions.

### Customization 

The library is designed in different modules which can be enabled or disabled before compiling. 
This allows to reduce the footprint by not compiling unnecessary code. This is an important step especially for embedded devices. 
You can enable or disable modules in the **common_tools_lib/config/build_config.cmake** file. 

```cmake
option(PIL_COMMUNICATION      "Enable UART and socket communication"       ON)
option(PIL_THREADING          "Enable multithreading support"              ON)
option(PIL_FILE_HANDLING      "Enable file handling support"               ON)
option(PIL_LOGGING            "Enable logging support"                     ON)
option(PIL_SHARED             "Build shared library"                       ON)
option(PIL_STATIC             "Build static library"                       ON)
option(PIL_UNIT_TESTING       "Enable Unit testing"                        ON)
option(PIL_CSV                "Enable CSV file support"                    ON)
option(PIL_CXX                "Enable C++ support"                         ON)
option(PIL_EXCEPTION_HANDLING "Enable exception handling in C++ version"   ON)
```

Within this config file not only the modules can be enabled or disabled. It allows also to configure if the library should be built as a shared or static lib. Additionally, the C++ interface can be enabled or disabled.

After changing the files, execute the build commands like described above. 

## Modules 

The library is decomposed into different modules which can be build independently. 
Some modules have dependencies, e.g. the callback functions in the socket implementation require the threading module to be enabled. Such warnings are displayed warnings during the compilation process.

### Communication

The communication module contains currently a socket as well as an UART implementation.
The UART as well as the socket implementation provide a simple API with less configuration methods to open a simple connection by one or two lines of code. There exists also an interface  

#### Socket

Sample code of a server socket using the C API and callback functions
```c
// Callback is triggered when 
void CallbackAccept(PIL_SOCKET sck, char* ip)
{
    
    if(PIL_SOCKET_Send(&sck, "Return message", &length) != PIL_NO_ERROR)
    {
        // Error Handling
    }
}

// Create a simple TCP server socket on port 8001
PIL_SOCKET srvSocket;
if(PIL_SOCKET_Setup_ServerSocket(&srvSocket, 8001,  CallbackAccept) != PIL_NO_ERROR)
{
    // Error Handling
}

```
<br>
Sample code of a server socket using the API:

```c
void ReceiveHandler(uint8_t *buffer, uint32_t len)
{
    // Receive data.
}

PIL_SOCKET clientSocket;
if(PIL_SOCKET_ConnectToServer(&clientSocket, "127.0.0.1", 15000, 14000, ReceiveHandler) != PIL_NO_ERROR)
{
    // Error Handling
}
```

### Logging

## Testing

|       OS       |                                                       Status                                                       | 
|:--------------:|:------------------------------------------------------------------------------------------------------------------:|
|Windows Latest  | ![Windows latest](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/windows_latest.yml/badge.svg) |
|Mac OS Latest   |  ![MAC OS latest](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/ubuntu_latest.yml/badge.svg)  |
|Ubuntu Latest   |  ![Ubuntu Latest](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/mac_os_latest.yml/badge.svg)  |


