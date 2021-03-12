#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * naive verifica a existência de um padrão num determinado texto
 * \param text texto onde um certo padrão deve ser procurado
 * \param pat padrão a encontrar
 */
void naive(FILE *file, char *text, char *pat);

/**
 * safeMalloc faz o malloc e depois garante que a memória foi alocada
 * \param size tamanho em bytes da memória a alocar
 * \return ponteiro resultante do malloc de size bytes
 */
void* safeMalloc(size_t size);

int main(int argc, char *argv[]){

    FILE *fin, *fout;
    char *line = NULL, *text = NULL;
    char *n_pat = NULL, *k_pat = NULL, *b_pat = NULL;
    size_t len = 0;
    ssize_t read;
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

    while((read = getline(&line, &len, fin)) != -1) {
        /* Verificar a primeira letra da linha e a partir
        * daí fazer a operação desejada */
        if(line[0] == 'T'){
            op_flag[0] = 1;
            text = safeMalloc(sizeof(char)*(read-1));
            strcpy(text, line+2);
        }else if(line[0] == 'N'){
            /* Naive algorithm */
            n_pat = safeMalloc(sizeof(char)*(read-1));
            op_flag[1] = 1;
            strcpy(n_pat, line+2);
            naive(fout, text, n_pat);
        }else if(line[0] == 'K'){
            /* Knuth-Morris-Pratt algorithm */
            k_pat = safeMalloc(sizeof(char)*(read-1));
            op_flag[2] = 1;
            strcpy(k_pat, line+2);
        }else if(line[0] == 'B'){
            /* Boyer-Moore algorithm */
            b_pat = safeMalloc(sizeof(char)*(read-1));
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
    }
    exit(EXIT_SUCCESS);

    return 0;
}

void naive(FILE *file, char *text, char *pat){

    int n = 0, m = 0, count = 0;

    /* Percorre o texto todo até ao segmento de 
    * tamanho igual ao padrão a encontrar */
    for(n=0; n<strlen(text)-strlen(pat); n++){
        count = 0;
        for(m=0; m<strlen(pat)-1; m++){
            if(pat[m] == text[n+m]){
                count++;
            }
        }
        /* Se todos caracteres de pat tiverem 
        * correspondência num dado ciclo, então 
        * o padrão foi encontrado */
        if(count == strlen(pat)-1){
            fprintf(file, "%d ", n);
            if(n == strlen(text)-strlen(pat)-1)
                fprintf(file, "%d\n", n);
        }
    }
}

void* safeMalloc(size_t size){
    
    /* Alocar memória de uma variável do tipo desejado */
    void* p = malloc(size);
    if (p == NULL)
        exit(EXIT_FAILURE);
    return p;
}
