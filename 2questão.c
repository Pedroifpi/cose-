#include <stdio.h>

int main() {
    char c;

    printf("Digite um caractere: ");
    scanf(" %c", &c);

    switch (c >= 'a' && c <= 'z') {
        case 1:
            printf("É letra minúscula\n");
            break;
        default:
            printf("Não é letra minúscula\n");
    }

    return 0;
}
