#include <gtest/gtest.h> // google test
#include <ctlib/FileHandler.hpp>

TEST(FileHandlingTestCpp, FileExists){
    std::string fileName = "UnitTestFile.txt";
    PIL::FileHandler fileHandler(fileName, READ_WRITE, CREATE_TRUNCATE);
    std::string sampleText = "This is a sample text";
    auto ret = fileHandler.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    ret = fileHandler.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);

    std::string retText;
    ret = fileHandler.ReadFile(&retText);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    ASSERT_STREQ((sampleText + sampleText).c_str(), retText.c_str());
}

TEST(FileHandlingTestCpp, FileTruncate){
    std::string fileName = "FileTruncate.txt";
    PIL::FileHandler fileHandler(fileName, READ_WRITE, CREATE_TRUNCATE);
    std::string sampleText = "This is a sample text";
    auto ret = fileHandler.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    PIL::FileHandler fileHandler2(fileName, READ_WRITE, CREATE_TRUNCATE);
    ret = fileHandler2.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);


    std::string retText;
    ret = fileHandler.ReadFile(&retText);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    ASSERT_STREQ(sampleText.c_str(), retText.c_str());
}

TEST(FileHandlingTestCpp, FileAppend){
    std::string fileName = "FileTruncate.txt";
    PIL::FileHandler fileHandler(fileName, READ_WRITE, CREATE_TRUNCATE);
    std::string sampleText = "This is a sample text";
    auto ret = fileHandler.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    PIL::FileHandler fileHandler2(fileName, READ_WRITE, CREATE_APPEND);
    ret = fileHandler2.WriteFile(sampleText, true);
    ASSERT_EQ(PIL_NO_ERROR, ret);

    std::string retText;
    ret = fileHandler.ReadFile(&retText);
    ASSERT_EQ(PIL_NO_ERROR, ret);
    ASSERT_STREQ((sampleText+sampleText).c_str(), retText.c_str());
}

#ifndef __WIN32__
TEST(FileHandlingTestCpp, ListFilesDirectoriesOnly){
    std::string path = ".";
    PIL_FileListElem listOfFiles;
    auto ret = PIL::FileHandler::ListFilesInDirectory(path, DIRECTORIES, &listOfFiles, false);
    ASSERT_EQ(ret, PIL_NO_ERROR);
    auto currentElem = &listOfFiles;
    ASSERT_EQ(nullptr, currentElem->previous);
    ASSERT_NE(nullptr, currentElem->next);
    while(currentElem)
    {
        if(currentElem->handle)
            ASSERT_EQ(currentElem->handle->type, DIRECTORIES);
        currentElem = currentElem->next;
    }
}

TEST(FileHandlingTestCpp, ListFilesFilesOnly){
    std::string path = ".";
    PIL_FileListElem listOfFiles;
    auto ret = PIL::FileHandler::ListFilesInDirectory(path, REGULAR_FILES, &listOfFiles, false);
    ASSERT_EQ(ret, PIL_NO_ERROR);
    auto currentElem = &listOfFiles;
    ASSERT_FALSE(currentElem->previous);
    ASSERT_TRUE(currentElem->next);
    while(currentElem)
    {
        if(currentElem->handle){
            printf("%s\n", currentElem->handle->path);
            ASSERT_EQ(currentElem->handle->type, REGULAR_FILES);
        }
        currentElem = currentElem->next;
    }
}
#endif // __WIN32__