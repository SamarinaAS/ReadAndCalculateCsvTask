#include <calculate_csv.h>
#include <gtest/gtest.h>

void isTableEqual(std::vector<std::vector<std::string>> &tableData, std::vector<std::vector<std::string>> &standard) {
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

TEST(ReadCsv, CorrectRead) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/ReadCsv/correct_read.csv", tableData);
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell"}, {"1", "1", "0", "1"}, {"2", "2", "=A1+Cell30", "0"}, {"30", "0", "=B1+A1", "5"}};
    isTableEqual(tableData, standard);
}

TEST(ReadCsv, ExceptionInvalidFormatFile) {
    std::vector<std::vector<std::string>> tableData;
    EXPECT_THROW(
        {
            try {
                ReadCsv("../csv_files/ReadCsv/file.txt", tableData);
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
                ReadCsv("fej.csv", tableData);
            } catch (char const *str) {
                EXPECT_STREQ("Error: File could not open", str);
                throw;
            }
        },
        char const *);
}

TEST(CalculateCsv, Correct_1) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/CalculateTable/correct_calculate/1.csv", tableData);
    calculateTable(tableData);
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell"}, {"1", "1", "0", "1"}, {"2", "2", "6", "0"}, {"30", "0", "2", "5"}};
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

TEST(CalculateCsv, Correct_2) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/CalculateTable/correct_calculate/2.csv", tableData);
    calculateTable(tableData);
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell", "D"},
                                                   {"1", "1", "13", "1", "3"},
                                                   {"2", "2", "3", "0", "2"},
                                                   {"30", "7", "14", "8", "5"}};
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

TEST(CalculateCsv, Correct_3) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/CalculateTable/correct_calculate/3.csv", tableData);
    calculateTable(tableData);
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell", "D"}, {"1", "1", "2", "1", "1"}, {"2", "2", "6", "0", "2"}, {"30", "3", "3", "1", "3"}};
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

TEST(CalculateCsv, Correct_4) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/CalculateTable/correct_calculate/4.csv", tableData);
    calculateTable(tableData);
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell", "D"}, {"1", "5", "7", "1", "10"}, {"2", "5", "5", "0", "5"}, {"30", "3", "4", "1", "2"}};
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

TEST(CalculateCsv, Correct_5) {
    std::vector<std::vector<std::string>> tableData;
    ReadCsv("../csv_files/CalculateTable/correct_calculate/5.csv", tableData);
    calculateTable(tableData);
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell", "D"},
                                                   {"1", "5", "7", "3", "4"},
                                                   {"2", "2", "19", "3", "2"},
                                                   {"30", "12", "7", "3", "3"}};
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
