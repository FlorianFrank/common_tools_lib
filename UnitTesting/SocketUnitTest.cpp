//
// Created by florianfrank on 30.12.20.
//

#include <gtest/gtest.h> // google test
#include <string>

#include "ctlib/Socket.hpp"
#include <thread>
#include <chrono>

extern "C" {
#include "ctlib/Socket.h"
}


bool acceptFlag = true;
static std::string loram_ipsum = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut "
                    "labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores "
                    "et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus.";

char recvBuff[4096];

void ReceiveHandlerC(PIL_SOCKET *socket, uint8_t* buffer, uint32_t len, void*)
{
    strncpy(recvBuff, (char*)buffer, len);
}


void ReceiveHandlerCPP(std::unique_ptr<PIL::Socket> &socket, std::string& buffer)
{
    strncpy(recvBuff, (char*)buffer.c_str(), buffer.length());
}


void CallbackAcceptCPP(std::unique_ptr<PIL::Socket> &sck)
{
    acceptFlag = true;
    sck->Send(loram_ipsum);
}

void CallbackAcceptC(PIL_SOCKET sock, char* ipAddr)
{
    acceptFlag = true;
    uint32_t len = loram_ipsum.length();
    PIL_SOCKET_Send(&sock, (const uint8_t*)loram_ipsum.c_str(), &len);
}



/**
 * This test checks if an error code is returned when passing an invalid parameter,
 * like a nullptr or a range of 0.
 */
TEST(SocketTest_C, SimpleSocketTest)
{

    PIL_SOCKET srvSocket;
    PIL_ERROR_CODE ret = PIL_SOCKET_Setup_ServerSocket(&srvSocket, 14000, CallbackAcceptC);
    EXPECT_EQ(ret,PIL_NO_ERROR);

    acceptFlag = false;

    PIL_SOCKET clientSocket;
    ret = PIL_SOCKET_ConnectToServer(&clientSocket, "127.0.0.1", 15000, 14000, 1000, ReceiveHandlerC, nullptr);
    EXPECT_EQ(ret, PIL_NO_ERROR);

    std::this_thread::sleep_for(std::chrono::microseconds(100000));
    EXPECT_EQ(acceptFlag, true);

    EXPECT_STREQ(loram_ipsum.c_str(), recvBuff);

    ret = PIL_SOCKET_Close(&clientSocket);
    EXPECT_EQ(ret, PIL_NO_ERROR);

    ret = PIL_SOCKET_Close(&srvSocket);
    //Nonce nonce;
    EXPECT_EQ(ret, PIL_NO_ERROR);
}

TEST(SocketTest_CPP, SimpleSocketTest)
{
    PIL::Socket srvSock(TCP, IPv4, "localhost", 14002, 1000);
    std::function<void(std::unique_ptr<PIL::Socket>&)> callbackFunct(CallbackAcceptCPP);
    bool ret = srvSock.CreateServerSocket(callbackFunct);
    EXPECT_EQ(ret, PIL_NO_ERROR);
    PIL::Socket clientSock(TCP, IPv4, "localhost", 14003, 1000);
    std::string ipAddr = "127.0.0.1";
    std::function<void(std::unique_ptr<PIL::Socket>& , std::string &)> callbackFunc = ReceiveHandlerCPP;
    ret = clientSock.ConnectToServer(ipAddr, 14002, callbackFunc);
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
