#include <gtest/gtest.h> // google test
#include <string>
#include <ctlib/Exception.h>
#include "ctlib/Logging.h"

TEST(ExceptionTest, SimpleExceptionNoArgs){
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try
                     {
                         throw PIL::Exception(PIL_INVALID_ARGUMENTS);
                     }
                     catch(PIL::Exception &e)
                     {
                         // and this tests that it has the correct message
                         EXPECT_STREQ( "Exception: Invalid arguments", e.what() );
                         throw;
                     }
                 }, PIL::Exception );
}

TEST(ExceptionTest, SimpleExceptionAdditionalErrorMessage){
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try
                     {
                         throw PIL::Exception(PIL_INTERFACE_CLOSED, "Additional Error Message");
                     }
                     catch(PIL::Exception &e)
                     {
                         // and this tests that it has the correct message
                         EXPECT_STREQ( "Exception: Socket is closed (Additional Error Message)", e.what() );
                         throw;
                     }
                 }, PIL::Exception );
}

TEST(ExceptionTest, ExceptionFileNameLineNumberAdditionalMessage){
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try
                     {
                         throw PIL::Exception(PIL_INTERFACE_CLOSED, __FILENAME__, __LINE__, "Additional Message %d", 42);
                     }
                     catch(PIL::Exception &e)
                     {
                         // and this tests that it has the correct message
                         EXPECT_STREQ( "Exception (ExceptionUnitTest.cpp:43): Socket is closed (Additional Message 42)", e.what() );
                         throw;
                     }
                 }, PIL::Exception );
}

TEST(ExceptionTest, ExceptionFileNameLineNumberNoAdditionalMessage){
    // this tests _that_ the expected exception is thrown
    EXPECT_THROW({
                     try
                     {
                         throw PIL::Exception(PIL_INTERFACE_CLOSED, __FILENAME__, __LINE__);
                     }
                     catch(PIL::Exception &e)
                     {
                         // and this tests that it has the correct message
                         EXPECT_STREQ( "Exception (ExceptionUnitTest.cpp:59): Socket is closed", e.what() );
                         throw;
                     }
                 }, PIL::Exception );
}