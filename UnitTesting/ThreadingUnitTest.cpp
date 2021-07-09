//
// Created by florianfrank on 30.12.20.
//

#include <gtest/gtest.h> // google test
#include <string>

extern "C" {
#include <ctlib/Socket.h>
}

#include "ctlib/Socket.hpp"

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
}


/**
 * This test checks if an error code is returned when passing an invalid parameter,
 * like a nullptr or a range of 0.
 */
TEST(SocketTest_C, SimpleSocketTest)
{

    EXPECT_EQ(ret, PIL_NO_ERROR);

    ret = PIL_SOCKET_Close(&srvSocket);
    //Nonce nonce;
    EXPECT_EQ(ret, PIL_NO_ERROR);
}

TEST(SocketTest_CPP, SimpleSocketTest)
{

}

/**
 * @brief Main function executes all tests.
 */
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
