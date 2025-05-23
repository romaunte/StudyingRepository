#include "long_number.hpp"
#include <gtest/gtest.h>

using biv::LongNumber;

TEST(ModuloTest, Basic) {
    LongNumber a("10");
    LongNumber b("3");
    LongNumber mod = a % b;
    EXPECT_EQ(mod, LongNumber("1"));
}

// #include "long_number.hpp"
// #include <gtest/gtest.h>

// using biv::LongNumber;

// // Проверка конструктора по умолчанию
// TEST(LongNumberTest, DefaultConstructor) {
//     LongNumber num;
//     EXPECT_EQ(num.get_digits_number(), 0);
//     EXPECT_FALSE(num.is_negative());
// }

// // Конструктор из строки, проверка знака и цифр
// TEST(LongNumberTest, ConstructorFromString) {
//     LongNumber num1("12345");
//     EXPECT_EQ(num1.get_digits_number(), 5);
//     EXPECT_FALSE(num1.is_negative());
//     EXPECT_EQ(num1.get_rank_number(1), 5);
//     EXPECT_EQ(num1.get_rank_number(5), 1);

//     LongNumber num2("-0000123");
//     EXPECT_EQ(num2.get_digits_number(), 3);
//     EXPECT_TRUE(num2.is_negative());
//     EXPECT_EQ(num2.get_rank_number(1), 3);
//     EXPECT_EQ(num2.get_rank_number(3), 1);

//     LongNumber num3("0");
//     EXPECT_EQ(num3.get_digits_number(), 0);
//     EXPECT_FALSE(num3.is_negative());
// }

// // Проверка копирования и перемещения
// TEST(LongNumberTest, CopyAndMoveConstructor) {
//     LongNumber original("4567");
//     LongNumber copy = original;
//     EXPECT_EQ(copy, original);

//     LongNumber moved = std::move(original);
//     EXPECT_EQ(moved.get_digits_number(), 4);
//     EXPECT_EQ(moved.get_rank_number(1), 7);
// }

// // Присваивание из строки
// TEST(LongNumberTest, AssignmentFromString) {
//     LongNumber num;
//     num = "789";
//     EXPECT_EQ(num.get_digits_number(), 3);
//     EXPECT_EQ(num.get_rank_number(1), 9);
//     EXPECT_FALSE(num.is_negative());

//     num = "-42";
//     EXPECT_EQ(num.get_digits_number(), 2);
//     EXPECT_TRUE(num.is_negative());
//     EXPECT_EQ(num.get_rank_number(2), 4);
// }

// // Проверка операторов сравнения
// TEST(LongNumberTest, ComparisonOperators) {
//     LongNumber a("123");
//     LongNumber b("124");
//     LongNumber c("123");
//     LongNumber d("-123");

//     EXPECT_TRUE(a == c);
//     EXPECT_TRUE(a != b);
//     EXPECT_TRUE(b > a);
//     EXPECT_TRUE(d < a);
//     EXPECT_FALSE(a < c);
//     EXPECT_FALSE(a > c);
// }

// // Проверка арифметики сложения
// TEST(LongNumberTest, Addition) {
//     LongNumber a("123");
//     LongNumber b("877");
//     LongNumber sum = a + b;  // 1000
//     EXPECT_EQ(sum, LongNumber("1000"));

//     LongNumber c("-123");
//     LongNumber d("100");
//     LongNumber sum2 = c + d;  // -23
//     EXPECT_EQ(sum2, LongNumber("-23"));

//     LongNumber e("-50");
//     LongNumber f("-50");
//     LongNumber sum3 = e + f;  // -100
//     EXPECT_EQ(sum3, LongNumber("-100"));
// }

// // Проверка арифметики вычитания
// TEST(LongNumberTest, Subtraction) {
//     LongNumber a("1000");
//     LongNumber b("1");
//     LongNumber diff = a - b;  // 999
//     EXPECT_EQ(diff, LongNumber("999"));

//     LongNumber c("50");
//     LongNumber d("100");
//     LongNumber diff2 = c - d;  // -50
//     EXPECT_EQ(diff2, LongNumber("-50"));

//     LongNumber e("-100");
//     LongNumber f("-50");
//     LongNumber diff3 = e - f;  // -50
//     EXPECT_EQ(diff3, LongNumber("-50"));
// }

// // Проверка умножения
// TEST(LongNumberTest, Multiplication) {
//     LongNumber a("12");
//     LongNumber b("12");
//     LongNumber prod = a * b;  // 144
//     EXPECT_EQ(prod, LongNumber("144"));

//     LongNumber c("-3");
//     LongNumber d("5");
//     LongNumber prod2 = c * d;  // -15
//     EXPECT_EQ(prod2, LongNumber("-15"));

//     LongNumber e("-4");
//     LongNumber f("-4");
//     LongNumber prod3 = e * f;  // 16
//     EXPECT_EQ(prod3, LongNumber("16"));
// }

// // Проверка деления
// TEST(LongNumberTest, Division) {
//     LongNumber a("100");
//     LongNumber b("10");
//     LongNumber quot = a / b;  // 10
//     EXPECT_EQ(quot, LongNumber("10"));

//     LongNumber c("7");
//     LongNumber d("2");
//     LongNumber quot2 = c / d;  // 3 (целочисленное деление)
//     EXPECT_EQ(quot2, LongNumber("3"));

//     LongNumber e("-15");
//     LongNumber f("5");
//     LongNumber quot3 = e / f;  // -3
//     EXPECT_EQ(quot3, LongNumber("-3"));
// }

// // Проверка оператора остатка (%)
// TEST(LongNumberTest, Modulo) {
//     LongNumber a("10");
//     LongNumber b("3");
//     LongNumber mod = a % b;  // 1
//     EXPECT_EQ(mod, LongNumber("1"));

//     LongNumber c("-10");
//     LongNumber d("3");
//     LongNumber mod2 = c % d;  // 2
//     EXPECT_EQ(mod2, LongNumber("2"));

//     LongNumber e("10");
//     LongNumber f("-3");
//     LongNumber mod3 = e % f;  // 1
//     EXPECT_EQ(mod3, LongNumber("1"));
// }

