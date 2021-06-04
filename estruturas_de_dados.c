//Eduardo Filipe Ferreira da Cruz
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "estruturas_de_dados.h"

//Socio Vazio
static Socio null_socio={"","",NULL};
//Raiz da lista ligada de Socios (Estrutura Principal do programa)
static struct elem_lista_socios *raiz_lista_socios; //primeiro elemento da lista de socios

/*Implementação de Funções (declaradas em estruturas_de_dados.h)*/
void inicializar_lista_socios(){
	raiz_lista_socios=NULL;
}

void inicializar_lista_pagamentos(Socio *socio){
  socio->raiz_lista_pagamentos=NULL;
}

struct elem_lista_socios* get_raiz_lista_socios(){
  return raiz_lista_socios;
}

Socio* pesquisar_socio(char *id,int *encontrado){
  struct elem_lista_socios *aux;
  if(raiz_lista_socios==NULL){ //lista vazia
      *encontrado=0;
      return &null_socio;
  }

  //percorrer lista
  aux=raiz_lista_socios;
  while(aux!=NULL){
    if(str_equals(id,aux->socio.id)==1){
      *encontrado=1;
      return &(aux->socio);
	 }
    aux=aux->seguinte;
  }
  *encontrado=0;
  return &null_socio;
}

int inserir_socio(char *nome,char *id){
	struct elem_lista_socios *aux;
  int encontrado;
  pesquisar_socio(id,&encontrado);
  if(encontrado==1){
    //libertar memoria de strings, já que n serão usadas.
    free(nome);
		free(id);
    return 0;
  }

  aux=(struct elem_lista_socios *)malloc(sizeof(struct elem_lista_socios)); //criar espaco para novo node
  if(aux==NULL)
    return -1;
  //criar Socio a partir dos dados
	if((aux->socio.nome=(char*)malloc(sizeof(char)*(strlen(nome)+1)))==NULL)
		return -1;
	if((aux->socio.id=(char*)malloc(sizeof(char)*(strlen(id)+1)))==NULL)
		return -1;
  strcpy(aux->socio.nome,nome);
	strcpy(aux->socio.id,id);
	inicializar_lista_pagamentos(&aux->socio);
	//introduzir socio na lista de socios
  aux->seguinte=raiz_lista_socios;
  raiz_lista_socios=aux;

  return 1;
}

int inserir_pagamento(char* id, float quantia_paga){ //insere no inicio da lista
  struct pagamento *node_pagamento;
  Socio *socio;
  int encontrado;

  socio=pesquisar_socio(id,&encontrado);
  if(encontrado==0) //socio inexistente
    return 0; //POSSIVEL ERRO NO INPUT DOS PAGAMENTOS [ID ASSOCIADO A PAGAMENTO É INEXISTENTE]

  node_pagamento=(struct pagamento *)malloc(sizeof(struct pagamento)); //criar espaco para novo node
  if(node_pagamento==NULL)
    return -1;

  node_pagamento->quantia_paga=quantia_paga;
  node_pagamento->seguinte=socio->raiz_lista_pagamentos;
  socio->raiz_lista_pagamentos=node_pagamento;
  return 1;
}

void free_all(){
	struct elem_lista_socios *aux_node_socios;
	struct elem_lista_socios *aux1_node_socios; //aux_node_socios será deallocated, daí a necessidade de ter este aux1, para 'guardar' o node seguinte
	Socio *socio;
	struct pagamento *aux_pagamento;
	struct pagamento *aux1_pagamento;

	aux_node_socios=raiz_lista_socios; //raiz da lista ligada
	while(aux_node_socios!=NULL){
		aux1_node_socios=aux_node_socios->seguinte;
		socio=&aux_node_socios->socio;
		free(socio->nome);
		free(socio->id);
		aux_pagamento=socio->raiz_lista_pagamentos;
		while(aux_pagamento!=NULL){ //percorrer lista de pagamentos de cada socio e limpar
			aux1_pagamento=aux_pagamento->seguinte;
			free(aux_pagamento); //node pagamento
			aux_pagamento=aux1_pagamento;
		}
		free(aux_node_socios); //node socio
		aux_node_socios=aux1_node_socios;
	}
}

int str_equals(char *str1,char *str2){
  int i=0;

  while(str1[i]!='\0'){
    if(str2[i]=='\0')
      return 0;
    if(str1[i]!=str2[i])
      return 0;
    i++;
  }
  if(str2[i]!='\0')
    return 0;

  return 1; //s1 e s2 sao a mesma pessoa
}

float get_soma_pagamentos(Socio socio){
  float soma=0;
  struct pagamento *aux; //node da lista de pagamentos
  //percorrer lista de pagamentos de socio
  aux=socio.raiz_lista_pagamentos;
  while(aux!=NULL){
    soma+=aux->quantia_paga;
    aux=aux->seguinte;
  }
  return soma;
}
