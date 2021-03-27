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
void naive(char *text, char *pat);

/**
 * kmp verifica a existência de um padrão num determinado 
 * texto utilizado o algoritmo Knuth-Morris-Pratt
 * \param file ficheiro onde é escrito o output
 * \param text texto onde um certo padrão deve ser procurado
 * \param pat padrão a encontrar
 */
void kmp(char *text, char *pat);

/**
 * bm verifica a existência de um padrão num determinado 
 * texto utilizado o algoritmo Boyer-Moore
 * \param text texto onde um certo padrão deve ser procurado
 * \param pat padrão a encontrar
 */
void bm(char *text, char *pat);

void generateBadCharShift(int bad_char_shift[], char *pattern, int pat_size);
void generateSuffixShift(int suffix_shift[], char *pattern, int pat_size);
int *suffixes(char *pattern, int pat_size, int *buffer);

/**
 * max retorna o máximo entre dois inteiros
 * \param x primeiro inteiro
 * \param y segundo inteiro
 * \return inteiro correspondente ao máximo entre x e y
 */
int max(int x, int y);

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

    char *line = NULL, *text = NULL;
    char *n_pat = NULL, *k_pat = NULL, *b_pat = NULL;
    int op_flag[4] = {0,0,0,0};

    while (!feof(stdin)){
        line = readLine(stdin, line);
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
            naive(text, n_pat);
        }else if(line[0] == 'K'){
            /* Knuth-Morris-Pratt algorithm */
            k_pat = safeMalloc(sizeof(char)*(strlen(line)-1));
            op_flag[2] = 1;
            strcpy(k_pat, line+2);
            kmp(text, k_pat);
        }else if(line[0] == 'B'){
            /* Boyer-Moore algorithm */
            b_pat = safeMalloc(sizeof(char)*(strlen(line)-1));
            op_flag[3] = 1;
            strcpy(b_pat, line+2);
            bm(text, b_pat);
        }else if(line[0] == 'X'){
            /* Libertar memória antes de sair */
            if(line){
                free(line);
                if(op_flag[0])
                    free(text);
                if(op_flag[1])
                    free(n_pat);
                if(op_flag[2])
                    free(k_pat);
                if(op_flag[3])
                    free(b_pat);
            }
            exit(0);
        }
        free(line);
    }

    return 0;
}

char* readLine(FILE *file, char *buffer){

    int maxLen = 1280, count = 0;
    char ch;

    buffer = safeMalloc(sizeof(char)*maxLen);

    ch = getchar();
    /* Percorrer ficheiro caracter a caracter 
    * até encontrar um \n ou EOF */
    while((ch != '\n') && (ch != EOF)){
        if(count == maxLen){
            maxLen += 128;
            buffer = realloc(buffer, maxLen);
            if(buffer == NULL){
                printf("Erro de memória no buffer.");
                exit(EXIT_FAILURE);
            }
        }
        buffer[count] = ch;
        count++;
        ch = getchar();
    }
    buffer[count] = '\0';
    buffer = realloc(buffer, count + 1);
    if(buffer == NULL){
        printf("Erro de memória no buffer.");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

void naive(char *text, char *pat){

    int n = 0, m = 0, count = 0;

    /* Percorre o texto todo até ao segmento de 
    * tamanho igual ao padrão a encontrar */
    for(n = 0; n < strlen(text)-strlen(pat)+1; n++){
        count = 0;
        for(m = 0; m < strlen(pat); m++){
            /*printf("%d ", m);
            printf("%c %c\n",pat[m], text[n+m]);*/
            if(pat[m] == text[n+m]){
                count++;
            }else break;
        }
        /* Se todos caracteres de pat tiverem 
        * correspondência num dado ciclo, então 
        * o padrão foi encontrado */
        if(count == strlen(pat))
            printf("%d ", n);
    }
    printf("\n");
}

void kmp(char *text, char *pat){

    int comparissons = 0, m = 0, n = 0, i = 0, j = 0;
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
    }

    i = 0;
    j = 0;
    while(i + m - j <= n){
        while(pat[j] == text[i] && j < m){
            comparissons++;
            i++;
            j++;
        }
        if(j == m){
            printf("%d ", i-m);
            /*j = pre[j-1];*/
        }
        if(j == 0){
            i++;
        }else
            j = pre[j-1];
        if(i < n)
            comparissons++;
    }
    printf("\n%d \n", comparissons);
    free(pre);
}

void bm(char *text, char *pat){

    int pat_size = strlen(pat);
    int text_size = strlen(text);

    int i, j=0, comparisons=0;

    int bad_char_shift[57];
    for(i=0; i<57; i++){bad_char_shift[i]=pat_size;};
    int suffix_shift[pat_size];
    for(i=0; i<pat_size; i++){suffix_shift[i]=pat_size;};

    /* Preprocessing */
    generateBadCharShift(bad_char_shift, pat, pat_size);
    generateSuffixShift(suffix_shift, pat, pat_size);

    /* Searching */
    while(j <= (text_size-pat_size)){
        for (i = pat_size - 1; (i >= 0) && (pat[i] == text[i + j]); --i){comparisons++;}
        if (i < 0) {
            printf("%d ", j);
            j += suffix_shift[0];
        }
        else {
            comparisons++;
            j += max(suffix_shift[i], bad_char_shift[((int)text[i + j])-65] - pat_size + 1 + i);
        }
    }

    printf("\n%d \n", comparisons);
}

void generateBadCharShift(int bad_char_shift[], char *pattern, int pat_size){

    int i;

    for(i=0; i<57; ++i){
        bad_char_shift[i] = pat_size;
    }
    for(i=0; i<pat_size - 1; ++i){
        bad_char_shift[((int)pattern[i])-65] = pat_size -i -1;
    }
}

void generateSuffixShift(int suffix_shift[], char *pattern, int pat_size){
    int buffer[pat_size];
    memset(buffer, 0, pat_size*sizeof(int));
    int i, j;

    int *buffer_aux = suffixes(pattern, pat_size, buffer);

    for (i = pat_size - 1; i >= 0; --i)
        if (buffer_aux[i] == i + 1)
            for (j=0; j < pat_size - 1 - i; ++j){
                if (suffix_shift[j] == pat_size)
                    suffix_shift[j] = pat_size - 1 - i;
            }
    for (i = 0; i <= pat_size - 2; ++i)
        suffix_shift[pat_size - 1 - buffer_aux[i]] = pat_size - 1 - i;
}

int *suffixes(char *pattern, int pat_size, int *buffer){
    int i, j, k = 0;

    buffer[pat_size-1] = pat_size;
    j = pat_size-1;

    for (i = pat_size - 2; i >= 0; --i) {
        if (i > j && buffer[i + pat_size - 1 - k] < i - j)
            buffer[i] = buffer[i + pat_size - 1 - k];
        else {
            if (i < j)
                j = i;
            k = i;
            while (j >= 0 && pattern[j] == pattern[j + pat_size - 1 - k])
                --j;
            buffer[i] = k - j;
        }
    }

    return buffer;
}

int max(int x, int y) { return (x > y)? x: y; } 

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
