//Eduardo Filipe Ferreira da Cruz
/********
Este programa serve para gerar inputs aleatórios para o programa desenvolvido em main.c
Para que funcione corretamente são necessários dois ficheiros, nomes.txt e apelidos.txt,
que possuem, respetivamente, exemplos de primeiros nomes e de apelidos utilizados em Portugal.
*********
Para compilar o programa:
> gcc random_input_generator.c -Wall -o random_input_generator
ou
> make
*********
Para executar o programa:
>./random_input_generator nome_de_fich_para_escrita_da_listagem_de_socios.txt nome_de_fich_para_escrita_da_listagem_de_pagamentos.txt
ou
>./random_input_generator
*********/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DIM 1024
#define FIRST_ID 10000
#define USERS_QNT 100 /*numero de inscritos no clube a gerar*/
#define PAYMENT_QNT 250 /*numero de pagamentos a gerar*/
#define QUOTA_ANUAL 50

int count_file_lines(char *filename);
int file_to_array(char* filename,char *array[]);
int get_one_line(FILE * fich, char *linha, int lim);

int main(int argc, char *argv[]){
	int id,i;
	int count_nomes,count_apelidos;
	srand(time(NULL)); //set seed
	/*files*/
	char output0_filename[MAX_DIM+1];
	char output1_filename[MAX_DIM+1];
	FILE *file0=NULL; /*ficheiro com pares NOME ID*/
	FILE *file1=NULL;	/*ficheiro com pares ID QUANTIA_PAGA*/
	if(argc>=2){
		file0= fopen(argv[1],"w");
		if(file0==NULL)
			fprintf(stderr,"Erro a criar ficheiro para escrita de pares (Nome	ID_Number)!\n");

		file1= fopen(argv[2],"w");
		if(file1==NULL)
			fprintf(stderr,"Erro a criar ficheiro para escrita de pares (ID_Number	Qntia_Paga)!\n");
	}

	while(file0==NULL){
		printf("Nome do ficheiro para escrita de pares (Nome	ID_Number): ");
		if(get_one_line(stdin,output0_filename,MAX_DIM+1)==EOF)
			return 0;
		file0= fopen(output0_filename,"w");
		if(file0==NULL)
			fprintf(stderr,"Erro a criar ficheiro para escrita de pares (Nome	ID_Number)!\n");
	}

	while(file1==NULL){
			printf("Nome do ficheiro para escrita de pares (ID_Number	Qntia_Paga): ");
			if(get_one_line(stdin,output1_filename,MAX_DIM+1)==EOF)
				return 0;
			file1= fopen(output1_filename,"w");
			if(file1==NULL)
				fprintf(stderr,"Erro a criar ficheiro para escrita de pares (ID_Number	Qntia_Paga)!\n");
	}

	/*aqui ja tenho assegurados os ficheiros para escrita*/
	/*contar numero de linhas em nomes.txt e em apelidos.txt*/
	if((count_nomes=count_file_lines("nomes.txt"))==-1){
		fprintf(stderr,"Erro a abrir/encontrar ficheiro para leitura de nomes");
		return 0;
	}
	if((count_apelidos=count_file_lines("apelidos.txt"))==-1){
		fprintf(stderr,"Erro a abrir/encontrar ficheiro para leitura de apelidos");
		return 0;
	}

	char *nomes[count_nomes];
	if(file_to_array("nomes.txt",nomes)==-1)
		fprintf(stderr,"Erro: não há espaço em memória para alocar mais nomes");

	char *apelidos[count_apelidos];
	if(file_to_array("apelidos.txt",apelidos)==-1)
		fprintf(stderr,"Erro: não há espaço em memória para alocar mais apelidos");



	//gerar nomes (first_name+last_name) com o id para a respetiva pessoa
	//escrever no file0
	fprintf(file0,"NOME\tID\n"); //cabeçalho
	for(id=FIRST_ID;id<FIRST_ID+USERS_QNT;id++){
		fprintf(file0,"%s %s\t%d\n",nomes[rand()%count_nomes],apelidos[rand()%count_apelidos],id);
	}


	//intervalo de ids gerados = [FIRST_ID;FIRST_ID+USERS_QNT]
	//quota anual = 50euros
	//gerar listagem de pagamentos
	fprintf(file1,"ID\tQUANTIA_PAGA\n"); //cabeçalho
	for(i=0;i<PAYMENT_QNT;i++){
		//rand()%(max+1-min)+min
		id=rand()%(FIRST_ID+USERS_QNT-FIRST_ID)+FIRST_ID;
		fprintf(file1,"%d\t%d.%d\n",id,rand()%QUOTA_ANUAL+1,rand()%100); //escrever no file1
	}

	fclose(file0);
	fclose(file1);

	return 0;
}

int count_file_lines(char *filename){
	int i=0;
	FILE *file=NULL;
	char aux[MAX_DIM+1];

	file=fopen(filename,"r");
	if(file==NULL){
		return -1;
	}

	while(get_one_line(file,aux,MAX_DIM+1)!=EOF)
		i++;

	fclose(file);
	return i;
}

int file_to_array(char* filename,char *array[]){ //devolve -1 se n houver espaço suficiente em memoria; devolve 1 se sucessfull
	FILE *file=NULL;
	char line[MAX_DIM+1];
	int i=0;
	file=fopen(filename,"r");

	while(get_one_line(file,line,MAX_DIM+1)!=EOF){
		array[i]=(char*)malloc(strlen(line)+1); //+1 para o /0
		if(array[i]==NULL)
			return -1;
		strcpy(array[i++],line);
	}
	return 1; //sucessfull
	fclose(file);
}

/* get next line from input - "lim" includes the terminating zero */
int get_one_line(FILE * fich, char *linha, int lim) {
    int c, i;
    i = 0;
    while (isspace(c = fgetc(fich)));
    if (c != EOF) {
        if (!iscntrl(c)) linha[i++] = c;
    } else
        return c;

    for (; i < lim - 1;) {
        c = fgetc(fich);
        if (c == EOF)
            return c;
        if (c == '\n')
            break;
        if (!iscntrl(c)) linha[i++] = c;
    }
    linha[i] = 0;
    while ((c != EOF) && (c != '\n'))
        c = fgetc(fich);
    return c;
}
