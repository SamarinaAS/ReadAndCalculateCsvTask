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

void isCalculateCsvCorrect(std::string fileName, std::vector<std::vector<std::string>> standard) {
    std::vector<std::vector<std::string>> tableData;
    readCsv(fileName, tableData);
    calculateTable(tableData);
    ASSERT_EQ(tableData.size(), standard.size());
    isTableEqual(tableData, standard);
}

void expectExcepiton(std::string fileName, const char *expectedException) {
    std::vector<std::vector<std::string>> tableData;
    ASSERT_THROW(
        {
            try {
                readCsv(fileName, tableData);
                calculateTable(tableData);
            } catch (const char *str) {
                EXPECT_STREQ(str, expectedException);
                throw;
            }
        },
        char const *);
}

TEST(ReadCsv, Correct) {
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell"}, {"1", "1", "0", "1"}, {"2", "2", "=A1+Cell30", "0"}, {"30", "0", "=B1+A1", "5"}};
    std::vector<std::vector<std::string>> tableData;
    readCsv("../csv_files/ReadCsv/correct_read.csv", tableData);

    isTableEqual(tableData, standard);
}

TEST(ReadCsvException, InvalidFormatFile) {
    expectExcepiton("../csv_files/ReadCsv/file.txt", "Error: invalid input. The file must be in csv format");
}

TEST(ReadCsvException, FileCouldNotOpen) { expectExcepiton("fej.csv", "Error: File could not open"); }

TEST(CalculateCsvCorrect, 1) {
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell"}, {"1", "1", "0", "1"}, {"2", "2", "0", "0"}, {"30", "0", "2", "5"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/1.csv", standard);
}

TEST(CalculateCsvCorrect, 2) {
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell", "D"}, {"1", "1", "0", "1", "3"}, {"2", "2", "3", "0", "2"}, {"30", "1", "0", "2", "-1"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/2.csv", standard);
}

TEST(CalculateCsvCorrect, 3) {
    std::vector<std::vector<std::string>> standard{
        {"", "A", "B", "Cell", "D"}, {"1", "1", "2", "1", "1"}, {"2", "2", "6", "0", "2"}, {"30", "3", "2", "1", "1"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/3.csv", standard);
}

TEST(CalculateCsvCorrect, 4) {
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell", "D"},
                                                   {"1", "0", "-1", "1", "0"},
                                                   {"2", "-3", "5", "0", "5"},
                                                   {"30", "3", "4", "1", "2"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/4.csv", standard);
}

TEST(CalculateCsvCorrect, 5) {
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell", "D"},
                                                   {"1", "6", "7", "4", "0"},
                                                   {"2", "2", "13", "0", "2"},
                                                   {"30", "13", "0", "6", "-1"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/5.csv", standard);
}

TEST(CalculateCsvCorrect, 6) {
    std::vector<std::vector<std::string>> standard{{"", "A", "B", "Cell", "D"},
                                                   {"1", "1", "-2", "0", "0"},
                                                   {"2", "-1", "3", "3", "10"},
                                                   {"30", "11", "23", "23", "23"},
                                                   {"4", "21", "42", "46", "23"}};
    isCalculateCsvCorrect("../csv_files/CalculateTable/correct_calculate/6.csv", standard);
}

TEST(ExceptionTableIsSetIncorrectly, 1) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/1.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 2) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/2.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 3) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/3.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 4) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/4.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 5) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/5.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 6) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/6.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 7) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/7.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 8) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/8.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 9) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/9.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 10) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/10.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 11) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/11.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 12) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/12.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 13) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/13.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 14) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/14.csv", "Error: table is set incorrectly");
}

TEST(ExceptionTableIsSetIncorrectly, 15) {
    expectExcepiton("../csv_files/CalculateTable/incorrect_table/15.csv", "Error: table is set incorrectly");
}

TEST(ExceptionReferenceToNonExistenetCell, 1) {
    expectExcepiton("../csv_files/CalculateTable/reference_to_a_non_existent_cell/1.csv",
                    "Error: reference to a non-existent cell");
}

TEST(ExceptionReferenceToNonExistenetCell, 2) {
    expectExcepiton("../csv_files/CalculateTable/reference_to_a_non_existent_cell/2.csv",
                    "Error: reference to a non-existent cell");
}

TEST(ExceptionReferenceToNonExistenetCell, 3) {
    expectExcepiton("../csv_files/CalculateTable/reference_to_a_non_existent_cell/3.csv",
                    "Error: reference to a non-existent cell");
}

TEST(ExceptionLoopingFormulas, 1) {
    expectExcepiton("../csv_files/CalculateTable/looping_of_formulas/1.csv",
                    "Error: unable to calculate formulas in the table, there was a looping of formulas");
}

TEST(ExceptionLoopingFormulas, 2) {
    expectExcepiton("../csv_files/CalculateTable/looping_of_formulas/2.csv",
                    "Error: unable to calculate formulas in the table, there was a looping of formulas");
}

TEST(ExceptionDivisionByZero, 1) {
    expectExcepiton("../csv_files/CalculateTable/division_by_zero/1.csv", "Error: division by zero");
}

TEST(ExceptionDivisionByZero, 2) {
    expectExcepiton("../csv_files/CalculateTable/division_by_zero/2.csv", "Error: division by zero");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
