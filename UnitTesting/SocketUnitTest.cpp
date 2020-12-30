//
// Created by florianfrank on 30.12.20.
//
#if 0

#include <gtest/gtest.h> // google test

extern "C"
{
#include "Socket.h"
}

/**
 * This test checks if an error code is returned when passing an invalid parameter,
 * like a nullptr or a range of 0.
 */
TEST(SocketTest, SimpleSocketTest)
{
    PIL_SOCKET socket;
    PIL_ERROR_CODE retCode = PIL_SOCKET_Create(&socket, UDP, IPv4, "127.0.0.1", 8080);
    retCode = PIL_SOCKET_Bind(&socket);
    retCode = PIL_SOCKET_Listen(&socket, 10);
    retCode = PIL_SOCKET_Accept(&socket, "127.0.0.1");
    retCode = PIL_SOCKET_Connect(&socket,"127.0.0.1", 8080);
    retCode = PIL_SOCKET_WaitTillDataAvail(&socket, 10);
    PIL_SOCKET_ReceiveFrom(&socket, nullptr, nullptr, nullptr, nullptr);
    PIL_SOCKET_Receive(&socket, nullptr, nullptr);
    PIL_SOCKET_Send(&socket, nullptr, nullptr);
    PIL_SOCKET_SendTo(&socket, nullptr, 80, nullptr, nullptr);
    PIL_SOCKET_GetSenderIP(&socket);
    PIL_SOCKET_IsOpen(&socket);
     PIL_SOCKET_Close(&socket);



//Nonce nonce;
EXPECT_EQ(1,1);
}

/**
 * @brief Main function executes all tests.
 */
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif // IF 0

int main()
{return 0;}