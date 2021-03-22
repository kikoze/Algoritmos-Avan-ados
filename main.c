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

int bm_aux(char letter, char *table_letters, int *shit_values, int tableSize);

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

    FILE *fin;
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
            /* Fechar todos os ficheiros e 
            * libertar memória antes de sair */
            fclose(fin);
            /*fclose(fout);*/
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

void naive(char *text, char *pat){

    int n = 0, m = 0, count = 0;

    /* Percorre o texto todo até ao segmento de 
    * tamanho igual ao padrão a encontrar */
    for(n = 0; n < strlen(text)-strlen(pat)+1; n++){
        count = 0;
        for(m = 0; m < strlen(pat)-1; m++){
            if(pat[m] == text[n+m]){
                count++;
            }else continue;
        }
        /* Se todos caracteres de pat tiverem 
        * correspondência num dado ciclo, então 
        * o padrão foi encontrado */
        if(count == strlen(pat)-1)
            printf("%d ", n);
    }
    printf("\n");
}

/* Função ainda não terminada */
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
        if(j == m)
            printf("%d ", i-m);
        if(j == 0){
            i++;
            comparissons++;
        }else{
            if(i < n)
                comparissons++;
            j = pre[j-1];
        }
    }
    printf("\n%d \n", comparissons);
    free(pre);
}

void bm(char *text, char *pat){

    int text_size = strlen(text), pat_size = strlen(pat);
    int comparissons = 0, numOfSkips = 0, i = 0, j = 0;
    int *shift_values = NULL;
    char *letters = NULL;

    letters = safeMalloc(sizeof(char)*(pat_size+10));
    shift_values = safeMalloc(sizeof(int)*(pat_size+1));

    for(i = 0; i < pat_size; i++){
        for(j = 0; j < pat_size+1; j++){
            if(letters[j] == 0){
                letters[j] = pat[i];
                shift_values[j] = max(1, pat_size-j-1);
                break;
            }else{
                if(letters[j]!=0 && letters[j] == pat[i]){
                    shift_values[j] = max(1, pat_size-j-1);
                    break;
                }
            }
        }
    }


    shift_values[j++] = pat_size;
    letters[j] = '\0';
    shift_values[j] = pat_size;
    int tableSize = j;

    letters = realloc(letters, sizeof(char)*(j+1));
    shift_values = realloc(shift_values, sizeof(int)*(j+1));

    
    for (i = 0; i <= (text_size-pat_size); i += numOfSkips){
       numOfSkips = 0;
       for(j = pat_size-1; j >= 0; j--){
           comparissons++;
           if(pat[j] != text[i+j]){
               numOfSkips = bm_aux(text[i+j], letters, shift_values, tableSize);
               break;
           }
       }
       if(numOfSkips == 0){printf("%d ",i+j+1); numOfSkips=1;} /*pattern*/
   }
   printf("\n%d \n", comparissons);

   free(letters);
   free(shift_values);
}

int bm_aux(char letter, char *table_letters, int *shift_values, int tableSize){
    
    int i;

    for(i=0; i<tableSize; i++){
        if(table_letters[i] == letter){
            return shift_values[i];
        }
    }
    return shift_values[i];
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
