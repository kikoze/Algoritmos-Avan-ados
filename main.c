#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * readLine lê as linhas de um ficheiro letra a letra
 * e aloca memória para um buffer com o tamanho da linha ldia
 * \param file ficheiro de onde são lidas as linhas
 * \param buffer string a ser preenchida com o que foi lido
 * \return linha do ficheiro lida
 */
char* readLine(FILE *file, char *buffer);

/**
 * naive verifica a existência de um padrão num determinado 
 * texto utilizado o naive algorithm de procura
 * \param file ficheiro onde é escrito o output
 * \param text texto onde um certo padrão deve ser procurado
 * \param pat padrão a encontrar
 */
void naive(FILE *file, char *text, char *pat);

/**
 * naive verifica a existência de um padrão num determinado 
 * texto utilizado o algoritmo Knuth-Morris-Pratt
 * \param file ficheiro onde é escrito o output
 * \param text texto onde um certo padrão deve ser procurado
 * \param pat padrão a encontrar
 */
void kmp(FILE *file, char *text, char *pat);

/**
 * safeMalloc faz o malloc e depois garante que a memória foi alocada
 * \param size tamanho em bytes da memória a alocar
 * \return ponteiro resultante do malloc de size bytes
 */
void* safeMalloc(size_t size);

/**
 * safeMalloc faz o malloc e depois garante que a memória foi alocada
 * \param size comprimento da memória a realocar
 * \return ponteiro resultante do realloc de size bytes
 */
void* safeRealloc(int size);

int main(int argc, char *argv[]){

    FILE *fin, *fout;
    char *line = NULL, *text = NULL;
    char *n_pat = NULL, *k_pat = NULL, *b_pat = NULL;
    int op_flag[4] = {0,0,0,0};

    if(argc < 2){
        exit(0);
    }
    /* Abrir ficheiro com instruções */
    fin = fopen("input", "r");
    if(fin == NULL)
        exit(EXIT_FAILURE);
    fout = fopen(argv[2], "w");
    if(fout == NULL)
        exit(EXIT_FAILURE);

    while (!feof(fin)){
        line = readLine(fin, line);
        /* Verificar a primeira letra da linha e a partir
        * daí fazer a operação desejada */
        if(line[0] == 'T'){
            op_flag[0] = 1;
            text = safeMalloc(sizeof(char)*(strlen(line)-1));
            strcpy(text, line+2);
        }else if(line[0] == 'N'){
            /* Naive algorithm */
            n_pat = safeMalloc(sizeof(char)*(strlen(line)-1));
            op_flag[1] = 1;
            strcpy(n_pat, line+2);
            naive(fout, text, n_pat);
        }else if(line[0] == 'K'){
            /* Knuth-Morris-Pratt algorithm */
            k_pat = safeMalloc(sizeof(char)*(strlen(line)-1));
            op_flag[2] = 1;
            strcpy(k_pat, line+2);
            kmp(fout, text, k_pat);
        }else if(line[0] == 'B'){
            /* Boyer-Moore algorithm */
            b_pat = safeMalloc(sizeof(char)*(strlen(line)-1));
            op_flag[3] = 1;
            strcpy(b_pat, line+2);
        }else if(line[0] == 'X'){
            /* Fechar todos os ficheiros e 
            * libertar memória antes de sair */
            fclose(fin);
            fclose(fout);
            if(line){
                free(line);
                if(op_flag[0])
                    free(text);
                if(op_flag[0])
                    free(n_pat);
                if(op_flag[0])
                    free(k_pat);
                if(op_flag[0])
                    free(b_pat);
            }
            exit(0);
        }
        free(line);
    }

    return 0;
}

char* readLine(FILE *file, char *buffer){

    int maxLen = 128, count = 0;
    char ch;

    buffer = safeMalloc(sizeof(char)*maxLen);

    if (file == NULL) {
        printf("Ficheiro input vazio.");
        exit(EXIT_FAILURE);
    }

    ch = getc(file);
    /* Percorrer ficheiro caracter a caracter 
    * até encontrar um \n ou EOF */
    while((ch != '\n') && (ch != EOF)){
        if(count == maxLen){
            maxLen += 1;
            buffer = realloc(buffer, maxLen);
            if(buffer == NULL){
                printf("Erro de memória no buffer.");
                exit(EXIT_FAILURE);
            }
        }
        buffer[count] = ch;
        count++;
        ch = getc(file);
    }
    buffer[count] = '\0';
    buffer = realloc(buffer, count + 1);
    if(buffer == NULL){
        printf("Erro de memória no buffer.");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

void naive(FILE *file, char *text, char *pat){

    int n = 0, m = 0, count = 0;

    /* Percorre o texto todo até ao segmento de 
    * tamanho igual ao padrão a encontrar */
    for(n = 0; n < strlen(text)-strlen(pat); n++){
        count = 0;
        for(m = 0; m < strlen(pat)-1; m++){
            if(pat[m] == text[n+m]){
                count++;
            }
        }
        /* Se todos caracteres de pat tiverem 
        * correspondência num dado ciclo, então 
        * o padrão foi encontrado */
        if(count == strlen(pat)-1)
            fprintf(file, "%d ", n);
    }
    fprintf(file, "\n");
}

/* Função ainda não terminada */
void kmp(FILE *file, char *text, char *pat){

    int count = 0, m = 0, n = 0, i = 0, j = 0;
    int *pre = NULL;

    pre = safeMalloc(sizeof(int)*strlen(pat));

    /* Construir a prefix table */
    n = strlen(text);
    m = strlen(pat);
    pre[0] = 0;
    for(j = 1; j < m; j++){
        while(i > 0 && pat[i+1] != pat[j])
            i = pre[i];
        if(pat[i+1] == pat[j])
            i++;
        pre[j] = i;
        printf("%d\n",pre[j]);
    }
    int a=0;
    i = 0;
    j = 0;
    /* Encontrar padrão */
    printf("i %d %d %d\n", i,j,count);
    while(i < n){ 
        if(i < n-1)
            a++;
        if(pat[j] == text[i]){ 
            j++; 
            i++; 
            printf("a %d %d %d %d\n", i,j,count,a);
        }
        if(j == m){   
            fprintf(file,"%d ", i-j);
            j = pre[j-1];
            count++; 
            printf("b %d %d %d %d\n", i,j,count,a);
        }else if(i < n && pat[j] != text[i]){ 
            if(j != 0){
                j = pre[j-1];
                count++;
                printf("c %d %d %d %d\n", i,j,count,a);
            }else{
                i = i+1; 
                printf("d %d %d %d %d\n", i,j,count,a);}
        } 
    } 
    fprintf(file,"\n%d\n", count);
    printf("%d %d\n", count, a);
    free(pre);
}

void* safeMalloc(size_t size){
    
    /* Alocar memória de uma variável do tipo desejado */
    void* p = malloc(size);
    if(p == NULL)
        exit(EXIT_FAILURE);
    return p;
}

void* safeRealloc(int size){
    
    /* Relocar memória de uma variável do tipo desejado */
    void* p = NULL;

    p = realloc(p, size);
    if(p == NULL)
        exit(EXIT_FAILURE);
    return p;
}
