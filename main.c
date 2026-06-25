#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[100];
    int quantidade;
    float preco;
} Produto;

// Estoque de produtos (funciona como um array dinâmico)
typedef struct {
    Produto *produtos; // Vetor que guarda os produtos
    size_t qntd_produtos;
    size_t capacidade; // Capacidade do estoque, relacionada ao tamanho da memória alocada
} Estoque;

// Assinaturas das funções
void inserir_produto(Estoque *estoque, char nome_produto[100], int qntd_produto, float preco_produto);
void aumentar_estoque(Estoque *estoque, int codigo_produto, int qntd_aumentar, float *saldo);
void modificar_preco(Estoque *estoque, int codigo_produto, float novo_preco);
void vender(Estoque *estoque, int codigo_produto, float *preco_total);
void consultar_estoque(Estoque *estoque);
void consultar_saldo(float saldo);
void finalizar_dia(Estoque *estoque, float saldo);

// Funções auxiliares
void abrir_estoque(Estoque *estoque, float *saldo); // Carrega o estoque a partir dos dados armazenados na memória

// Útil para a formatação da saída
const char BARRA_HORIZONTAL[] = "--------------------------------------------------";

int main(void)
{
    char comando[3]; // 3 caracteres (2 para o comando + 1 para o terminador)
    float saldo = 100.0f; // Saldo inicial do caixa

    // Criação do estoque. Os valores abaixo serão sobrescritos pela função abrir_estoque()
    Estoque estoque =
    {
        .produtos = NULL,
        .qntd_produtos = 0,
        .capacidade = 0
    }; // para guardar os produtos cadastrados

    abrir_estoque(&estoque, &saldo);

    scanf(" %s", comando);

    while (strcmp(comando, "FE") != 0)
    {
        if(strcmp(comando, "IP") == 0)
        {
            char nome_produto[100];
            int qntd_produto;
            float preco_produto;
            scanf(" %s %d %f", nome_produto, &qntd_produto, &preco_produto);
            inserir_produto(&estoque, nome_produto, qntd_produto, preco_produto);
        }
        else if(strcmp(comando, "AE") == 0)
        {
            int codigo_produto;
            int qntd_aumentar;
            scanf(" %d %d", &codigo_produto, &qntd_aumentar);
            aumentar_estoque(&estoque, codigo_produto, qntd_aumentar, &saldo);
        }
        else if(strcmp(comando, "MP") == 0)
        {
            int codigo_produto;
            float novo_preco;
            scanf(" %d %f", &codigo_produto, &novo_preco);
            modificar_preco(&estoque, codigo_produto, novo_preco);
        }
        else if(strcmp(comando, "VE") == 0)
        {
            int codigo_produto;
            float preco_total=0.0f;
            int valido = 1;
            while(scanf(" %d", &codigo_produto) == 1 && valido)
            {
                if (codigo_produto == -1)
                    valido = 0;

                if (valido)
                    vender(&estoque, codigo_produto, &preco_total);
            }
            printf("Total: %.2f\n", preco_total);
            saldo += preco_total;
            printf("%s\n", BARRA_HORIZONTAL);
        }
        else if(strcmp(comando, "CE") == 0)
        {
            consultar_estoque(&estoque);
            printf("%s\n", BARRA_HORIZONTAL);
        }
        else if(strcmp(comando, "CS") == 0)
        {
            consultar_saldo(saldo);
            printf("%s\n", BARRA_HORIZONTAL);
        }
        scanf(" %s", comando);
    }
    
    finalizar_dia(&estoque, saldo);

    // Liberação de memória
    free(estoque.produtos);
    estoque.produtos = NULL;

    return 0;
}

// Insere um novo produto no estoque com nome, quantidade e preço fornecidos
// Realoca o vetor de produtos automaticamente se a capacidade for insuficiente
void inserir_produto(Estoque *estoque, char nome_produto[100], int qntd_produto, float preco_produto)
{
    // Realocação de memória no caso de "estouro" de capacidade do vetor
    if (estoque->qntd_produtos == estoque->capacidade)
    {
        // Evita multiplicar capacidade 0 por 2 (resultaria em 0). Garante ao menos 1 slot
        estoque->capacidade = estoque->capacidade == 0 ? 1 : estoque->capacidade * 2;

        Produto *temp = realloc(
            estoque->produtos,
            estoque->capacidade * sizeof(Produto)
        );

        if (temp == NULL)
        {
            printf("Erro de alocação.\n");
            exit(1);
        }

        estoque->produtos = temp;
    }

    Produto p = {0};
    strcpy(p.nome, nome_produto);
    p.quantidade = qntd_produto;
    p.preco = preco_produto;

    // Armazena no estoque, com a segurança de que haverá espaço no vetor
    estoque->produtos[estoque->qntd_produtos++] = p;
}

// Aumenta a quantidade de um produto existente no estoque
// O custo da aquisição (quantidade * preço) é descontado do saldo do caixa
void aumentar_estoque(Estoque *estoque, int codigo_produto, int qntd_aumentar, float *saldo)
{
    (estoque->produtos)[codigo_produto].quantidade += qntd_aumentar;
    float custo = (estoque->produtos)[codigo_produto].preco * qntd_aumentar;
    *saldo -= custo;
}

// Atualiza o preço de um produto existente no estoque
void modificar_preco(Estoque *estoque, int codigo_produto, float novo_preco)
{
    (estoque->produtos)[codigo_produto].preco = novo_preco;
}

// Realiza a venda de uma unidade de um produto, imprimindo nome e preço
// Acumula o valor no total da venda. Ignora o produto se estoque for zero
void vender(Estoque *estoque, int codigo_produto, float *preco_total)
{
    Produto *p = &(estoque->produtos[codigo_produto]);
    
    // Vende apenas se houver unidades disponíveis no estoque
    if (p->quantidade <= 0) return;

    p->quantidade--;
    *preco_total += p->preco;
    printf("%s %.2f\n", p->nome, p->preco);
}

// Imprime o código, nome e quantidade em estoque de todos os produtos cadastrados
void consultar_estoque(Estoque *estoque)
{
    if(estoque->qntd_produtos == 0)
    {
        printf("Estoque vazio.\n");
        return;
    }

    for(size_t i = 0; i < estoque->qntd_produtos; i++)
    {
        Produto *p = &(estoque->produtos[i]);
        printf("%zu %s %d\n", i, p->nome, p->quantidade);
    }
}

// Imprime o saldo atual do caixa.
void consultar_saldo(float saldo)
{
    printf("Saldo: %.2f\n", saldo);
}

// Encerra o dia gravando o estoque e o saldo em arquivo binário (estoque.bin)
void finalizar_dia(Estoque *estoque, float saldo)
{
    FILE *fp;
    fp = fopen("estoque.bin", "wb");

    if (fp == NULL)
    {
        return;
    }

    // Salva a quantidade de produtos, o saldo final do dia e o conteúdo do estoque
    fwrite(&estoque->qntd_produtos, sizeof(size_t), 1, fp);
    fwrite(&saldo, sizeof(float), 1, fp);
    fwrite(estoque->produtos, sizeof(Produto), estoque->qntd_produtos, fp);

    fclose(fp);
}

// Inicializa o estoque a partir do arquivo do dia anterior (estoque.bin), se existir
// Caso contrário, lê a capacidade inicial do stdin e aloca o vetor vazio
void abrir_estoque(Estoque *estoque, float *saldo)
{
    FILE *fp = fopen("estoque.bin", "rb");
    
    if (fp == NULL) // Sem estoque.bin
    {
        int capacidade_inicial;
        scanf("%d", &capacidade_inicial);
        estoque->capacidade = capacidade_inicial;
        estoque->produtos = (Produto*) malloc(sizeof(Produto) * capacidade_inicial); // Aloca espaço vazio no caso de não haver estoque anterior
        if (estoque->produtos == NULL)
        {
            printf("Erro de alocação.\n");
            exit(1);
        }
        return;
    }
    
    // Lê os dados referentes ao estado anterior do estoque e ao saldo
    size_t qnt_produtos;
    fread(&qnt_produtos, sizeof(size_t), 1, fp);

    fread(saldo, sizeof(float), 1, fp);
    
    Produto *novo = malloc(qnt_produtos * sizeof(Produto));
    if (novo == NULL)
    {
        printf("Sem espaço pra alocação do buffer");
        return;
    }
    
    fread(novo, sizeof(Produto), qnt_produtos, fp);
    
    // Libera a memória em caso do ponteiro apontar para um espaço de memória já alocado
    free(estoque->produtos);
    estoque->produtos = NULL;

    // Atualiza o estoque conforme os dados carregados
    estoque->produtos = novo;
    estoque->qntd_produtos = qnt_produtos;
    estoque->capacidade = qnt_produtos;

    fclose(fp);
}