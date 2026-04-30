
#include <stdio.h>

int main() {
    int op;

    printf("MENU:\n");
    printf("1 - Miojo\n");
    printf("2 - Ensopado\n");
    printf("3 - Arroz\n");
    printf("4 - Feijão\n");
    printf("5 - Macarrão\n");

    printf("Escolha: ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            printf("Macarrão instantâneo\n");
            break;
        case 2:
            printf("Comida com caldo\n");
            break;
        case 3:
            printf("Arroz cozido\n");
            break;
        case 4:
            printf("Feijão temperado\n");
            break;
        case 5:
            printf("Macarrão tradicional\n");
            break;
        default:
            printf("Opção inválida\n");
    }

    return 0;
}