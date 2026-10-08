#include <stdio.h>
#include <stdlib.h>
#include "ctype.h"
#include <string.h>
#include "compilador.h"


int pos = 0;
int simbolo_lido = 0;
char arrayglobal[500];
char lexema[100];

void erro_lexico(char c){
    printf("Erro lexico no caracter: [%c]", c);
}

int proximo_token(void){

char a = arrayglobal[pos];

    while (isspace(a)){ // para ignorar espacos em branco
        pos++;
        a = arrayglobal[pos];
    }

if (isalpha(a)) {
        int i = 0;

     while (isalnum(arrayglobal[pos]) || arrayglobal[pos] == '_') {
        lexema[i++] = arrayglobal[pos++];
    }
        lexema[i] = '\0';

        //para verificar se sao palavras reservadas ou identificadores
    if (strcmp(lexema, "program") == 0){
        return TOKEN_PROGRAM;}

    if (strcmp(lexema, "if") == 0){
        return TOKEN_IF;}

    if (strcmp(lexema, "then") == 0){ 
        return TOKEN_THEN;}

    if (strcmp(lexema, "else") == 0){
        return TOKEN_ELSE;}

    if (strcmp(lexema, "while") == 0){
        return TOKEN_WHILE;}

    if (strcmp(lexema, "do") == 0){
        return TOKEN_DO;}

    if (strcmp(lexema, "repeat") == 0){
        return TOKEN_REPEAT;}

    if (strcmp(lexema, "until") == 0){
        return TOKEN_UNTIL;}

    if (strcmp(lexema, "integer") == 0){
        return TOKEN_INTEGER;}

    if (strcmp(lexema, "real") == 0){
        return TOKEN_REAL;}

    if (strcmp(lexema, "char") == 0){
        return TOKEN_CHAR;}

    if (strcmp(lexema, "begin") == 0){
        return TOKEN_BEGIN;}

    if (strcmp(lexema, "end") == 0){
        return TOKEN_END;}

    if (strcmp(lexema, "write") == 0){
        return TOKEN_WRITE;}

    if (strcmp(lexema, "var") == 0){
        return TOKEN_VAR;}

    if (strcmp(lexema, "div") == 0){ 
        return TOKEN_DIV;}

    if (strcmp(lexema, "and") == 0){
        return TOKEN_AND;}

    if (strcmp(lexema, "or") == 0){
        return TOKEN_OR;}

    if (strcmp(lexema, "not") == 0){
        return TOKEN_NOT;}

    return TOKEN_IDENT; 
}

    if(isdigit(a)){
        int i = 0;
        int e_real = 0;

        while(isdigit(arrayglobal[pos])){
            lexema[i++] = arrayglobal[pos++];
        }
        if(arrayglobal[pos] == '.' && isdigit(arrayglobal[pos+1])){
            e_real = 1;
            lexema[i++] = arrayglobal[pos++];
            while(isdigit(arrayglobal[pos])){
                lexema[i++] = arrayglobal[pos++];
            }
        }
        lexema[i] = '\0';
        //aqui é so para retornar se for real ou inteiro
        if(e_real){
            return TOKEN_NUMERO_REAL;
        }
        else{
            return TOKEN_NUMERO_INT;
        }
    }
    if (a == '\'') {
        int i = 0;
        lexema[i++] = arrayglobal[pos++];

        while (arrayglobal[pos] != '\'' && arrayglobal[pos] != '\0') { //Para pegar so a primeira aspa
            lexema[i++] = arrayglobal[pos++];
        }
        if (arrayglobal[pos] == '\'') { //pega a ultima
            lexema[i++] = arrayglobal[pos++];
            lexema[i] = '\0';// fim do char

            return TOKEN_CHAR_LITERAL;
        } else {
            erro_lexico(arrayglobal[pos]);
        }
    }

    if (a == '\0'){
        return TOKEN_FIM;
    }

    if (a == '+'){
        pos++;
        return TOKEN_MAIS;
    }

    if (a == '-'){
        pos++;
        return TOKEN_MENOS;
    }

    if (a == '*'){
        pos++;
        return TOKEN_MULT;
    }
    
    if(a == '/'){
       pos++;
       return TOKEN_DIV_REAIS; 
    }

    if(a == '='){
        pos++;
        return TOKEN_IGUAL;
    }

    if (a == '('){
        pos++;
        return TOKEN_ABRE_PAR;
    }

    if (a == ')'){
        pos++;
        return TOKEN_FECHA_PAR;
    }
    
    if (a == '.'){
        pos++;
        return TOKEN_PONTO;
    }
    if(a == ','){
        pos++;
        return TOKEN_VIRGULA;
    } 

    if(a == ';'){
        pos++;
        return TOKEN_PONTO_E_VIRGULA;
    }

    if(a == '<'){
        if(arrayglobal[pos + 1] == '>'){
            pos +=2;
            return TOKEN_DIFERENTE;
        }
        if(arrayglobal[pos + 1] == '='){
            pos +=2;
            return TOKEN_MENORQ_OU_IGUAL;
        }
        else{
            pos++;
            return TOKEN_MENORQ;
        }
}   

    if(a == '>'){
        if(arrayglobal[pos + 1] == '='){
            pos +=2;
            return TOKEN_MAIORQ_OU_IGUAL;
        }
    else{
        pos++;
        return TOKEN_MAIORQ;
    }

    }
    if(a == ':'){
        if(arrayglobal[pos + 1] == '='){
            pos +=2;
            return TOKEN_ATRIBUICAO;
        }
        else{
            pos++;
            return TOKEN_DOIS_PONTOS;
        }
    }
    
    erro_lexico(a);
    return 0;

}
void obtenha_simbolo(void){
    simbolo_lido = proximo_token();
}
