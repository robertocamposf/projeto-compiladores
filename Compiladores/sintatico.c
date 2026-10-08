#include <stdio.h>
#include <stdlib.h>
#include "sintatico.h"
#include "compilador.h"


void erro_sintatico(char *esperado){
    printf("Erro de sintaxe no token [%s]\n", lexema, esperado);
    exit(1);
}

void programa(void){
    if(simbolo_lido == TOKEN_PROGRAM){
        obtenha_simbolo();
    }
    else{
        erro_sintatico("program");
    }

    if(simbolo_lido == TOKEN_IDENT){
        obtenha_simbolo();
    }
    else{
        erro_sintatico("identificador");
    }

    if(simbolo_lido == TOKEN_PONTO_E_VIRGULA){
        obtenha_simbolo();
    }
    else{
        erro_sintatico(";");
    }
    
    secao_var();
    bloco();
    
    if(simbolo_lido == TOKEN_PONTO){
        obtenha_simbolo();
    }
    else{
        erro_sintatico(".");
    }
}

void secao_var(void){
    if(simbolo_lido == TOKEN_VAR){
        obtenha_simbolo();
        
        while (simbolo_lido == TOKEN_IDENT){
            decl_var();
        }
    }
}

void decl_var(void){
    if(simbolo_lido == TOKEN_IDENT){
        obtenha_simbolo();
    } 
    else{
        erro_sintatico("identificador da variavel");
    }

    while(simbolo_lido == TOKEN_VIRGULA) {
        obtenha_simbolo();
        if(simbolo_lido == TOKEN_IDENT) {
            obtenha_simbolo();
        } else {
            erro_sintatico("identificador");
        }
    }

    if(simbolo_lido == TOKEN_DOIS_PONTOS){ 
        obtenha_simbolo();
    } 
    else{
        erro_sintatico(":");
    }

    if(simbolo_lido == TOKEN_INTEGER || simbolo_lido == TOKEN_REAL || simbolo_lido == TOKEN_CHAR){
        obtenha_simbolo();
    }
    else{
        erro_sintatico("tipo da variavel");
    }

    if(simbolo_lido == TOKEN_PONTO_E_VIRGULA){
        obtenha_simbolo();
    } 
    else{
        erro_sintatico(";");
    }
}

void lista_comandos(void){
    while(simbolo_lido != TOKEN_END && simbolo_lido != TOKEN_FIM){
        comando();
    }
}

void bloco(void){
    if (simbolo_lido == TOKEN_BEGIN){
        obtenha_simbolo();
        lista_comandos();

        if (simbolo_lido == TOKEN_END){
            obtenha_simbolo();
        } 
        else{
            erro_sintatico("end");
        }
    }
    else{
        erro_sintatico("begin");
    }
}

void comando(void) {
    if (simbolo_lido == TOKEN_BEGIN) {
        bloco();
        if (simbolo_lido == TOKEN_PONTO_E_VIRGULA) {
            obtenha_simbolo();
        }
    } 
    else if (simbolo_lido == TOKEN_IDENT) {
        atribuicao();
    } 
    else if (simbolo_lido == TOKEN_WHILE || simbolo_lido == TOKEN_REPEAT) {
        iteracao();
    } 
    else if (simbolo_lido == TOKEN_IF) {
        decisao();
    } 
    else if (simbolo_lido == TOKEN_WRITE) {
        escrita();
    } 
    else {
        erro_sintatico("comando valido");
    }
}

void atribuicao(void) {
    if (simbolo_lido == TOKEN_IDENT) {
        obtenha_simbolo();
    } else {
        erro_sintatico("identificador");
    }

    if (simbolo_lido == TOKEN_ATRIBUICAO) {
        obtenha_simbolo();
    } else {
        erro_sintatico(":=");
    }

    expressao();

    if (simbolo_lido == TOKEN_PONTO_E_VIRGULA) {
        obtenha_simbolo();
    } else {
        erro_sintatico(";");
    }
}

void iteracao(void) {
    if (simbolo_lido == TOKEN_WHILE) {
        obtenha_simbolo();
        expressao();
        
        if (simbolo_lido == TOKEN_DO) {
            obtenha_simbolo();
        } else {
            erro_sintatico("do");
        }
        comando();
    } 
    else if (simbolo_lido == TOKEN_REPEAT) {
        obtenha_simbolo();
        comando();
        
        if (simbolo_lido == TOKEN_UNTIL) {
            obtenha_simbolo();
        } else {
            erro_sintatico("until");
        }
        expressao();
        
        if(simbolo_lido == TOKEN_PONTO_E_VIRGULA){
            obtenha_simbolo();
        }
        else{
            erro_sintatico(";");
        }
    }
}

void decisao(void){

    if(simbolo_lido == TOKEN_IF){
        obtenha_simbolo();
        expressao();
        
        if(simbolo_lido == TOKEN_THEN){
            obtenha_simbolo();
        }
        else{
            erro_sintatico("then");
        }
        comando();

        if(simbolo_lido == TOKEN_ELSE){
            obtenha_simbolo();
            comando();
        }
    }
}

void escrita(void){
    if(simbolo_lido == TOKEN_WRITE){
        obtenha_simbolo();
        
        if(simbolo_lido == TOKEN_ABRE_PAR){ 
            obtenha_simbolo();
        }
        else{
            erro_sintatico("(");
        }

        expressao();

        if(simbolo_lido == TOKEN_FECHA_PAR){ 
            obtenha_simbolo();
        }
        else{
            erro_sintatico(")");
        }

        if(simbolo_lido == TOKEN_PONTO_E_VIRGULA){
            obtenha_simbolo();
        } 
        else{
            erro_sintatico(";");
        }
    }
}

void expressao(void){
    expr_nivel3(); 
    while(simbolo_lido == TOKEN_OR || simbolo_lido == TOKEN_AND){
        obtenha_simbolo();
        expr_nivel3();
    }
}

void expr_nivel3(void){
    expr_nivel2(); 
    while (simbolo_lido == TOKEN_IGUAL || simbolo_lido == TOKEN_DIFERENTE ||
           simbolo_lido == TOKEN_MENORQ_OU_IGUAL || simbolo_lido == TOKEN_MENORQ ||
           simbolo_lido == TOKEN_MAIORQ_OU_IGUAL || simbolo_lido == TOKEN_MAIORQ){
        obtenha_simbolo();
        expr_nivel2();
    }
}

void expr_nivel2(void){
    expr_nivel1(); 
    while (simbolo_lido == TOKEN_MAIS || simbolo_lido == TOKEN_MENOS){
        obtenha_simbolo();
        expr_nivel1();
    }
}

void expr_nivel1(void){
    expr_basica(); 
    while(simbolo_lido == TOKEN_MULT || simbolo_lido == TOKEN_DIV_REAIS || simbolo_lido == TOKEN_DIV){
        obtenha_simbolo();
        expr_basica();
    }
}

void expr_basica(void){
    if(simbolo_lido == TOKEN_ABRE_PAR){
        obtenha_simbolo();
        expressao();

        if(simbolo_lido == TOKEN_FECHA_PAR){
            obtenha_simbolo();
        }

        else{
            erro_sintatico(")");
        }
    }

    else if(simbolo_lido == TOKEN_NOT){
        obtenha_simbolo();
        expressao();
    }

    else if(simbolo_lido == TOKEN_NUMERO_INT || simbolo_lido == TOKEN_NUMERO_REAL || 
            simbolo_lido == TOKEN_CHAR_LITERAL || 
            simbolo_lido == TOKEN_IDENT) {
        obtenha_simbolo();
    }

    else{
        erro_sintatico("expressao basica");
    }
}