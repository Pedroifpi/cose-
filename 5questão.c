#include <stdio.h>

int main() {
    float a, b;
    char op;

    printf("Digite (ex: 10 + 2): ");
    scanf("%f %c %f", &a, &op, &b);

    switch (op) {
        case '+':
            printf("Resultado: %.2f\n", a + b);
            break;
        case '-':
            printf("Resultado: %.2f\n", a - b);
            break;
        case '*':
            printf("Resultado: %.2f\n", a * b);
            break;
        case '/':
            if (b != 0)
                printf("Resultado: %.2f\n", a / b);
            else
                printf("Erro: divisão por zero\n");
            break;
        default:
            printf("Operador inválido\n");
    }

    return 0;
}
