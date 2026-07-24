#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALUNOS 50
#define MAX_DISCIPLINAS 10
#define MAX_NOME 50

// --- CORES ANSI ---
#define COR_RESET   "\033[0m"
#define COR_TITULO  "\033[1;34m" // Azul
#define COR_SUCESSO "\033[1;32m" // Verde
#define COR_ERRO    "\033[1;31m" // Vermelho
#define COR_AVISO   "\033[1;33m" // Amarelo
#define COR_DESTINU "\033[1;36m" // Ciano

// --- ESTRUTURAS ---
typedef struct {
    int id;
    char nome[MAX_NOME];
    int ativo; // 1 = Ativo, 0 = Excluído
} Aluno;

typedef struct {
    int id;
    char nome[MAX_NOME];
    int ativo; // 1 = Ativo, 0 = Excluído
} Disciplina;

typedef struct {
    float n1;        // -1.0f = Sem Nota
    float n2;        // -1.0f = Sem Nota
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

// Persistência em Arquivos (.dat)
void salvarDados();
void carregarDados();

// Módulos
void cadastrarAluno();
void editarAluno();
void excluirAluno();

void cadastrarDisciplina();
void editarDisciplina();
void excluirDisciplina();

void matricularAluno();
void lancarNotasDuplas();

void buscarPorNome();
void ordenarAlunosAlfabetico();
void listarBoletimAluno();
void listarMatrizGeral();

// --- FUNÇÃO PRINCIPAL ---
int main() {
    int opcao;
    
    inicializarSistema();
    carregarDados(); // Carregamento Automático ao iniciar

    do {
        limpaTela();
        exibirCabecalho("SISTEMA ACADÊMICO COMPLETO");
        
        printf("  [1]  Cadastrar Aluno              [7]  Lançar Notas (N1 e N2)\n");
        printf("  [2]  Editar / Excluir Aluno       [8]  Buscar por Nome\n");
        printf("  [3]  Cadastrar Disciplina         [9]  Boletim Individual do Aluno\n");
        printf("  [4]  Editar / Excluir Disciplina  [10] Matriz Geral de Notas\n");
        printf("  [5]  Listar Alunos (A-Z)          [11] Salvar Dados Manualmente\n");
        printf("  [6]  Matricular Aluno             [0]  Sair do Sistema\n");
        printf("----------------------------------------------------------------\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        getchar();

        switch (opcao) {
            case 1: cadastrarAluno(); break;
            case 2: 
                limpaTela();
                exibirCabecalho("GERENCIAR ALUNOS");
                printf("  [1] Editar Aluno\n  [2] Excluir Aluno\nOpção: ");
                int subA; scanf("%d", &subA); getchar();
                if (subA == 1) editarAluno();
                else if (subA == 2) excluirAluno();
                break;
            case 3: cadastrarDisciplina(); break;
            case 4:
                limpaTela();
                exibirCabecalho("GERENCIAR DISCIPLINAS");
                printf("  [1] Editar Disciplina\n  [2] Excluir Disciplina\nOpção: ");
                int subD; scanf("%d", &subD); getchar();
                if (subD == 1) editarDisciplina();
                else if (subD == 2) excluirDisciplina();
                break;
            case 5: 
                ordenarAlunosAlfabetico();
                printf(COR_SUCESSO "\nAlunos ordenados em ordem alfabética!\n" COR_RESET);
                pausa();
                break;
            case 6: matricularAluno(); break;
            case 7: lancarNotasDuplas(); break;
            case 8: buscarPorNome(); break;
            case 9: listarBoletimAluno(); break;
            case 10: listarMatrizGeral(); break;
            case 11: 
                salvarDados();
                printf(COR_SUCESSO "\nDados salvos com sucesso nos arquivos .dat!\n" COR_RESET);
                pausa();
                break;
            case 0: 
                salvarDados(); // Salvamento Automático ao sair
                limpaTela();
                printf(COR_SUCESSO "\nDados salvos automaticamente. Saindo do sistema...\n\n" COR_RESET);
                break;
            default:
                printf(COR_ERRO "\n[Opção Inválida!]\n" COR_RESET);
                pausa();
                break;
        }
    } while (opcao != 0);

    return 0;
}

// --- UTILITÁRIOS ---

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
        alunos[i].ativo = 0;
        for (int j = 0; j < MAX_DISCIPLINAS; j++) {
            notas[i][j].n1 = -1.0f;
            notas[i][j].n2 = -1.0f;
            notas[i][j].matriculado = 0;
        }
    }
    for (int j = 0; j < MAX_DISCIPLINAS; j++) {
        disciplinas[j].ativo = 0;
    }
}

void exibirCabecalho(const char *titulo) {
    printf(COR_TITULO "================================================================\n");
    printf("                  SISTEMA ACADÊMICO COMPLETO                    \n");
    printf("================================================================\n" COR_RESET);
    printf(COR_DESTINU " >>> %s <<<\n" COR_RESET, titulo);
    printf("----------------------------------------------------------------\n");
}

// --- PERSISTÊNCIA DE DADOS (.DAT) ---

void salvarDados() {
    FILE *fAlunos = fopen("alunos.dat", "wb");
    if (fAlunos) {
        fwrite(&totalAlunos, sizeof(int), 1, fAlunos);
        fwrite(alunos, sizeof(Aluno), MAX_ALUNOS, fAlunos);
        fclose(fAlunos);
    }

    FILE *fDisc = fopen("disciplinas.dat", "wb");
    if (fDisc) {
        fwrite(&totalDisciplinas, sizeof(int), 1, fDisc);
        fwrite(disciplinas, sizeof(Disciplina), MAX_DISCIPLINAS, fDisc);
        fclose(fDisc);
    }

    FILE *fNotas = fopen("notas.dat", "wb");
    if (fNotas) {
        fwrite(notas, sizeof(Avaliacao), MAX_ALUNOS * MAX_DISCIPLINAS, fNotas);
        fclose(fNotas);
    }
}

void carregarDados() {
    FILE *fAlunos = fopen("alunos.dat", "rb");
    if (fAlunos) {
        fread(&totalAlunos, sizeof(int), 1, fAlunos);
        fread(alunos, sizeof(Aluno), MAX_ALUNOS, fAlunos);
        fclose(fAlunos);
    }

    FILE *fDisc = fopen("disciplinas.dat", "rb");
    if (fDisc) {
        fread(&totalDisciplinas, sizeof(int), 1, fDisc);
        fread(disciplinas, sizeof(Disciplina), MAX_DISCIPLINAS, fDisc);
        fclose(fDisc);
    }

    FILE *fNotas = fopen("notas.dat", "rb");
    if (fNotas) {
        fread(notas, sizeof(Avaliacao), MAX_ALUNOS * MAX_DISCIPLINAS, fNotas);
        fclose(fNotas);
    }
}

// --- GESTÃO DE ALUNOS ---

void cadastrarAluno() {
    limpaTela();
    exibirCabecalho("CADASTRO DE ALUNO");

    if (totalAlunos >= MAX_ALUNOS) {
        printf(COR_ERRO "[Erro] Limite de alunos atingido!\n" COR_RESET);
        pausa();
        return;
    }

    alunos[totalAlunos].id = totalAlunos;
    alunos[totalAlunos].ativo = 1;
    
    printf("Nome do Aluno: ");
    fgets(alunos[totalAlunos].nome, MAX_NOME, stdin);
    alunos[totalAlunos].nome[strcspn(alunos[totalAlunos].nome, "\n")] = '\0';

    if (strlen(alunos[totalAlunos].nome) > 0) {
        printf(COR_SUCESSO "\n-> Aluno '%s' cadastrado com ID [%d]!\n" COR_RESET, alunos[totalAlunos].nome, totalAlunos);
        totalAlunos++;
        salvarDados();
    } else {
        printf(COR_ERRO "\n[Erro] Nome inválido.\n" COR_RESET);
    }
    pausa();
}

void editarAluno() {
    int id;
    printf("Digite o ID do Aluno a ser editado: ");
    scanf("%d", &id); getchar();

    if (id >= 0 && id < totalAlunos && alunos[id].ativo) {
        printf("Nome atual: %s\nDigite o NOVO Nome: ", alunos[id].nome);
        fgets(alunos[id].nome, MAX_NOME, stdin);
        alunos[id].nome[strcspn(alunos[id].nome, "\n")] = '\0';
        printf(COR_SUCESSO "-> Aluno atualizado com sucesso!\n" COR_RESET);
        salvarDados();
    } else {
        printf(COR_ERRO "[Erro] Aluno não encontrado.\n" COR_RESET);
    }
    pausa();
}

void excluirAluno() {
    int id;
    printf("Digite o ID do Aluno a ser excluído: ");
    scanf("%d", &id); getchar();

    if (id >= 0 && id < totalAlunos && alunos[id].ativo) {
        alunos[id].ativo = 0; // Exclusão lógica
        printf(COR_SUCESSO "-> Aluno '%s' foi removido!\n" COR_RESET, alunos[id].nome);
        salvarDados();
    } else {
        printf(COR_ERRO "[Erro] Aluno não encontrado.\n" COR_RESET);
    }
    pausa();
}

// --- GESTÃO DE DISCIPLINAS ---

void cadastrarDisciplina() {
    limpaTela();
    exibirCabecalho("CADASTRO DE DISCIPLINA");

    if (totalDisciplinas >= MAX_DISCIPLINAS) {
        printf(COR_ERRO "[Erro] Limite de disciplinas atingido!\n" COR_RESET);
        pausa();
        return;
    }

    disciplinas[totalDisciplinas].id = totalDisciplinas;
    disciplinas[totalDisciplinas].ativo = 1;

    printf("Nome da Disciplina: ");
    fgets(disciplinas[totalDisciplinas].nome, MAX_NOME, stdin);
    disciplinas[totalDisciplinas].nome[strcspn(disciplinas[totalDisciplinas].nome, "\n")] = '\0';

    if (strlen(disciplinas[totalDisciplinas].nome) > 0) {
        printf(COR_SUCESSO "\n-> Disciplina '%s' cadastrada com ID [%d]!\n" COR_RESET, disciplinas[totalDisciplinas].nome, totalDisciplinas);
        totalDisciplinas++;
        salvarDados();
    } else {
        printf(COR_ERRO "\n[Erro] Nome inválido.\n" COR_RESET);
    }
    pausa();
}

void editarDisciplina() {
    int id;
    printf("Digite o ID da Disciplina a editar: ");
    scanf("%d", &id); getchar();

    if (id >= 0 && id < totalDisciplinas && disciplinas[id].ativo) {
        printf("Nome atual: %s\nDigite o NOVO Nome: ", disciplinas[id].nome);
        fgets(disciplinas[id].nome, MAX_NOME, stdin);
        disciplinas[id].nome[strcspn(disciplinas[id].nome, "\n")] = '\0';
        printf(COR_SUCESSO "-> Disciplina atualizada!\n" COR_RESET);
        salvarDados();
    } else {
        printf(COR_ERRO "[Erro] Disciplina não encontrada.\n" COR_RESET);
    }
    pausa();
}

void excluirDisciplina() {
    int id;
    printf("Digite o ID da Disciplina a excluir: ");
    scanf("%d", &id); getchar();

    if (id >= 0 && id < totalDisciplinas && disciplinas[id].ativo) {
        disciplinas[id].ativo = 0;
        printf(COR_SUCESSO "-> Disciplina '%s' removida!\n" COR_RESET, disciplinas[id].nome);
        salvarDados();
    } else {
        printf(COR_ERRO "[Erro] Disciplina não encontrada.\n" COR_RESET);
    }
    pausa();
}

// --- MATRÍCULA E NOTAS ---

void matricularAluno() {
    limpaTela();
    exibirCabecalho("MATRICULAR ALUNO");

    printf(COR_AVISO "Alunos:\n" COR_RESET);
    for (int i = 0; i < totalAlunos; i++) {
        if (alunos[i].ativo) printf("  ID [%d] - %s\n", alunos[i].id, alunos[i].nome);
    }

    printf(COR_AVISO "\nDisciplinas:\n" COR_RESET);
    for (int j = 0; j < totalDisciplinas; j++) {
        if (disciplinas[j].ativo) printf("  ID [%d] - %s\n", disciplinas[j].id, disciplinas[j].nome);
    }
    printf("----------------------------------------------------------------\n");

    int idA, idD;
    printf("ID do Aluno: "); scanf("%d", &idA);
    printf("ID da Disciplina: "); scanf("%d", &idD); getchar();

    if (idA >= 0 && idA < totalAlunos && alunos[idA].ativo &&
        idD >= 0 && idD < totalDisciplinas && disciplinas[idD].ativo) {
        notas[idA][idD].matriculado = 1;
        printf(COR_SUCESSO "-> '%s' matriculado em '%s'!\n" COR_RESET, alunos[idA].nome, disciplinas[idD].nome);
        salvarDados();
    } else {
        printf(COR_ERRO "[Erro] IDs inválidos.\n" COR_RESET);
    }
    pausa();
}

void lancarNotasDuplas() {
    limpaTela();
    exibirCabecalho("LANÇAMENTO DE NOTAS (N1 E N2)");

    int idA, idD;
    float n1, n2;

    printf("ID do Aluno: "); scanf("%d", &idA);
    printf("ID da Disciplina: "); scanf("%d", &idD);

    if (idA >= 0 && idA < totalAlunos && alunos[idA].ativo &&
        idD >= 0 && idD < totalDisciplinas && disciplinas[idD].ativo) {
        
        if (!notas[idA][idD].matriculado) {
            printf(COR_ERRO "\n[Aviso] Aluno não está matriculado nesta disciplina!\n" COR_RESET);
            getchar(); pausa(); return;
        }

        printf("\nDigite a Primeira Nota (N1) [0.0 a 10.0]: "); scanf("%f", &n1);
        printf("Digite a Segunda Nota  (N2) [0.0 a 10.0]: "); scanf("%f", &n2); getchar();

        if (n1 >= 0.0f && n1 <= 10.0f && n2 >= 0.0f && n2 <= 10.0f) {
            notas[idA][idD].n1 = n1;
            notas[idA][idD].n2 = n2;
            printf(COR_SUCESSO "\n-> Notas N1=%.1f e N2=%.1f salvas para %s!\n" COR_RESET, n1, n2, alunos[idA].nome);
            salvarDados();
        } else {
            printf(COR_ERRO "\n[Erro] Notas devem estar entre 0.0 e 10.0.\n" COR_RESET);
        }
    } else {
        printf(COR_ERRO "\n[Erro] IDs inválidos.\n" COR_RESET);
        getchar();
    }
    pausa();
}

// --- BUSCA E ORDENAÇÃO ---

void buscarPorNome() {
    limpaTela();
    exibirCabecalho("PESQUISA POR NOME");

    char termo[MAX_NOME];
    printf("Digite o nome (ou parte dele) para buscar: ");
    fgets(termo, MAX_NOME, stdin);
    termo[strcspn(termo, "\n")] = '\0';

    printf("\n" COR_AVISO "Resultados Encontrados:\n" COR_RESET);
    int achou = 0;

    for (int i = 0; i < totalAlunos; i++) {
        if (alunos[i].ativo && strstr(alunos[i].nome, termo) != NULL) {
            printf("  ID [%d] - %s\n", alunos[i].id, alunos[i].nome);
            achou = 1;
        }
    }

    if (!achou) {
        printf(COR_ERRO "Nenhum aluno encontrado com este termo.\n" COR_RESET);
    }
    pausa();
}

void ordenarAlunosAlfabetico() {
    for (int i = 0; i < totalAlunos - 1; i++) {
        for (int j = 0; j < totalAlunos - i - 1; j++) {
            if (strcmp(alunos[j].nome, alunos[j + 1].nome) > 0) {
                // Troca Cadastro de Alunos
                Aluno tempA = alunos[j];
                alunos[j] = alunos[j + 1];
                alunos[j + 1] = tempA;

                // Troca as linhas correspondentes da Matriz de Notas
                for (int k = 0; k < MAX_DISCIPLINAS; k++) {
                    Avaliacao tempN = notas[j][k];
                    notas[j][k] = notas[j + 1][k];
                    notas[j + 1][k] = tempN;
                }
            }
        }
    }
    salvarDados();
}

// --- RELATÓRIOS E BOLETIM ---

void listarBoletimAluno() {
    limpaTela();
    exibirCabecalho("BOLETIM INDIVIDUAL DO ALUNO");

    int idA;
    printf("ID do Aluno: "); scanf("%d", &idA); getchar();

    if (idA < 0 || idA >= totalAlunos || !alunos[idA].ativo) {
        printf(COR_ERRO "\n[Erro] Aluno inválido.\n" COR_RESET);
        pausa(); return;
    }

    limpaTela();
    printf(COR_TITULO "===================================================================\n");
    printf(" BOLETIM ESCOLAR - ALUNO: %s\n", alunos[idA].nome);
    printf("===================================================================\n" COR_RESET);
    printf("%-20s | %-7s | %-7s | %-7s | %-12s\n", "Disciplina", "Nota 1", "Nota 2", "Média", "Situação");
    printf("-------------------------------------------------------------------\n");

    for (int j = 0; j < totalDisciplinas; j++) {
        if (disciplinas[j].ativo && notas[idA][j].matriculado) {
            float n1 = notas[idA][j].n1;
            float n2 = notas[idA][j].n2;

            char sN1[10] = "-", sN2[10] = "-";
            if (n1 >= 0.0f) sprintf(sN1, "%.1f", n1);
            if (n2 >= 0.0f) sprintf(sN2, "%.1f", n2);

            printf("%-20s | %-7s | %-7s | ", disciplinas[j].nome, sN1, sN2);

            if (n1 >= 0.0f && n2 >= 0.0f) {
                float media = (n1 + n2) / 2.0f;
                printf("%-7.1f | ", media);
                if (media >= 7.0f) printf(COR_SUCESSO "%-12s" COR_RESET "\n", "APROVADO");
                else printf(COR_ERRO "%-12s" COR_RESET "\n", "REPROVADO");
            } else {
                printf("%-7s | ", "-");
                printf(COR_AVISO "%-12s" COR_RESET "\n", "PENDENTE");
            }
        }
    }
    printf("-------------------------------------------------------------------\n");
    pausa();
}

void listarMatrizGeral() {
    limpaTela();
    exibirCabecalho("VISÃO GERAL DA MATRIZ ACADÊMICA");

    printf("%-15s |", "Aluno");
    for (int j = 0; j < totalDisciplinas; j++) {
        if (disciplinas[j].ativo) printf(" %-12s |", disciplinas[j].nome);
    }
    printf("\n----------------------------------------------------------------------\n");

    for (int i = 0; i < totalAlunos; i++) {
        if (alunos[i].ativo) {
            printf("%-15s |", alunos[i].nome);
            for (int j = 0; j < totalDisciplinas; j++) {
                if (disciplinas[j].ativo) {
                    if (notas[i][j].matriculado) {
                        float n1 = notas[i][j].n1;
                        float n2 = notas[i][j].n2;
                        if (n1 >= 0.0f && n2 >= 0.0f) {
                            float media = (n1 + n2) / 2.0f;
                            if (media >= 7.0f) printf(COR_SUCESSO " M: %-6.1f " COR_RESET "|", media);
                            else printf(COR_ERRO " M: %-6.1f " COR_RESET "|", media);
                        } else {
                            printf(COR_AVISO " %-10s " COR_RESET "|", "Pendente");
                        }
                    } else {
                        printf(" %-10s |", "-");
                    }
                }
            }
            printf("\n");
        }
    }
    printf("----------------------------------------------------------------------\n");
    pausa();
}
