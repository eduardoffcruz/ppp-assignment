//Eduardo Filipe Ferreira da Cruz
/********
Para compilar o programa:
> gcc main.c estruturas_de_dados.c -Wall -o main
ou
> make
*********
Para executar o programa:
> ./main nome_do_fich_com_listagem_de_socios.txt nome_do_fich_com_listagem_de_pagamentos.txt nome_do_fich_pra_escrita_de_quotas_pagas.txt nome_do_fich_pra_escrita_de_quotas_atrasadas.txt
ou
> ./main
*********
As linhas dos ficheiros de texto devem seguir a seguinte estrutura:
	nome_do_fich_com_listagem_de_socios.txt:
		NOME\tID\n

	nome_do_fich_com_listagem_de_pagamentos.txt:
		ID\tQUANTIA_PAGA\n

	nome_do_fich_pra_escrita_de_quotas_pagas.txt:
		NOME\tID\tQUANTIA_TOTAL_PAGA\tQUANTIA_PAGA_A_MAIS_A_SER_DESCONTADA_NO_ANO_SEGUINTE\n

	nome_do_fich_pra_escrita_de_quotas_atrasadas.txt:
		NOME\tID\tQUANTIA_TOTAL_PAGA\tQUANTIA_QUE_FALTA_PAGAR\tTIPO_DE_DIVIDA\n
*********
Estruturas de Dados utilizadas:
	Os dados dos ficheiros de texto foram carregados para
  	listas ligadas simples baseadas em gestão dinâmica de memória.
	Existe uma lista ligada com Socio's, e cada Socio possui uma lista
		ligada com os vários pagamentos registados com o seu ID no ficheiro
		com a listagem de pagamentos.
*********
Notas:
	Todos os sócios possuem um ID diferente. Um ID consiste num número.
	Supõe-se que, se um sócio se registar agora, ser-lhe-há atribuído
		um ID cujo número é uma unidade (1) superior ao do ID do último
		sócio que foi registado. O novo sócio é adicionado ao fim da listagem de sócios.
	Para o programa funcionar corretamente não é necessário existir esta ordem de registo,
		mas, existindo, e fazendo sentido num contexto de vida real,por questões de organização
		e de facilidade	de leitura, podemos prever que as listagens geradas pelo programa seguem
		um ordenamento decrescente, do sócio registado à menos tempo até ao sócio registado à
		mais tempo, ou seja, com mais anos no club. Este ordenamento é benéfico num contexto
		real, já que permite aferir com mais facilidade o cumprimento do pagamento de quotas
		por parte de novos sócios do club.
	Cada linha da listagem de pagamentos consiste num pagamento efetuado por um sócio no ano correspondente a essa listagem.
	Situações de possíveis erros no input, nomeadamente,
		pagamentos cujo ID não esteja associado a nenhum sócio,
		pagamentos com quantias anómalas,
		sócios repetidos,
		são alertados na consola e as listagens de output são geradas
		corretamente, ignorando os erros que deverão ser verificados e corrigidos posteriormente.
	Os ficheiros de input foram gerados através do programa random_input_generator.c que gera
		dois ficheiros com a disposição e as características mencionadas acima. Depois de serem
		gerados, decidi adicionar, propositadamente, aos dois ficheiros, três erros que serão detetados por este
		programa aquando da sua execução e que, como se poderá verificar, em nada influenciarão
		o correto funcionamento deste programa.
*********/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "estruturas_de_dados.h"

#define BUF_SIZE 1024
#define QUOTA_ANUAL 50

/*Funções*/
int carregar_socio(char* linha);
int carregar_pagamento(char *linha);
int is_valid_value(char *str);
int get_one_line(FILE * fich, char *linha, int lim);

int main(int argc, char *argv[]){
	char buffer[BUF_SIZE+1];
	FILE *fp_socios=NULL,*fp_pagamentos=NULL; //input
	FILE *fp_quotas_pagas=NULL,*fp_quotas_atrasadas=NULL; //output
	int i=0,k;
	float pago,dif; //para guardar quantia paga pelo socio
	struct elem_lista_socios *node_socio; //node da lista ligada de socios
	/*pedir nomes de ficheiros de input e de output e proceder com a respetiva abertura dos mesmos*/
	if(argc==5){
		fp_socios=fopen(argv[1],"r"); //leitura
		if(fp_socios==NULL)
			fprintf(stderr,"ERRO: Ficheiro '%s' inexistente\n",argv[1]);

		fp_pagamentos=fopen(argv[2],"r"); //leitura
		if(fp_pagamentos==NULL)
			fprintf(stderr,"ERRO: Ficheiro '%s' inexistente\n",argv[2]);

		fp_quotas_pagas=fopen(argv[3],"w"); //escrita
		if(fp_quotas_pagas==NULL)
			fprintf(stderr,"ERRO: Impossível criar o ficheiro para escrita '%s'\n",argv[3]);

		fp_quotas_atrasadas=fopen(argv[4],"w"); //escrita
		if(fp_quotas_atrasadas==NULL)
			fprintf(stderr,"ERRO: Impossível criar o ficheiro para escrita '%s'\n",argv[4]);
	}

	while(fp_socios==NULL){
		printf("Nome de ficheiro com listagem de Sócios: ");
		if(get_one_line(stdin,buffer,BUF_SIZE+1)==EOF)
			return 0;
		fp_socios=fopen(buffer,"r"); //leitura
		if(fp_socios==NULL)
			fprintf(stderr,"ERRO: Ficheiro '%s' inexistente\n",buffer);
	}
	while(fp_pagamentos==NULL){
		printf("Nome de ficheiro com listagem de Pagamentos: ");
		if(get_one_line(stdin,buffer,BUF_SIZE+1)==EOF)
			return 0;
		fp_pagamentos=fopen(buffer,"r"); //leitura
		if(fp_pagamentos==NULL)
			fprintf(stderr,"ERRO: Ficheiro '%s' inexistente\n",buffer);
	}
	while(fp_quotas_pagas==NULL){
		printf("Nome de ficheiro para escrita de listagem de Quotas em Dia : ");
		if(get_one_line(stdin,buffer,BUF_SIZE+1)==EOF)
			return 0;
		fp_quotas_pagas=fopen(buffer,"w"); //escrita
		if(fp_quotas_pagas==NULL)
			fprintf(stderr,"ERRO: Impossível criar o ficheiro para escrita '%s'\n",buffer);
	}
	while(fp_quotas_atrasadas==NULL){
		printf("Nome de ficheiro para escrita de listagem de Quotas em Atraso: ");
		if(get_one_line(stdin,buffer,BUF_SIZE+1)==EOF)
			return 0;
		fp_quotas_atrasadas=fopen(buffer,"w"); //escrita
		if(fp_quotas_atrasadas==NULL)
			fprintf(stderr,"ERRO: Impossível criar o ficheiro para escrita '%s'\n",buffer);
	}
	/*aqui já terei assegurado a abertura de todos os ficheiros de leitura e escrita necessários*/
	inicializar_lista_socios();
	/*leitura linha a linha do ficheiro com listagem de sócios + carregamento de sócios para lista ligada*/
	while(get_one_line(fp_socios, buffer, BUF_SIZE+1)!=EOF){
		if(i==0){
			i++;
			continue; //saltar linha 0 (cabeçalho)
		}
		if((k=carregar_socio(buffer))==-1){
			fprintf(stderr,"ERRO: ao alocar memória para Socio [malloc]\n");
			return 0;
		}
		else if(k==0)
			fprintf(stderr,"AVISO: Sócio repetido na linha %d !\n",i);
		i++;
	}
	/*aqui a lista ligada de socios já se encontrará preenchida*/
	i=0;
	/*é necessário agora preencher as listas ligadas com os pagamentos feitos, registados no ficheiro de pagamentos*/
	while(get_one_line(fp_pagamentos, buffer, BUF_SIZE+1)!=EOF){
		if(i==0){
			i++;
			continue; //saltar linha 0 (cabeçalho)
		}
		if((k=carregar_pagamento(buffer))==-1){
			fprintf(stderr,"ERRO: ao alocar memória para Socio [malloc]\n");
			return 0;
		}
		else if(k==-2)
			fprintf(stderr,"AVISO: Pagamento com quantia anómala na linha %d !\n",i);
		else if(k==0)
			fprintf(stderr,"AVISO: Não existe nenhum sócio com o ID do pagamento na linha %d !\n",i);
		i++;
	}
	/*aqui todas as estruturas de dados já se encontram preenchidas com todos os dados dos ficheiros de input*/
	fclose(fp_socios);
	fclose(fp_pagamentos);
	fprintf(fp_quotas_pagas,"NOME\tID\tQUANTIA_TOTAL_PAGA\tDESCONTADO_NO_ANO_SEGUINTE\n"); //cabeçalho
	fprintf(fp_quotas_atrasadas,"NOME\tID\tQUANTIA_TOTAL_PAGA\tQUANTIA_QUE_FALTA_PAGAR\tTIPO_DE_DIVIDA\n"); //cabeçalho
	/*percorrer a lista de socios e verificar, para cada um, se tem ou não quotas por pagar, e preeencher respetivos ficheiros de output*/
	node_socio=get_raiz_lista_socios();
  while(node_socio!=NULL){
    pago=get_soma_pagamentos(node_socio->socio);
		if(QUOTA_ANUAL-pago<=0){//socio tem quotas em dia
			dif=pago-QUOTA_ANUAL; //para reutilizar a var pago
			fprintf(fp_quotas_pagas,"%s\t%s\t%.2f\t%.2f\n",node_socio->socio.nome,node_socio->socio.id,pago,dif);
		}
		else{//tem quotas por pagar
			dif=QUOTA_ANUAL-pago;
			if(dif==QUOTA_ANUAL) //divida total
				fprintf(fp_quotas_atrasadas,"%s\t%s\t%.2f\t%.2f\tTOTAL\n",node_socio->socio.nome,node_socio->socio.id,pago,dif);
			else
				fprintf(fp_quotas_atrasadas,"%s\t%s\t%.2f\t%.2f\tPARCIAL\n",node_socio->socio.nome,node_socio->socio.id,pago,dif);
		}
    node_socio=node_socio->seguinte;
  }
	fclose(fp_quotas_pagas);
	fclose(fp_quotas_atrasadas);
	/*free all allocated memory*/
	free_all();
  return 0;
}

int carregar_socio(char* linha){ //devolve -1 se n houver espaco para alocar, 0 se socio ja existir na lista, 1 if sucessfull
	int i=0,j=i;
	char ch;
	char aux[BUF_SIZE+1];
	char *nome,*id;
	while((ch=linha[i++])!='\0'){
		aux[j++]=ch;
		if(ch=='\t'){ //nome em aux
			aux[j-1]='\0'; //'\0' ocupa o lugar de '\t'
			nome=(char*)malloc(sizeof(char)*(strlen(aux)+1)); //+1 para '\0'
			if(nome==NULL)
				return -1;
			strcpy(nome,aux);
			j=0;
		}
	}
	aux[j]='\0';
	//id em aux
	id=(char*)malloc(sizeof(char)*(strlen(aux)+1)); //de notar que sizeof(char) neste caso seria desnecessário já que sizof(char) == 1, mas coloquei por ser boa prática
	if(id==NULL)
		return -1;
	strcpy(id,aux);

	//inserir socio, criado a partir da linha, na lista de socios
	return inserir_socio(nome,id);
}

int carregar_pagamento(char *linha){ //devolve 0 se nao existir nenhum socio com o id registado no pagamento, devolve -1 se erro no mallor, devolve -2 se o valor pago for inválido
	int i=0,j=i;
	float valor;
	char ch;
	char aux[BUF_SIZE+1];
	char *id;
	while((ch=linha[i++])!='\0'){
		aux[j++]=ch;
		if(ch=='\t'){ //id em aux
			aux[j-1]='\0';
			id=(char*)malloc(sizeof(char)*(strlen(aux)+1));
			if(id==NULL)
				return -1;
			strcpy(id,aux);
			j=0;
		}
	}
	aux[j]='\0';

	//quantia_paga em aux
	if(is_valid_value(aux)==0){ //para assegurar q o valor é um float válido (para evitar problemas com funcao atof e para detetar valores anómalos)
		return -2;
	}
	//converter string para float
	valor=atof(aux);

	return inserir_pagamento(id,valor);

}

int is_valid_value(char* str){ //devolve 0 se str nao tiver o formato de um valor monetário válido, devolve 1 otherwise
	int i=0,flag=0,count=0;
	char ch;
	while((ch=str[i++])!='\0'){
		count++;
		if(!isdigit(ch)){
			if(ch=='.'&&flag==0){ //valores negativos, com mais de um . ou com caracteres inválidos serão completamente descartados por se considerarem inválidos
				flag=1;
				if(count-1>7) //valores acima de 9999999.99euros sao considerados inválidos
					return 0;
				count=0; //para comecar a contar casas decimais
			}
			else
				return 0;
		}
	}
	if(count>2) //considera inválido valores com mais de 2 casas decimais
		return 0;

	return 1;
}

//função retirada dos exercicíos disponibilizados em aula e adaptada para não descartar controled characters como '\t'
/* get next line from input - "lim" includes the terminating zero */
int get_one_line(FILE * fich, char *linha, int lim) {
    int c, i;
    i = 0;
    while (isspace(c = fgetc(fich)));
    if (c != EOF) {
    	linha[i++] = c;
    } else
        return c;

    for (; i < lim - 1;) {
        c = fgetc(fich);
        if (c == EOF)
            return c;
        if (c == '\n')
            break;
        linha[i++] = c;
    }
    linha[i] = 0;
    while ((c != EOF) && (c != '\n'))
        c = fgetc(fich);
    return c;
}
