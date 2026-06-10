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
void inserir_produto();
void aumentar_estoque();
void modificar_preco();
void vender();
void consultar_estoque();
void consultar_saldo();
void finalizar_dia();

int main(void)
{

}