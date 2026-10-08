#include <stdio.h>
#include <string.h>
#include "compilador.h"
#include "sintatico.h"

//as variaveis globais ja foram definidas no compilador.h
int main(void) {
strcpy(arrayglobal, "program SomaImpares; var n : integer; i, proximoImpar, soma : integer; begin n := 4; i := 0; soma := 0; while i < n do begin proximoImpar := 2*i + 1; soma := soma + proximoImpar; i := i + 1; end; write(soma); write('n'); end.");

    pos = 0;
    obtenha_simbolo();    
    programa();

    printf("Sucesso\n");
    return 0;
}