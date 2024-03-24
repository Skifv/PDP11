#include <stdio.h>

// ������� ��� ������ ��������� ������������� �����
void decimalToBinary(int n) {
    // ������� ������ ��� �������� �������� ����
    int binaryNum[32];
    
    // �������������� ���������� ��� �������� ������� �������
    int i = 0;
    
    // ���� ���������� ����� �� ����� ����
    while (n > 0) {
        // ���������� ������� �� ������� �� 2 � ������
        binaryNum[i] = n % 2;
        // ������ ������������� ������� �� 2
        n = n / 2;
        // ����������� ������ �������
        i++;
    }
    
    // ������� �������� ����� � �������� �������
    for (int j = i - 1; j >= 0; j--)
        printf("%d", binaryNum[j]);
}

int main() {
    int decimalNumber;
    
    decimalNumber = 0xFF << 8;
    printf("�������� �������������: ");
    decimalToBinary(decimalNumber);
    printf("\n");
    printf("���������� �������������: ");
    printf("%x", decimalNumber);
    printf("\n");
    return 0;
}
