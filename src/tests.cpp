#include <calculate_csv.h>
#include <gtest/gtest.h>

TEST(ReadCsv, CorrectRead) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/input.csv", tableData);
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell"},
                                                   {"1", "1", "0", "1"},
                                                   {"2", "2", "=A1+Cell30", "0"},
                                                   {"30", "0", "=B1+A1", "5"}};
    ASSERT_EQ(tableData.size(), standard.size());
    for (long unsigned i = 0; i < tableData.size(); i++) {
        ASSERT_EQ(tableData[i].size(), standard[i].size());
    }
    for (long unsigned i = 0; i < tableData.size(); i++) {
        for (long unsigned j = 0; j < tableData[i].size(); j++) {
            ASSERT_EQ(tableData[i][j], standard[i][j]);
        }
    }
}

TEST(ReadCsv, ExceptionInvalidFromatFile) {
    std::vector<std::vector<std::string>> tableData;
    EXPECT_THROW(
        {
            try {
                ReadCsv("../csv_files/input.txt", tableData);
            } catch (char const *str) {
                EXPECT_STREQ("Error: invalid input. The file must be in csv format", str);
                throw;
            }
        },
        char const *);
}

TEST(ReadCsv, ExceptionFileCouldNotOpen) {
    std::vector<std::vector<std::string>> tableData;
    EXPECT_THROW(
        {
            try {
                ReadCsv("../csv_files/inputt.csv", tableData);
            } catch (char const *str) {
                EXPECT_STREQ("Error: File could not open", str);
                throw;
            }
        },
        char const *);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
