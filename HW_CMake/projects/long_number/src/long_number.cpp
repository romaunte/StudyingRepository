#include "long_number.hpp"
#include <iostream>

using biv::LongNumber;

LongNumber::LongNumber() {
    numbers = nullptr;
    length = 0;
    sign = 0;  // 0 значит число равно нулю
}

LongNumber::LongNumber(const char* const str) {
    if (!str) {
        numbers = nullptr;
        length = 0;
        sign = 0;
        return;
    }

    int pos = 0;
    if (str[0] == '-') {
        sign = -1;
        ++pos;
    } else if (str[0] == '+') {
        sign = 1;
        ++pos;
    } else {
        sign = 1;
    }

    // Пропускаем ведущие нули
    while (str[pos] == '0' && str[pos + 1] != '\0') pos++;

    length = get_length(str);
    if (length == 0) {  // если строка пустая после удаления нулей
        numbers = nullptr;
        sign = 0;
        return;
    }

    numbers = new int[length];
    for (int i = 0; i < length; ++i) {
        numbers[i] = str[pos + length - 1 - i] - '0';  // цифры храним в обратном порядке
    }
}

LongNumber::LongNumber(const LongNumber& x) {
    length = x.length;
    sign = x.sign;
    if (length > 0) {
        numbers = new int[length];
        for (int i = 0; i < length; ++i) {
            numbers[i] = x.numbers[i];
        }
    } else {
        numbers = nullptr;
    }
}

LongNumber::LongNumber(LongNumber&& x) {
    numbers = x.numbers;
    length = x.length;
    sign = x.sign;

    x.numbers = nullptr;
    x.length = 0;
    x.sign = 0;
}

LongNumber::~LongNumber() {
    delete[] numbers;
    numbers = nullptr;
    length = 0;
    sign = 0;
}

LongNumber& LongNumber::operator = (const char* const str) {
    delete[] numbers;

    if (!str) {
        numbers = nullptr;
        length = 0;
        sign = 0;
        return *this;
    }

    int pos = 0;
    if (str[0] == '-') {
        sign = -1;
        ++pos;
    } else if (str[0] == '+') {
        sign = 1;
        ++pos;
    } else {
        sign = 1;
    }

    while (str[pos] == '0' && str[pos + 1] != '\0') pos++;

    length = get_length(str);
    if (length == 0) {
        numbers = nullptr;
        sign = 0;
        return *this;
    }

    numbers = new int[length];
    for (int i = 0; i < length; ++i) {
        numbers[i] = str[pos + length - 1 - i] - '0';
    }

    return *this;
}

LongNumber& LongNumber::operator = (const LongNumber& x) {
    if (this == &x) return *this;

    delete[] numbers;

    length = x.length;
    sign = x.sign;
    if (length > 0) {
        numbers = new int[length];
        for (int i = 0; i < length; ++i) {
            numbers[i] = x.numbers[i];
        }
    } else {
        numbers = nullptr;
    }

    return *this;
}

LongNumber& LongNumber::operator = (LongNumber&& x) {
    if (this != &x) {
        delete[] numbers;

        numbers = x.numbers;
        length = x.length;
        sign = x.sign;

        x.numbers = nullptr;
        x.length = 0;
        x.sign = 0;
    }
    return *this;
}

bool LongNumber::operator == (const LongNumber& x) const {
    if (length != x.length || sign != x.sign) return false;
    for (int i = 0; i < length; ++i) {
        if (numbers[i] != x.numbers[i]) return false;
    }
    return true;
}

bool LongNumber::operator != (const LongNumber& x) const {
    return !(*this == x);
}

bool LongNumber::operator > (const LongNumber& x) const {
    if (sign > x.sign) return true;
    if (sign < x.sign) return false;
    if (sign == 0) return false; // оба равны нулю

    // Сравниваем длину с учетом знака
    if (length > x.length) return (sign == 1);
    if (length < x.length) return (sign == -1);

    for (int i = length - 1; i >= 0; --i) {
        if (numbers[i] > x.numbers[i]) return (sign == 1);
        if (numbers[i] < x.numbers[i]) return (sign == -1);
    }
    return false;
}

bool LongNumber::operator < (const LongNumber& x) const {
    return !(*this > x || *this == x);
}

LongNumber LongNumber::operator + (const LongNumber& x) const {
    if (sign == 0) return x;
    if (x.sign == 0) return *this;

    if (sign == x.sign) {
        int max_len = (length > x.length) ? length : x.length;
        int* res = new int[max_len + 1]();
        int carry = 0;

        for (int i = 0; i < max_len; ++i) {
            int a = (i < length) ? numbers[i] : 0;
            int b = (i < x.length) ? x.numbers[i] : 0;
            int sum = a + b + carry;
            res[i] = sum % 10;
            carry = sum / 10;
        }
        int res_len = max_len;
        if (carry > 0) {
            res[res_len++] = carry;
        }

        LongNumber result;
        result.numbers = res;
        result.length = res_len;
        result.sign = sign;
        return result;
    } else {
        if (sign == -1) {
            LongNumber temp = *this;
            temp.sign = 1;
            return x - temp;
        } else {
            LongNumber temp = x;
            temp.sign = 1;
            return *this - temp;
        }
    }
}

LongNumber LongNumber::operator - (const LongNumber& x) const {
    if (sign == 0) {
        LongNumber temp = x;
        temp.sign = -temp.sign;
        return temp;
    }
    if (x.sign == 0) return *this;

    if (sign == x.sign) {
        if (*this == x) return LongNumber("0");

        bool bigger = false;
        if (length != x.length) {
            bigger = (length > x.length);
        } else {
            for (int i = length - 1; i >= 0; --i) {
                if (numbers[i] != x.numbers[i]) {
                    bigger = (numbers[i] > x.numbers[i]);
                    break;
                }
            }
        }

        const LongNumber* a = bigger ? this : &x;
        const LongNumber* b = bigger ? &x : this;

        int new_len = a->length;
        int* res = new int[new_len]();
        int borrow = 0;

        for (int i = 0; i < new_len; ++i) {
            int num1 = (i < a->length) ? a->numbers[i] : 0;
            int num2 = (i < b->length) ? b->numbers[i] : 0;
            int diff = num1 - num2 - borrow;

            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            res[i] = diff;
        }

        while (new_len > 1 && res[new_len - 1] == 0) new_len--;

        LongNumber result;
        result.numbers = res;
        result.length = new_len;
        result.sign = bigger ? sign : -sign;

        return result;
    } else {
        LongNumber temp = x;
        temp.sign = sign;
        return *this + temp;
    }
}

LongNumber LongNumber::operator * (const LongNumber& x) const {
    if (sign == 0 || x.sign == 0) return LongNumber("0");

    int new_len = length + x.length;
    int* res = new int[new_len]();

    for (int i = 0; i < length; ++i) {
        int carry = 0;
        for (int j = 0; j < x.length; ++j) {
            int prod = numbers[i] * x.numbers[j] + res[i + j] + carry;
            res[i + j] = prod % 10;
            carry = prod / 10;
        }
        res[i + x.length] += carry;
    }

    while (new_len > 1 && res[new_len - 1] == 0) new_len--;

    LongNumber result;
    result.numbers = res;
    result.length = new_len;
    result.sign = sign * x.sign;
    return result;
}

LongNumber LongNumber::operator / (const LongNumber& x) const {
    if (sign == 0) return LongNumber("0");

    LongNumber dividend = *this;
    dividend.sign = 1;
    LongNumber divisor = x;
    divisor.sign = 1;

    if (dividend < divisor) return LongNumber("0");

    LongNumber quotient("0");
    LongNumber remainder("0");
    LongNumber ten("10");

    for (int i = dividend.length - 1; i >= 0; --i) {
        remainder = remainder * ten;

        LongNumber now;
        now.numbers = new int[1]{ dividend.numbers[i] };
        now.length = 1;
        now.sign = 1;

        remainder = remainder + now;

        int digit = 0;
        while (remainder > divisor || remainder == divisor) {
            remainder = remainder - divisor;
            digit++;
        }

        quotient = quotient * ten;

        LongNumber digitNum;
        digitNum.numbers = new int[1]{ digit };
        digitNum.length = 1;
        digitNum.sign = 1;

        quotient = quotient + digitNum;
    }

    while (quotient.length > 1 && quotient.numbers[quotient.length - 1] == 0) {
        quotient.length--;
    }

    quotient.sign = sign * x.sign;
    return quotient;
}

LongNumber LongNumber::operator % (const LongNumber& x) const {
    if (sign == 0) return LongNumber("0");

    LongNumber div = *this / x;
    LongNumber res = *this - div * x;

    if (res < LongNumber("0")) {
        if (x > LongNumber("0")) res = res + x;
        else res = res - x;
    }

    return res;
}

int LongNumber::get_digits_number() const noexcept {
    return length;
}

int LongNumber::get_rank_number(int rank) const {
    if (rank < 1 || rank > length) return -1;
    return numbers[length - rank];
}

bool LongNumber::is_negative() const noexcept {
    return sign < 0;
}

// ----------------------------------------------------------
// PRIVATE
// ----------------------------------------------------------
int LongNumber::get_length(const char* const str) const noexcept {
    int pos = 0;
    if (str[0] == '-' || str[0] == '+') pos++;

    while (str[pos] == '0' && str[pos + 1] != '\0') pos++;

    int len = 0;
    for (int i = pos; str[i] != '\0'; ++i) ++len;

    return len;
}

// ----------------------------------------------------------
// FRIENDLY
// ----------------------------------------------------------
namespace biv {
    std::ostream& operator << (std::ostream& os, const LongNumber& x) {
        if (x.sign == -1) os << '-';
        if (x.length == 0) os << '0';
        else {
            for (int i = x.length - 1; i >= 0; --i) {
                os << x.numbers[i];
            }
        }
        return os;
    }
}
