#include <stdio.h>

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
*/

// Assinaturas das funções
typedef struct {
    char nome[100];
    int quantidade;
    float preco;
} Produto;

void inserir_produto(Produto *produtos,int *nprodutos,char nome_produto[100],int qntd_produto,float preco_produto);
void aumentar_estoque(int codigo_produto, int qntd_aumentar);
void modificar_preco(int codigo_produto, float novo_preco);
void vender(int codigo_produto,int *preco_total);
void consultar_estoque();
void consultar_saldo();
void finalizar_dia();



int main(void)
{
    int nProdutos = 0;
    char comando[2];
    Produto *produtos = NULL; // para guardar os produtos cadastrados
    scanf(" %s", comando);

    while(comando[0] != 'F' || comando[1] != 'E')
    {
        if(comando == 'IP')
        {
            char nome_produto[100];
            int qntd_produto;
            float preco_produto;
            scanf(" %s %d %f", nome_produto, &qntd_produto, &preco_produto);
            inserir_produto(&nProdutos, nome_produto, qntd_produto, preco_produto);
        }
        else if(comando == 'AE')
        {
            int codigo_produto;
            int qntd_aumentar;
            scanf(" %d %d", &codigo_produto, &qntd_aumentar);
            aumentar_estoque(codigo_produto, qntd_aumentar);
        }
        else if(comando == 'MP')
        {
            int codigo_produto;
            float novo_preco;
            scanf(" %d %f", &codigo_produto, &novo_preco);
            modificar_preco(codigo_produto, novo_preco);
        }
        else if(comando == 'VE')
        {
            int codigo_produto;
            int preco_total=0;
            scanf(" %d", &codigo_produto);
            while(codigo_produto != -1)
            {
                vender(codigo_produto, &preco_total);
                scanf(" %d", &codigo_produto);
            }
            printf("Total: %d\n", preco_total);
            
        }
        else if(comando == 'CE')
        {
            consultar_estoque();
        }
        else if(comando == 'CS')
        {
            consultar_saldo();
        }
        scanf(" %s", comando);
    }
    
    finalizar_dia();
    

}

void inserir_produto(Produto *produtos, int *nprodutos, char nome_produto[100],int qntd_produto,float preco_produto)
{
    Produto p = {nome_produto, qntd_produto, preco_produto};
}