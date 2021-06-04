//Eduardo Filipe Ferreira da Cruz
#ifndef ESTRUTURAS_DE_DADOS_H //if not defined
#define ESTRUTURAS_DE_DADOS_H

/*Estruturas*/

typedef struct Socio{
  char *nome;
  char *id;
  struct pagamento *raiz_lista_pagamentos;
}Socio;

//node da lista ligada de socios do club. Estrutura principal do Programa
struct elem_lista_socios{
  Socio socio;
  struct elem_lista_socios *seguinte;
};

//node de lista ligada de pagamentos associada a cada socio (cada socio tem uma lista (independente) de pagamentos associados a ele). Estrutura secundaria do programa
struct pagamento{
  float quantia_paga;
  struct pagamento *seguinte;
};

/*Funções*/

/*inicialização de listas ligadas (set)*/
//para inicializar a lista ligada dos socios
void inicializar_lista_socios(void);
//para inicializar a lista de pagamentos associada a cada socio
void inicializar_lista_pagamentos(Socio*);

/*(get)*/
//devolve raiz da lista ligada dos socios
struct elem_lista_socios* get_raiz_lista_socios(void);

/*pesquisa de Socio's*/
//atraves do id procura pelo Socio na lista ligada de socios. De notar que o ID é a maneira mais viável para diferenciar sócios tendo em conta q não existem socios com IDs iguais
//Se encontrar o socio set o int a 1 e devolve o ponteira para esse socio, se não encontrar set o int a 0 e devolve null_socio (inicializado em estruturas_de_dados.c)
Socio* pesquisar_socio(char*,int*);

/*inserção de nodes*/
//verifica (para prevenir possível erro no input) se já existe algum socio com o id lido, na lista.
//Se não existir, cria o Socio e insere-o à entrada da lista e devolve 1; se já existir devolve 0 e liberta memoria; se houver erro na alocação de mem devolve -1
int inserir_socio(char*,char*);
//verifica (para prevenir possíveis erros no input) se o socio com o id associado ao pagamento existe na lista ligada de socios.
//Se não existir devolve 0; se existir, cria dinamicamente um node com a qntia desse pagamento e adiciona-o à entrada da lista de pagamentos desse socio (e devolve 1); se houver erro na alocação de mem devolve -1.
int inserir_pagamento(char*,float);

/*free all*/
//para desalocar toda a memória alocada na criação das estruturas de dados do programa
void free_all();

/*func auxiliares */
//devolve 1 se as strings foram iguais; devolve 0 se forem diferentes
int str_equals(char*,char*);
//percorre lista de pagamentos do Socio e
//devolve soma dos pagamentos efetuado pelo Socio
float get_soma_pagamentos(Socio);

#endif
