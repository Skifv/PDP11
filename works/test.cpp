#include <stdio.h>

// Функция для вывода двоичного представления числа
void decimalToBinary(int n) {
    // Создаем массив для хранения двоичных цифр
    int binaryNum[32];
    
    // Инициализируем переменную для хранения индекса массива
    int i = 0;
    
    // Пока переданное число не равно нулю
    while (n > 0) {
        // Записываем остаток от деления на 2 в массив
        binaryNum[i] = n % 2;
        // Делаем целочисленное деление на 2
        n = n / 2;
        // Увеличиваем индекс массива
        i++;
    }
    
    // Выводим двоичное число в обратном порядке
    for (int j = i - 1; j >= 0; j--)
        printf("%d", binaryNum[j]);
}

int main() {
    int decimalNumber;
    
    decimalNumber = 0xFF << 8;
    printf("Двоичное представление: ");
    decimalToBinary(decimalNumber);
    printf("\n");
    printf("Десятичное представление: ");
    printf("%x", decimalNumber);
    printf("\n");
    return 0;
}
