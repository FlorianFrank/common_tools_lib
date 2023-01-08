/**
 * @copyright University of Passau - Chair of Computer Engineering
 * @author Florian Frank
 */
#include <gtest/gtest.h> // google test
#include <string>

extern "C" {
#include <ctlib/Threading.h>
}

TEST(ThreadingC, SimpleThreadingTest) {

    ThreadHandle threadHandle;
    int testVariable = 42;
    auto retValue = PIL_THREADING_CreateThread(&threadHandle, [](void* arg) -> void* {
        auto var = reinterpret_cast<int*>(arg);
        EXPECT_EQ(*var, 42);
        return nullptr;
        }, &testVariable);
    EXPECT_EQ(retValue, PIL_NO_ERROR);
    retValue = PIL_THREADING_RunThread(&threadHandle, FALSE);
    EXPECT_EQ(retValue, PIL_NO_ERROR);
    retValue = PIL_THREADING_JoinThread(&threadHandle, nullptr);
    EXPECT_EQ(retValue, PIL_NO_ERROR);
}
/**
 * @brief Main function executes all tests.
 */
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
