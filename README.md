# Common Tools Lib
This repository is a collection of useful platform independent wrapper and helper files.

Installation on debian based operating systems:
```shell
./build_<architecture>.sh
dpkg -i common_tools_lib*.deb
```

### Communication
Contains C snippets for a platform independet implementation of sockets, uart, etc.

#### Socket

Sample code of a server socket using the API
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
Contains a platform independent logging implementation.

## Testing

|       OS       |                                                                Status                                                     | 
|:--------------:|:-------------------------------------------------------------------------------------------------------------------------:|
|Windows Latest  | ![Build Platform Independent Lib](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/cmake.yml/badge.svg) |
|Mac OS Latest   | ![Build Platform Independent Lib](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/cmake.yml/badge.svg) |
|Ubuntu Latest   | ![Build Platform Independent Lib](https://github.com/FlorianFrank/common_tools_lib/actions/workflows/cmake.yml/badge.svg) |


