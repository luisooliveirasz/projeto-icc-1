#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Obs.: remover esses comentários antes de entregar o projeto.

Detalhes importantes a se lembrar:
- Método de avaliação: código (clareza modularidade), execução (faz tudo que deveria e como
deveria?), utilização de recursos (processamento e memória) e documentação interna.
- Utilizar padrão C99, compilador GCC e ambiente Cygwin (Windows) ou shell (Linux/Mac)

Formatação das entradas e saídas:
----------------------------------------------------------
Inserir produto   - IP_<nome>_<quantidade>_<preço>
Aumentar estoque  - AE_<código>_<quantidade>
Modificar preço   - MP_<código>_<preço>
Vender            - VE_<código>_<código>_..._<código>_<-1>
Consultar estoque - CE
Consultar saldo   - CS
Finalizar o dia   - FE
----------------------------------------------------------

Registro 15/06 (luís):
- criei a struct Estoque para armazenar de forma dinâmica os produtos
- troquei as comparações comando == '...' por strcmp(comando, "...")
- Troquei o tipo da variável preco_total para float

*/


typedef struct {
    char nome[100];
    int quantidade;
    float preco;
} Produto;

// Estoque de produtos (funciona como um array dinâmico)
typedef struct {
    Produto *produtos; // Vetor que guarda os produtos
    size_t qntd_produtos; // Tamanho do vetor
    size_t capacidade; // Capacidade do estoque, relacionada ao tamanho da memória alocada
} Estoque;

// Assinaturas das funções
void inserir_produto(Estoque *estoque, char nome_produto[100], int qntd_produto, float preco_produto);
void aumentar_estoque(Estoque *estoque, int codigo_produto, int qntd_aumentar, float *saldo);
void modificar_preco(Estoque *estoque, int codigo_produto, float novo_preco);
void vender(Estoque *estoque, int codigo_produto, float *preco_total, float *saldo);
void consultar_estoque(Estoque *estoque);
void consultar_saldo(float saldo);
void finalizar_dia(Estoque *estoque);


// Útil para a formatação da saída
const char BARRA_HORIZONTAL[] = "--------------------------------------------------";

int main(void)
{
    char comando[3]; // 3 caracteres (2 para o comando + 1 para o terminador)
    float saldo = 100.0f; // Saldo inicial do caixa

    Estoque estoque = {
        .produtos = NULL,
        .qntd_produtos = 0,
        .capacidade = 10 // Capacidade inicial
    }; // para guardar os produtos cadastrados

    estoque.produtos = (Produto*) malloc(sizeof(Produto) * estoque.capacidade);

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
            scanf(" %d", &codigo_produto);
            while(codigo_produto != -1)
            {
                vender(&estoque, codigo_produto, &preco_total, &saldo);
                scanf(" %d", &codigo_produto);
            }
            printf("Total: %.2f\n", preco_total);
            saldo += preco_total;
            printf("%s", BARRA_HORIZONTAL);
        }
        else if(strcmp(comando, "CE") == 0)
        {
            consultar_estoque(&estoque);
            printf("%s", BARRA_HORIZONTAL);
        }
        else if(strcmp(comando, "CS") == 0)
        {
            consultar_saldo(saldo);
            printf("%s", BARRA_HORIZONTAL);
        }
        scanf(" %s", comando);
    }
    
    finalizar_dia(&estoque);

    // Liberação de memória
    free(estoque.produtos);

    return 0;
}

void inserir_produto(Estoque *estoque, char nome_produto[100], int qntd_produto, float preco_produto)
{
    if (estoque->qntd_produtos == estoque->capacidade)
    {
        estoque->capacidade *= 2;

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

    Produto p;

    strcpy(p.nome, nome_produto);
    p.quantidade = qntd_produto;
    p.preco = preco_produto;

    estoque->produtos[estoque->qntd_produtos++] = p;
}

void aumentar_estoque(Estoque *estoque, int codigo_produto, int qntd_aumentar, float *saldo)
{
    (estoque->produtos)[codigo_produto - 1].quantidade += qntd_aumentar;
    float custo = (estoque->produtos)[codigo_produto - 1].preco * qntd_aumentar;
    *saldo -= custo;
}

void modificar_preco(Estoque *estoque, int codigo_produto, float novo_preco)
{
    (estoque->produtos)[codigo_produto - 1].preco = novo_preco;
}

void vender(Estoque *estoque, int codigo_produto, float *preco_total)
{
    Produto *p = &(estoque->produtos[codigo_produto]);
    p->quantidade--;
    *preco_total += p->preco;
    printf("%s %.2f\n", p->nome, p->preco);
}

void consultar_estoque(Estoque *estoque)
{
    if(estoque->qntd_produtos == 0)
    {
        printf("Estoque vazio.\n");
        return;
    }
    for(int i = 0; i < estoque->qntd_produtos; i++)
    {
        Produto *p = &(estoque->produtos[i]);
        printf("Código: %d Nome: %s Quantidade: %d\n", i + 1, p->nome, p->quantidade);
    }
}

void consultar_saldo(float saldo)
{
    printf("Saldo: %.2f\n", saldo);
}

void finalizar_dia(Estoque *estoque)
{
    FILE *fp;
    fp = fopen("estoque.txt", "wb");

    if (!fp) return;

    fwrite(&estoque->qntd_produtos, sizeof(size_t), 1, fp);
    fwrite(estoque->produtos, sizeof(Produto), estoque->qntd_produtos, fp);

    fclose(fp);
}