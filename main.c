#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int* count_words(const char *filename, int *err_code, int N, ...) {
    if(!filename || N < 1)
    {
        if(err_code)    *err_code = 1;
        return NULL;
    }

    int *result = calloc(N,sizeof (int));
    if(!result)
    {
        if(err_code)    *err_code = 3;
        return NULL;
    }
    FILE *f = fopen(filename,"r");
    if(!f)
    {
        if(err_code)    *err_code = 2;
        free(result);
        return NULL;
    }
    va_list(arg);
    va_start(arg,N);
    int pasuje;
    char comp = 0;
    char *word;
    for(int i = 0 ; i < N ; i++)
    {
        fseek(f,0L,SEEK_SET);
        word = va_arg(arg,char*);
        for(int j = 0 ; ; j++)
        {
            if(feof(f)) break;
            pasuje = 1;
            if(j == 0)
            {
                for(int k = 0 ; k < (int)strlen(word) ; k++)
                {
                    if(!fread(&comp,1,1,f)  || comp != *(word + k) || comp == EOF)
                    {
                        pasuje = 0;
                        break;
                    }
                }
                if(!fscanf(f,"%c",&comp) || comp == ' ' || comp == '\n' || comp == '.' || comp == '"' || comp == ',' || comp == EOF || comp == '\'' || comp == '-' || comp == ';' || comp == '?' || comp == ':')
                {
                    if(pasuje == 1) (*(result + i))++;
                }
                else fseek(f,-1L,SEEK_CUR);
            }
            else
            {
                if((comp != ' ' && comp != '\n' && comp != '.' && comp != '"' && comp != ',' && comp != EOF && comp != '\'' && comp != '-' && comp != ';' && comp != '?' && comp != ':'))
                {
                    while(1)
                    {
                        if(!fread(&comp,1,1,f) || feof(f) || comp == ' ' || comp == '\n' || comp == '.' || comp == '"' || comp == ',' || comp == EOF || comp == '\''|| comp == '-' || comp == ';' || comp == '?' || comp == ':')  break;
                    }
                    continue;
                }
                while(1)
                {
                    if(!fread(&comp,1,1,f) || feof(f) || (comp != ' ' && comp != '\n' && comp != '.' && comp != '"' && comp != ',' && comp != EOF && comp != '\'' && comp != '-' && comp != ';' && comp != '?' && comp != ':'))  break;
                }
                if(feof(f)) break;
                fseek(f,-1L,SEEK_CUR);
                for(int k = 0 ; k < (int)strlen(word) ; k++)
                {
                    if(!fread(&comp,1,1,f)  || comp != *(word + k) || comp == EOF)
                    {
                        pasuje = 0;
                        break;
                    }
                }
                if(!fscanf(f,"%c",&comp) || comp == ' ' || comp == '\n' || comp == '.' || comp == '"' || comp == ',' || comp == EOF || comp == '\'' || comp == '-' || comp == ';' || comp == '?' || comp == ':')
                {
                    if(pasuje == 1) (*(result + i))++;
                }
                else fseek(f,-1L,SEEK_CUR);
            }
        }
    }
    va_end(arg);
    fclose(f);
    if(err_code)    *err_code = 0;
    return result;
}
int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Not enough arguments");
        return 9;
    }
    int err = 0;
    int* tab_res = calloc(argc-2,sizeof (int));
    if(!tab_res)
    {
        printf("Failed to allocate memory");
        return 8;
    }
    for(int i = 0 ; i < argc-2 ; i++)
    {
        int *tab = count_words(*(argv + 1), &err, 1, *(argv + 2 + i));
        if(!tab)
        {
            if(err == 1)
            {
                printf("Not enough arguments");
                free(tab_res);
                return 9;
            }
            if(err == 2)
            {
                printf("Couldn't open file");
                free(tab_res);
                return 4;
            }
            if(err == 3)
            {
                printf("Failed to allocate memory");
                free(tab_res);
                return 8;
            }
            return 0;
        }
        *(tab_res + i) = *tab;
        free(tab);
    }

    for(int i = 0 ; i < argc-2 ; i++)
    {
        printf("%d\n",*(tab_res + i));
    }
    free(tab_res);
    return 0;
}