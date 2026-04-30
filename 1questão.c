#include <stdio.h>

int main() {
    char c;

    printf("Digite uma letra: ");
    scanf(" %c", &c);

    switch (c) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            printf("É vogal\n");
            break;
        default:
            printf("Não é vogal\n");
    }

    return 0;
}
