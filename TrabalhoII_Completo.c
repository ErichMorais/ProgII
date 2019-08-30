#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/*
Nomes:Erich Morais e Samuel Formigheri
Trabalho de implementação
****IMPORTANTE****
Verificar se o Arquivo está no local adequado pela leitura do mesmo(função "separa",linha 295),
podendo ser execudatado pelo programa.
*/

/*------------------STRUCTS----------------- */
typedef struct filme {
	char nome[100];
	int ano;
	struct filmeAtores *atores;
	struct diretor *diretores;
	struct filme *prox;
	struct filme *ant;
}filme;

typedef struct ator {
	char nome[1000];
	struct filmeAtores *filmes;
	struct ator *ant;
	struct ator *prox;
}ator;

typedef struct diretor {
	char nome[1000];
	diretor *prox;
}diretor;

typedef struct filmeAtores {
	char personagem[1000];
	struct filme *filme;
	struct ator *ator;
	struct filmeAtores *prox;
}filmeAtores;

typedef struct filme_duplicado {
	char nome[1000];
	int ano;
	struct filme_duplicado *prox;
}filme_duplicado;

 struct filme_duplicado *filmes_duplicados = NULL;

/*------------------INSERÇÕES EM GERAL----------------- */	
struct ator *insereAtor(struct ator **inicio, struct ator **fim, char nome[100]){
	struct ator * aux = *inicio;
	
    struct ator * ator = (struct ator *) malloc(sizeof(struct ator));
    strcpy(ator->nome, nome);
    ator->filmes = NULL;
    
    if((*inicio) == NULL || (stricmp(aux->nome, ator->nome) > 0)){
		ator->prox = *inicio;
		ator->ant = NULL;
		if (*inicio != NULL) (*inicio)->ant = ator;
		else *fim = ator;
		*inicio = ator;
	}else {
		//(inserir ordenado)
		while((aux->prox != NULL) && (stricmp(aux->prox->nome, ator->nome) <= 0)){
			aux = aux->prox;
		}
		
		//verifica se ator já foi cadastrado
		if(stricmp(aux->nome, ator->nome) == 0){
			//se sim, retorna o cadastrado
			ator = aux;
		} else {
			//se não, insere um novo e retorna
			ator->prox = aux->prox;
			ator->ant = aux;
			aux->prox = ator;
			
			if(ator->prox == NULL){
				(*fim)=ator;
			} else {
				ator->prox->ant = ator;
			}
		}
	}	
	return ator;
}

void insereFilmeDiretor(struct filme **filme, char nome[100]) {
    struct diretor *diretor = (struct diretor *)malloc(sizeof(struct diretor));
    strcpy(diretor->nome, nome);
    diretor->prox = NULL;
    
    if((*filme)->diretores == NULL){
    	(*filme)->diretores = diretor;
	} else {
		while((*filme)->diretores->prox != NULL){
			(*filme)->diretores = (*filme)->diretores->prox;
		}
		(*filme)->diretores->prox = diretor;
	}	
}

void insereAtorFilme(struct filme **filme, struct filmeAtores *filmeAtores){
	//busca atores do filme
	struct filmeAtores *atoresDoFilme = (*filme)->atores;
	struct filmeAtores *aux = NULL;
	//verifica se filme já tem atores
	if((*filme)->atores == NULL  || stricmp(filmeAtores->ator->nome, atoresDoFilme->ator->nome) < 0)  {
    	 filmeAtores->prox = (*filme)->atores;
         (*filme)->atores = filmeAtores;
	} else {
		aux = atoresDoFilme;
		//procura o filme na lista do ator, se ja existir não insere novamente
		while(aux->prox != NULL){
			if (stricmp((aux->prox->ator)->nome, filmeAtores->ator->nome) == 0) {
				return;
			}
			aux = aux->prox;
		}
    	//se não insere um novo e retorna
    	filmeAtores->prox = atoresDoFilme->prox;
    	(*filme)->atores->prox = filmeAtores;
	}
}

void insereFilme(struct filme **inicio, struct filme **fim,  struct filme *filme){
	struct filme * aux = *inicio;
    
    if((*inicio) == NULL || (stricmp(aux->nome, filme->nome) > 0)){
		filme->prox = *inicio;
		filme->ant = NULL;
		if (*inicio != NULL) (*inicio)->ant = filme;
		else *fim = filme;
		*inicio = filme;
		return;
	}else {
		//faz laço até encontrar filme com nome maior (ordenado)
		while((aux->prox != NULL) && (stricmp(aux->prox->nome, filme->nome) < 0)){
			aux = aux->prox;
		}
		
		//se não insere um novo e retorna
		filme->prox = aux->prox;
		filme->ant = aux;
		aux->prox = filme;
		// inserção no final da lista
		if(filme->prox == NULL){
			(*fim)=filme;
		} else {
			filme->prox->ant = filme;
		}
	}	
}

void insereFilmeAtor(struct ator **ator, struct filmeAtores *filmeAtores){
	//busca filmes do ator
	struct filmeAtores *filmesDoAtor = (*ator)->filmes;
		
	struct filmeAtores *aux = NULL;
	//verifica se o ator tem filmes
	if((*ator)->filmes == NULL  || (filmeAtores)->filme->ano < filmesDoAtor->filme->ano){
    	 filmeAtores->prox = (*ator)->filmes;
         (*ator)->filmes = filmeAtores;
	} else {
		aux = filmesDoAtor;
		//procura o filme na lista do ator, se sim não insere novamente
		while(aux->prox != NULL){
			if (stricmp((aux->prox->filme)->nome, (filmeAtores->filme)->nome) == 0) {
				return;
			}
			aux = aux->prox;
		}
		while((filmesDoAtor->prox != NULL) && (filmesDoAtor->prox->filme)->ano <= (filmeAtores->filme)->ano){
			filmesDoAtor = filmesDoAtor->prox;
		}
			
    	//se não insere um novo e retorna
    	filmeAtores->prox = filmesDoAtor->prox;
    	filmesDoAtor->prox = filmeAtores;
	}
}

void insereFilmeDuplicado(struct filme_duplicado **inicio, struct filme *filme) {
	
	struct filme_duplicado *filme_dup = (struct filme_duplicado *) malloc(sizeof(struct filme_duplicado));
	strcpy(filme_dup->nome, filme->nome); 
	filme_dup->ano = filme->ano;
	
	struct filme_duplicado * aux = *inicio;
    
    if((*inicio) == NULL){
		filme_dup->prox = *inicio;
		*inicio = filme_dup;
		return;
	}else {
		//faz laço até encontrar filme com nome maior (ordenado)
		while((aux->prox != NULL) && (stricmp(aux->prox->nome, filme_dup->nome) != 0)){
			aux = aux->prox;
		}
		
		if (stricmp(aux->prox->nome, filme_dup->nome) == 0) {
			return;
		}
		//se não insere um novo e retorna
		filme_dup->prox = aux->prox;
		aux->prox = filme_dup;
	}
}

/*------------------RELACIONA UM TIPO COM O OUTRO----------------- */
struct filme* existeFilme(struct filme *inicio_filmes, char nome[100], int ano){
	if (ano == 0) {
		ano = 9999;
	}
	if(inicio_filmes != NULL){
		while(inicio_filmes != NULL && stricmp(inicio_filmes->nome, nome) != 0){
		   	inicio_filmes = inicio_filmes->prox;
		}	
		
		while(stricmp(inicio_filmes->nome, nome) == 0){
			if(stricmp(nome, inicio_filmes->nome) == 0 && ano == inicio_filmes->ano){
				return inicio_filmes;
			}
			inicio_filmes = inicio_filmes->prox;
		}
	}
	return NULL;
}



char *zStrtok(char *str, const char *delim) {  
  //Funçao para separar osatores, diretores, etc.
    static char *static_str=0;      /* var to store last address */
    int index=0, strlength=0;       /* integers for indexes */
    int found = 0;                  /* check if delim is found */

    /* delimiter cannot be NULL
    * if no more char left, return NULL as well
    */
    if (delim==0 || (str == 0 && static_str == 0))
        return 0;

    if (str == 0)
        str = static_str;

    /* get length of string */
    while(str[strlength])
        strlength++;

    /* find the first occurance of delim */
    for (index=0;index<strlength;index++)
        if (str[index]==delim[0]) {
            found=1;
            break;
        }

    /* if delim is not contained in str, return str */
    if (!found) {
        static_str = 0;
        return str;
    }

    /* check for consecutive delimiters
    *if first char is delim, return delim
    */
    if (str[0]==delim[0]) {
        static_str = (str + 1);
        return (char *)delim;
    }

    /* terminate the string
    * this assignmetn requires char[], so str has to
    * be char[] rather than *char
    */
    str[index] = '\0';

    /* save the rest of the string */
    if ((str + index + 1)!=0)
        static_str = (str + index + 1);
    else
        static_str = 0;

        return str;
}

void separa(struct filme **inicio_filmes, struct filme **fim_filmes, struct ator **inicio_atores, struct ator **fim_atores, char linha[1024]) {
	char *pch;
	char personagem[100];
	char nomeAtor[100];
	char *auxAtor;
	struct filme *aux = NULL;
	int result;
	
	pch = zStrtok(linha, ",");
	
	int cont = 0;
	struct filme *filme = (struct filme *) malloc(sizeof(struct filme));
	filme->ano = 0;
	filme->atores = NULL;
	filme->diretores = NULL;
	filme->prox = NULL;
	
	while(pch != NULL){
		
		
		if(strlen(pch) > 0){
				
			//nome
	  		if(cont == 0 && pch != ","){
	  			strcpy(filme->nome, pch); 
			}
			
			//ano
			if(cont == 1){
				if (pch != ",") {
					filme->ano = atoi(pch);
				}else{
					filme->ano = 9999;
				}
					
				if (strlen(filme->nome) > 0 && filme->nome != ",") {
					aux = existeFilme((*inicio_filmes), filme->nome, filme->ano);
					if (aux == NULL) {
						insereFilme(inicio_filmes, fim_filmes, filme);
					}else{
						insereFilmeDuplicado(&filmes_duplicados, aux);
						return;
					}
				}	
			}
			
			if(cont == 2 && pch != ","){
				 insereFilmeDiretor(&filme, pch);
			}
			
			if(cont >= 3){				
				if(strstr(pch, "dir:")){
					pch +=4;
					insereFilmeDiretor(&filme, pch);
					
				} else {
					memset(nomeAtor, 0, sizeof(nomeAtor));
					memset(personagem, 0, sizeof(personagem));
					
					//separa o personagem do nome do ator, se houver
					if(strstr(pch, "=")) {
						auxAtor = strtok(pch, "=");
						strcpy(nomeAtor, auxAtor); 	
						auxAtor = strtok(NULL, "=");
						strcpy(personagem, auxAtor); 
					}else{
						strcpy(nomeAtor, pch);
					}		
			   		//insere ator 
					struct ator * ator = insereAtor(inicio_atores, fim_atores, nomeAtor);
					
					//relacionamento entre ator e filme
    				struct filmeAtores *filmeAtores = (struct filmeAtores *) malloc(sizeof(struct filmeAtores));
    				strcpy(filmeAtores->personagem, personagem);
    				filmeAtores->filme = filme;
    				filmeAtores->ator = ator;
    				filmeAtores->prox = NULL;

					//relaciona ator com o filme
					insereFilmeAtor(&ator, filmeAtores);
					
					//relacionamento entre ator e filme
    				filmeAtores = (struct filmeAtores *) malloc(sizeof(struct filmeAtores));
    				strcpy(filmeAtores->personagem, personagem);
    				filmeAtores->filme = filme;
    				filmeAtores->ator = ator;
    				filmeAtores->prox = NULL;
					insereAtorFilme(&filme, filmeAtores);
				
				}
			}
		}
		
		cont++;
		pch = zStrtok(NULL, ",");
	}
	
	
}

void lerArquivo(struct filme **inicio_filmes, struct filme **fim_filmes, struct ator **inicio_atores, struct ator **fim_atores){
	char linha_leitura[1024]; 
	char linha_certa[1024];	
	FILE *arq;
	arq = fopen("resumo.txt", "rb");
	printf("Aguarde processando... \n");
	if(arq == NULL) printf("Arquivo não encontrado.");
	else{
		while(fgets(linha_leitura, sizeof(linha_leitura), arq)){
			int tam = strlen(linha_leitura);
			memset(linha_certa, 0, sizeof(linha_certa));
			strncpy(linha_certa, linha_leitura, tam-2);

			separa(inicio_filmes, fim_filmes, inicio_atores, fim_atores, linha_certa);
		}
	}
}
/*----------------------EDIÇÃO DE LISTAS--------------------------*/
void remover(struct filmeAtores **inicio, char nome[1024])
{
	struct filmeAtores*ptaux;
	if (*inicio==NULL) return;
	if (stricmp((*inicio)->filme->nome,nome)!=0) 
	{
		remover(&((*inicio)->prox), nome);
		return;
	}
	ptaux=*inicio;
	(*inicio)=(*inicio)->prox;
	free(ptaux);
}

void removerFilmeAtor(struct filmeAtores *inicio, char nome[1024])
{
	while (inicio != NULL) {
		
		filmeAtores **filmes = &inicio->ator->filmes;
 		filmeAtores *aux = NULL;
		filmeAtores *ant = *filmes;
		remover(filmes, nome);
		inicio = inicio->prox;
	}
	
}

int removerFilme(struct filme **inicio, struct filme **fim, char nome[1024])
{
	struct filme *aux , *ant, *prox ;
	struct filmeAtores *atoresDoFilme;
	if (*inicio == NULL) {
		printf("Filme nao encontrado\n");
		return 0; 
	}	
	if (stricmp((*inicio)->nome,nome)==0)
	{
		removerFilmeAtor((*inicio)->atores, nome);
		free((*inicio)->atores);
		aux = *inicio ;
		prox = (*inicio)->prox ;
		ant = (*inicio)->ant ;
		if (prox != NULL) prox->ant = (*inicio )->ant;
		else (*fim)= (*inicio)->ant ;
		if (ant != NULL ) ant->prox = (*inicio)->prox;
		else (*inicio) = (*inicio)->prox;
		free (aux);
		printf("Filme removido com sucesso!\n");
		return 1;
	}
	return removerFilme(&((*inicio)->prox),fim, nome);
	
}


void inserirNovoFilme(struct filme **inicio_filmes, struct filme **fim_filmes, struct ator **inicio_atores, struct ator **fim_atores){
	char linha[1024], opcao[100], aux[100], nome[100], ano[100];
	int i = 0;
	
	memset(linha, 0, sizeof(linha));
	memset(opcao, 0, sizeof(opcao));
	memset(aux, 0, sizeof(aux));
	memset(nome, 0, sizeof(nome));
	memset(ano, 0, sizeof(ano));
	
	printf("\nDigite o nome do filme:\n");
	gets(opcao);
	strcpy(nome, opcao);
	strcat(opcao, ",");
	strcat(linha, opcao);
	
	printf("\nDigite o ano do filme (digite 0 para não informar):\n");
	gets(opcao);
	strcpy(ano, opcao);
	strcat(opcao, ",");
	strcat(linha, opcao);
	
	if (existeFilme(*inicio_filmes, nome, atoi(ano)) != NULL) {
		printf("\nFilme '%s (%s)' ja existente na lista!\n", nome, ano);
		return;
	}
	
	printf("\nDigite os diretores: (para sair insira um valor vazio)\n");
	gets(opcao);
	while(strcmp(opcao, "") != 0){
		if(i == 0){
			strcat(opcao, ",");
		} else {
			strcpy(aux, "dir:");
			strcat(aux, opcao);
			strcat(aux, ",");
			
			strcpy(opcao, aux);
		}
		i++;
		
		strcat(linha, opcao);	
		gets(opcao);
	}
	
	
	printf("\nDigite os atores: (para sair insira um valor vazio)\n");
	printf("IMPORTANTE: Se o ator fizer algum personagem, insira como no exemplo: ator=personagem\n");
	gets(opcao);
	while(strcmp(opcao, "") != 0){
		strcat(opcao, ",");
		strcat(linha, opcao);			
		gets(opcao);
	}
	
	linha[(strlen(linha) -1)] = '\0'; 
	separa(inicio_filmes, fim_filmes, inicio_atores, fim_atores, linha);	
	printf("Filme incluido com sucesso\n");	
}

/*----------------------PRINTS--------------------------*/
void escreverFilmes(struct filme *filmes){
	if(filmes != NULL){
		printf("%s\n", filmes->nome);
		escreverFilmes(filmes->prox);
	}
}

void escreverFilmesDuplicados(struct filme_duplicado *filmes_duplicados){
	if(filmes_duplicados != NULL){
		printf("%s (%i)\n", filmes_duplicados->nome, filmes_duplicados->ano == 9999 ? 0 : filmes_duplicados->ano);
		escreverFilmesDuplicados(filmes_duplicados->prox);
	}
}

void escreverFilmesDesc(struct filme *filmes){
	if(filmes != NULL){
		printf("%s\n", filmes->nome);
		escreverFilmesDesc(filmes->ant);
	}
}

void escreverAtores(struct ator *atores){
	if(atores != NULL){
		printf("%s\n", atores->nome);
		escreverAtores(atores->prox);
	}
}

void escreverAtoresDesc(struct ator *atores){
	if(atores != NULL){
		printf("%s\n", atores->nome);
		escreverAtoresDesc(atores->ant);
	}
}


void escreverAtorFilmes(struct ator *atores){
	char nome[1024];
	printf("\nDigite o nome do ator:\n");
	gets(nome);
	
	while((atores != NULL) && stricmp(atores->nome, nome) != 0){
		atores = atores->prox;
	}
	
	if(atores != NULL){
		printf("\nFilmes encontrados: \n");
		struct filmeAtores *filmes = atores->filmes;
					
		if (filmes == NULL) {
			printf("Nenhum filme encontrado\n");
		}
		
		while(filmes != NULL){
			printf("%s - %i\n", filmes->filme->nome, filmes->filme->ano == 9999 ? 0 : filmes->filme->ano);
			filmes = filmes->prox;
		} 
	}else{
			printf("Ator '%s' nao encontrado!\n", nome);
	}
}

void escreverAtorAtores(struct ator *atores){
	char nome[1024];
	printf("\nDigite o nome do ator desejado:\n");
	gets(nome);
	
	printf("Atores que trabalham com determinado ator: '%s'\n", nome);
	
	while((atores != NULL) && stricmp(atores->nome, nome) != 0){
		atores = atores->prox;
	}
	
	if(atores != NULL){
		struct filmeAtores *filmes = atores->filmes;					
		if (filmes == NULL) {
			printf("Nenhum filme encontrado\n");
		}
		
		while(filmes != NULL){
			struct filme *filme = filmes->filme;
			struct filmeAtores *atores = filme->atores;
			if (atores != NULL) {	
				while(atores != NULL){
					if (stricmp(atores->ator->nome, nome) != 0) {
						printf(" %s - Filme: %s (%i)\n", atores->ator->nome, filme->nome, filme->ano == 9999 ? 0 : filme->ano);
					}
					atores = atores->prox;
				} 
			}
			filmes = filmes->prox;
		} 
	}else{
		printf("Ator '%s' nao encontrado!\n", nome);
	}
}

void escreverDiretorAtores(struct filme *filme){
	char nome[1024];
	printf("\nDigite o nome do diretor:\n");
	gets(nome);
	
	printf("Atores que trabalharam com o diretor '%s'\n", nome);
	
	while (filme != NULL){	
		struct diretor *diretores = filme->diretores;
		if (diretores != NULL) {	
			while(diretores != NULL){
				if (stricmp(diretores->nome, nome) == 0) {
					struct filmeAtores *atores = filme->atores;
					if (atores != NULL) {	
						while(atores != NULL){
							printf(" %s - Filme: %s (%i)\n", atores->ator->nome, filme->nome, filme->ano == 9999 ? 0 : filme->ano);
							atores = atores->prox;
						} 
					}
				}		
				diretores = diretores->prox;
			} 
		}
		filme = filme->prox;
	}
}

void escreverPersonagemAtores(struct filme *filme){
	char personagem[1024];
	printf("\nDigite o nome do personagem:\n");
	gets(personagem);
	
	printf("\nAtores que interpretaram o personagem '%s':\n", personagem);
	while (filme != NULL){	
		struct filmeAtores *atores = filme->atores;
		if (atores != NULL) {	
			while(atores != NULL){
				if (stricmp(atores->personagem, personagem) == 0) {
					printf(" %s - Filme: %s (%i)\n", atores->ator->nome, filme->nome, filme->ano == 9999 ? 0 : filme->ano);
					break;
				}		
				atores = atores->prox;
			} 
		}
		filme = filme->prox;
	}
}

int escreverDiretorFilmes(struct filme *inicio, struct filme *fim, char nome[1024]){
	struct diretor *aux_dir;
	struct filme filmesDiretor[9999];
	struct filme aux;
	
	int maior = 0;
	int i = 0;
	
	while(inicio != NULL){
		maior = 0;
		aux_dir = inicio->diretores;
		
		while((aux_dir != NULL) && stricmp(aux_dir->nome, nome) != 0){
			aux_dir = aux_dir->prox;
		}
		
		if(aux_dir != NULL){
			filmesDiretor[i].ano = inicio->ano;
			strcpy(filmesDiretor[i].nome, inicio->nome);
			i++;
		}
		
		inicio = inicio->prox;
	}
	
	for(int x = 0; x < i; x++){
    	for(int y = x + 1; y < i; y++ ){
			if(filmesDiretor[x].ano > filmesDiretor[y].ano){
         		aux = filmesDiretor[x];
         		filmesDiretor[x] = filmesDiretor[y];
         		filmesDiretor[y] = aux;
      		}
    	}
  	}

	for(int n = 0; n<i; n++){
		struct filme auxFilme = filmesDiretor[n];
		printf("----- Filme: %s (%d)\n", auxFilme.nome, auxFilme.ano == 9999 ? 0 : auxFilme.ano);
	}
}

void escreveMenu() {
		printf("Digite uma opcao:\n");
		printf("\n1 - Listar atores"); //EXERCICIO 1 --------------------------------------------------------------------
		printf("\n2 - Listar filmes"); //EXERCICIO 3 --------------------------------------------------------------------
		printf("\n3 - Listar filmes de um determinado ator em ordem cronologica"); //EXERCICIO 2 ------------------------
		printf("\n4 - Editar lista filmes"); //EXECRCICIO 4 e 5 ---------------------------------------------------------
		printf("\n5 - Listar filmes de determinado diretor"); //EXERCICIO 6 ---------------------------------------------
		printf("\n6 - Atores que trabalharam com um ator");//EXERCICIO 7 ------------------------------------------
		printf("\n7 - Atores que trabalharam com um diretor");//EXERCICIO 8 ------------------------------------------
		printf("\n8 - Listar todos os filmes duplicados"); //EXERCICIO 9 ------------------------------------------------
		printf("\n9 - Todos os atores que interpretaram um determinado personagem");//EXERCICIO 10 ------------------------------------------
		printf("\n0 - Sair\n");
}

int main(){
	setlocale(LC_ALL,"Portuguese");
	char *item;	
	char *pch;
	char *ano;
	char linha[1024];
	char opcao[2],alf[2];
	char nome[1024];
	
	struct filme *inicio_filmes = NULL;
	struct filme *fim_filmes = NULL;
	struct ator *inicio_atores = NULL;
	struct ator *fim_atores = NULL;
	
	
	lerArquivo(&inicio_filmes, &fim_filmes, &inicio_atores, &fim_atores);
	
	escreveMenu();
 	gets(opcao);	 		
	while (atoi(opcao) != 0) {		
		switch (atoi(opcao)) {
			case 0: { break;}
			case 1: //Listar Atores - Exercicio 1
			{
				system("cls");
				printf("\n Escolha a opcao: \n <1> Alfabetica\n <2> Reversa \n ");
	            gets(alf);	            	
	            if(atoi(alf)==1){
	            escreverAtores(inicio_atores);
	            }else if(atoi(alf)==2){
	        	escreverAtoresDesc(fim_atores);
	        	}else{
	        		printf("Opcao invalida\n"); 
				}
				break;
			}
			case 2: //Listar Filmes	- Exercicio 3
			{
				system("cls");
				printf("\n Escolha a opcao: \n <1> Alfabetica\n <2> Reversa \n ");
	            gets(alf);	            	
	            if(atoi(alf)==1){
	            escreverFilmes(inicio_filmes);
	            }else if(atoi(alf)==2){
	        	escreverFilmesDesc(fim_filmes);
	        	}else{
	        		printf("Opcao errada\n");
				}
				break;			
			}
			case 3: //Listar filmes de um determinado ator em ordem cronologica" - Exercicio 2
			{
				system("cls");			
				escreverAtorFilmes(inicio_atores);
				break;
			}
			case 4: //Editar lista filmes - Exercicio 4 e 5
			{
				system("cls");
				 printf("\n Escolha a opcao: \n <1> Inserir\n <2> Remover \n ");
            	gets(alf);
            	if(atoi(alf)==1){
            		inserirNovoFilme(&inicio_filmes, &fim_filmes, &inicio_atores, &fim_atores);
            	}
            	else if(atoi(alf)==2){
					printf("Digite o titulo do filme que deseja remover\n");           	           	
					gets(nome);
					removerFilme(&inicio_filmes, &fim_filmes, nome);
					}else{
        		printf("Opcao errada\n");
				}
				break;
			}
			case 5: //Listar filmes de determinado diretor" - Exercicio 6 
			{
				system("cls");				
				printf("Nome do diretor: ");
				gets(nome);
				escreverDiretorFilmes(inicio_filmes, fim_filmes, nome);
				break;
			}
			case 6: // Atores que trabalharam com um ator - Exercicio 7 
			{
				system("cls");			
				escreverAtorAtores(inicio_atores);
				break;
			}
			case 7: // Atores que trabalharam com um diretor - Exercicio 8
			{
				system("cls");
				
				escreverDiretorAtores(inicio_filmes);
				break;
			}		
			case 8: //Listar todos os filmes duplicados - Exercicio 9
			{
				system("cls");
				
				escreverFilmesDuplicados(filmes_duplicados);
				break;
			}
			case 9: //Todos os atores que interpretaram um determnado personagem - Exercicio 10
			{
				system("cls");				
				escreverPersonagemAtores(inicio_filmes);
				break;
			}
		
		}
	 
	 	escreveMenu();
		gets(opcao);			
	}
	printf("\nFim da Execucao do programa\n");
}

