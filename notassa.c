#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 10
#define MAX_DISCIPLINAS 5
#define MAX_NOME 50

// --- CORES ANSI ---
#define COR_RESET   "\033[0m"
#define COR_TITULO  "\033[1;34m" // Azul Negrito
#define COR_SUCESSO "\033[1;32m" // Verde
#define COR_ERRO    "\033[1;31m" // Vermelho
#define COR_AVISO   "\033[1;33m" // Amarelo
#define COR_DESTINU "\033[1;36m" // Ciano

// --- ESTRUTURAS DE DADOS ---
typedef struct {
    int id;
    char nome[MAX_NOME];
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
} Disciplina;

// Estrutura para cada relação Aluno x Disciplina
typedef struct {
    float n1;        // -1.0f = Sem Nota Lançada
    float n2;        // -1.0f = Sem Nota Lançada
    int matriculado; // 1 = Sim, 0 = Não
} Avaliacao;

// --- VARIÁVEIS GLOBAIS ---
Aluno alunos[MAX_ALUNOS];
Disciplina disciplinas[MAX_DISCIPLINAS];
Avaliacao notas[MAX_ALUNOS][MAX_DISCIPLINAS];

int totalAlunos = 0;
int totalDisciplinas = 0;

// --- PROTÓTIPOS ---
void limpaTela();
void pausa();
void inicializarSistema();
void exibirCabecalho(const char *titulo);

void cadastrarAluno();
void cadastrarDisciplina();
void matricularAluno();
void lancarNotas();
void listarBoletimAluno();
void listarMatrizGeral();

// --- FUNÇÃO PRINCIPAL ---
int main() {
    int opcao;
    inicializarSistema();

    do {
        limpaTela();
        exibirCabecalho("MENU PRINCIPAL");
        
        printf("  [1] Cadastrar Aluno\n");
        printf("  [2] Cadastrar Disciplina\n");
        printf("  [3] Matricular Aluno em Disciplina\n");
        printf("  [4] Lançar Notas (Nota 1 e Nota 2)\n");
        printf("  [5] Consultar Boletim de um Aluno (Com Status)\n");
        printf("  [6] Visão Geral da Matriz Acadêmica\n");
        printf("  [0] Sair do Sistema\n");
        printf("--------------------------------------------------------\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1: cadastrarAluno(); break;
            case 2: cadastrarDisciplina(); break;
            case 3: matricularAluno(); break;
            case 4: lancarNotas(); break;
            case 5: listarBoletimAluno(); break;
            case 6: listarMatrizGeral(); break;
            case 0: 
                limpaTela();
                printf(COR_SUCESSO "\nSaindo do Sistema Acadêmico... Até logo!\n\n" COR_RESET);
                break;
            default:
                printf(COR_ERRO "\n[Opção Inválida!] Tente novamente.\n" COR_RESET);
                pausa();
                break;
        }
    } while (opcao != 0);

    return 0;
}

// --- FUNÇÕES UTILITÁRIAS ---

void limpaTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausa() {
    printf(COR_AVISO "\nPressione ENTER para continuar..." COR_RESET);
    getchar();
}

void inicializarSistema() {
    for (int i = 0; i < MAX_ALUNOS; i++) {
        for (int j = 0; j < MAX_DISCIPLINAS; j++) {
            notas[i][j].n1 = -1.0f;
            notas[i][j].n2 = -1.0f;
            notas[i][j].matriculado = 0;
        }
    }
}

void exibirCabecalho(const char *titulo) {
    printf(COR_TITULO "========================================================\n");
    printf("               SISTEMA ACADÊMICO - TUI                  \n");
    printf("========================================================\n" COR_RESET);
    printf(COR_DESTINU " >>> %s <<<\n" COR_RESET, titulo);
    printf("--------------------------------------------------------\n");
}

// --- MÓDULOS DO SISTEMA ---

void cadastrarAluno() {
    limpaTela();
    exibirCabecalho("CADASTRO DE ALUNO");

    if (totalAlunos >= MAX_ALUNOS) {
        printf(COR_ERRO "[Erro] Limite máximo de alunos (%d) atingido!\n" COR_RESET, MAX_ALUNOS);
        pausa();
        return;
    }

    alunos[totalAlunos].id = totalAlunos;
    printf("Nome do Aluno: ");
    fgets(alunos[totalAlunos].nome, MAX_NOME, stdin);
    alunos[totalAlunos].nome[strcspn(alunos[totalAlunos].nome, "\n")] = '\0';

    if (strlen(alunos[totalAlunos].nome) > 0) {
        printf(COR_SUCESSO "\n-> Aluno '%s' cadastrado com ID [%d]!\n" COR_RESET, alunos[totalAlunos].nome, totalAlunos);
        totalAlunos++;
    } else {
        printf(COR_ERRO "\n[Erro] O nome do aluno não pode ser vazio.\n" COR_RESET);
    }

    pausa();
}

void cadastrarDisciplina() {
    limpaTela();
    exibirCabecalho("CADASTRO DE DISCIPLINA");

    if (totalDisciplinas >= MAX_DISCIPLINAS) {
        printf(COR_ERRO "[Erro] Limite máximo de disciplinas (%d) atingido!\n" COR_RESET, MAX_DISCIPLINAS);
        pausa();
        return;
    }

    disciplinas[totalDisciplinas].id = totalDisciplinas;
    printf("Nome da Disciplina: ");
    fgets(disciplinas[totalDisciplinas].nome, MAX_NOME, stdin);
    disciplinas[totalDisciplinas].nome[strcspn(disciplinas[totalDisciplinas].nome, "\n")] = '\0';

    if (strlen(disciplinas[totalDisciplinas].nome) > 0) {
        printf(COR_SUCESSO "\n-> Disciplina '%s' cadastrada com ID [%d]!\n" COR_RESET, disciplinas[totalDisciplinas].nome, totalDisciplinas);
        totalDisciplinas++;
    } else {
        printf(COR_ERRO "\n[Erro] O nome não pode ser vazio.\n" COR_RESET);
    }

    pausa();
}

void matricularAluno() {
    limpaTela();
    exibirCabecalho("MATRICULAR ALUNO");

    if (totalAlunos == 0 || totalDisciplinas == 0) {
        printf(COR_ERRO "[Erro] Cadastre pelo menos 1 aluno e 1 disciplina primeiro.\n" COR_RESET);
        pausa();
        return;
    }

    printf(COR_AVISO "Alunos Cadastrados:\n" COR_RESET);
    for (int i = 0; i < totalAlunos; i++) {
        printf("  ID [%d] - %s\n", alunos[i].id, alunos[i].nome);
    }

    printf(COR_AVISO "\nDisciplinas Cadastradas:\n" COR_RESET);
    for (int j = 0; j < totalDisciplinas; j++) {
        printf("  ID [%d] - %s\n", disciplinas[j].id, disciplinas[j].nome);
    }
    printf("--------------------------------------------------------\n");

    int idAluno, idDisc;
    printf("Digite o ID do Aluno: ");
    scanf("%d", &idAluno);
    printf("Digite o ID da Disciplina: ");
    scanf("%d", &idDisc);
    getchar();

    if (idAluno >= 0 && idAluno < totalAlunos && idDisc >= 0 && idDisc < totalDisciplinas) {
        notas[idAluno][idDisc].matriculado = 1;
        printf(COR_SUCESSO "\n-> '%s' matriculado em '%s' com sucesso!\n" COR_RESET, alunos[idAluno].nome, disciplinas[idDisc].nome);
    } else {
        printf(COR_ERRO "\n[Erro] ID de Aluno ou Disciplina inválido!\n" COR_RESET);
    }

    pausa();
}

void lancarNotas() {
    limpaTela();
    exibirCabecalho("LANÇAMENTO DE NOTAS");

    if (totalAlunos == 0 || totalDisciplinas == 0) {
        printf(COR_ERRO "[Erro] Não há alunos ou disciplinas cadastradas.\n" COR_RESET);
        pausa();
        return;
    }

    int idAluno, idDisc, numNota;
    float notaValor;

    printf("ID do Aluno: ");
    scanf("%d", &idAluno);
    printf("ID da Disciplina: ");
    scanf("%d", &idDisc);

    if (idAluno >= 0 && idAluno < totalAlunos && idDisc >= 0 && idDisc < totalDisciplinas) {
        if (!notas[idAluno][idDisc].matriculado) {
            printf(COR_ERRO "\n[Aviso] O aluno não está matriculado nessa disciplina!\n" COR_RESET);
            getchar();
            pausa();
            return;
        }

        printf("Qual nota deseja lançar? [1 para Nota 1 / 2 para Nota 2]: ");
        scanf("%d", &numNota);

        if (numNota != 1 && numNota != 2) {
            printf(COR_ERRO "\n[Erro] Escolha 1 para Nota 1 ou 2 para Nota 2!\n" COR_RESET);
            getchar();
            pausa();
            return;
        }

        printf("Digite o valor da Nota %d (0.0 a 10.0): ", numNota);
        scanf("%f", &notaValor);
        getchar();

        if (notaValor >= 0.0f && notaValor <= 10.0f) {
            if (numNota == 1) {
                notas[idAluno][idDisc].n1 = notaValor;
            } else {
                notas[idAluno][idDisc].n2 = notaValor;
            }
            printf(COR_SUCESSO "\n-> Nota %d (%.1f) salva para %s em %s!\n" COR_RESET, 
                   numNota, notaValor, alunos[idAluno].nome, disciplinas[idDisc].nome);
        } else {
            printf(COR_ERRO "\n[Erro] A nota deve estar no intervalo de 0.0 a 10.0.\n" COR_RESET);
        }
    } else {
        printf(COR_ERRO "\n[Erro] IDs inválidos.\n" COR_RESET);
        getchar();
    }

    pausa();
}

void listarBoletimAluno() {
    limpaTela();
    exibirCabecalho("BOLETIM INDIVIDUAL DO ALUNO");

    if (totalAlunos == 0) {
        printf(COR_ERRO "Nenhum aluno cadastrado no sistema.\n" COR_RESET);
        pausa();
        return;
    }

    int idAluno;
    printf("Digite o ID do Aluno para gerar o Boletim: ");
    scanf("%d", &idAluno);
    getchar();

    if (idAluno < 0 || idAluno >= totalAlunos) {
        printf(COR_ERRO "\n[Erro] Aluno com este ID não existe.\n" COR_RESET);
        pausa();
        return;
    }

    limpaTela();
    printf(COR_TITULO "===================================================================\n");
    printf(" BOLETIM ACADÊMICO - ALUNO: %s\n", alunos[idAluno].nome);
    printf("===================================================================\n" COR_RESET);
    printf("%-20s | %-7s | %-7s | %-7s | %-12s\n", "Disciplina", "Nota 1", "Nota 2", "Média", "Situação");
    printf("-------------------------------------------------------------------\n");

    int disciplinasCursadas = 0;

    for (int j = 0; j < totalDisciplinas; j++) {
        if (notas[idAluno][j].matriculado) {
            disciplinasCursadas++;
            
            float n1 = notas[idAluno][j].n1;
            float n2 = notas[idAluno][j].n2;

            char strN1[10] = "-", strN2[10] = "-";

            if (n1 >= 0.0f) sprintf(strN1, "%.1f", n1);
            if (n2 >= 0.0f) sprintf(strN2, "%.1f", n2);

            printf("%-20s | %-7s | %-7s | ", disciplinas[j].nome, strN1, strN2);

            // REGRA: Só calcula média e status se tiver AS DUAS NOTAS LANÇADAS
            if (n1 >= 0.0f && n2 >= 0.0f) {
                float media = (n1 + n2) / 2.0f;
                printf("%-7.1f | ", media);

                if (media >= 7.0f) {
                    printf(COR_SUCESSO "%-12s" COR_RESET "\n", "APROVADO");
                } else {
                    printf(COR_ERRO "%-12s" COR_RESET "\n", "REPROVADO");
                }
            } else {
                // Se falta Nota 1 ou Nota 2
                printf("%-7s | ", "-");
                printf(COR_AVISO "%-12s" COR_RESET "\n", "PENDENTE");
            }
        }
    }

    if (disciplinasCursadas == 0) {
        printf(COR_AVISO " Este aluno não está matriculado em nenhuma disciplina.\n" COR_RESET);
    }

    printf("-------------------------------------------------------------------\n");
    pausa();
}

void listarMatrizGeral() {
    limpaTela();
    exibirCabecalho("VISÃO GERAL DA MATRIZ (TODOS OS ALUNOS)");

    if (totalAlunos == 0 || totalDisciplinas == 0) {
        printf(COR_ERRO "Cadastre alunos e disciplinas para visualizar a matriz.\n" COR_RESET);
        pausa();
        return;
    }

    // Cabeçalho da Tabela
    printf("%-15s |", "Aluno");
    for (int j = 0; j < totalDisciplinas; j++) {
        printf(" %-12s |", disciplinas[j].nome);
    }
    printf("\n----------------------------------------------------------------------\n");

    // Linhas da Matriz
    for (int i = 0; i < totalAlunos; i++) {
        printf("%-15s |", alunos[i].nome);

        for (int j = 0; j < totalDisciplinas; j++) {
            if (notas[i][j].matriculado) {
                float n1 = notas[i][j].n1;
                float n2 = notas[i][j].n2;

                if (n1 >= 0.0f && n2 >= 0.0f) {
                    float media = (n1 + n2) / 2.0f;
                    if (media >= 7.0f) {
                        printf(COR_SUCESSO " M: %-6.1f " COR_RESET "|", media);
                    } else {
                        printf(COR_ERRO " M: %-6.1f " COR_RESET "|", media);
                    }
                } else {
                    printf(COR_AVISO " %-10s " COR_RESET "|", "Pendente");
                }
            } else {
                printf(" %-10s |", "-"); // Não matriculado
            }
        }
        printf("\n");
    }

    printf("----------------------------------------------------------------------\n");
    pausa();
}
