//
// Created by florianfrank on 30.12.20.
//

#include <gtest/gtest.h> // google test
#include <string>

extern "C"
{
#include "Socket.h"
}

bool acceptFlag = true;
std::string loram = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut "
                    "labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores "
                    "et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus.";

char recvBuff[4096];

void receiveHandler(uint8_t *buffer, uint32_t len)
{
    strcpy(recvBuff, (char*)buffer);
}


void CallbackAccept(PIL_SOCKET sck, char* ip)
{
    acceptFlag = true;
    uint32_t length = loram.length();
    PIL_SOCKET_Send(&sck, reinterpret_cast<uint8_t*>((char*)loram.c_str()), &length);
}


/**
 * This test checks if an error code is returned when passing an invalid parameter,
 * like a nullptr or a range of 0.
 */
TEST(SocketTest_C, SimpleSocketTest)
{

    PIL_SOCKET srvSocket;
    PIL_ERROR_CODE ret = PIL_SOCKET_Setup_ServerSocket(&srvSocket, 14000, CallbackAccept);
    EXPECT_EQ(ret,PIL_NO_ERROR);

    acceptFlag = false;

    PIL_SOCKET clientSocket;
    ret = PIL_SOCKET_ConnectToServer(&clientSocket, "127.0.0.1", 15000, 14000, NULL);
    EXPECT_EQ(ret,PIL_NO_ERROR);

    PIL_SOCKET_RegisterCallbackFunction(&clientSocket,receiveHandler);

    usleep(100000);
    EXPECT_EQ(acceptFlag, true);

    EXPECT_STREQ(loram.c_str(), recvBuff);

    ret = PIL_SOCKET_Close(&clientSocket);
    EXPECT_EQ(ret, PIL_NO_ERROR);

    ret = PIL_SOCKET_Close(&srvSocket);
    //Nonce nonce;
    EXPECT_EQ(ret, PIL_NO_ERROR);
}

/**
 * @brief Main function executes all tests.
 */
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
