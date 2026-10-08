#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================
   Projeto de Compiladores - MicroPascal
   Primeira parte: Analisador Lexico + Analisador Sintatico
   Linguagem: C
   ============================================================ */

typedef enum {
    TOK_EOF,
    TOK_IDENTIFICADOR,
    TOK_INTEIRO_LITERAL,
    TOK_REAL_LITERAL,
    TOK_CHAR_LITERAL,

    TOK_MENOR,
    TOK_MAIOR,
    TOK_MENOR_IGUAL,
    TOK_MAIOR_IGUAL,
    TOK_IGUAL,
    TOK_DIFERENTE,

    TOK_MAIS,
    TOK_MENOS,
    TOK_MULT,
    TOK_DIV_REAL,
    TOK_DIV_INTEIRO,
    TOK_AND,
    TOK_OR,
    TOK_NOT,

    TOK_ATRIBUICAO,

    TOK_FECHA_PAREN,
    TOK_ABRE_PAREN,
    TOK_VIRGULA,
    TOK_PONTO_VIRGULA,
    TOK_PONTO,
    TOK_DOIS_PONTOS,

    TOK_PROGRAM,
    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_WHILE,
    TOK_DO,
    TOK_REPEAT,
    TOK_UNTIL,
    TOK_INTEGER,
    TOK_REAL,
    TOK_CHAR,
    TOK_BEGIN,
    TOK_END,
    TOK_WRITE,
    TOK_VAR
} TokenType;

typedef struct {
    TokenType tipo;
    char *lexema;
    int linha;
    int coluna;
} Token;

typedef struct {
    FILE *arquivo;
    int caractere;
    int linha;
    int coluna;
    int erro;
} Lexer;

static Token token_atual;
static Lexer lexer;
static int erros_sintaticos = 0;

const char *nome_token(TokenType tipo) {
    switch (tipo) {
        case TOK_EOF: return "EOF";
        case TOK_IDENTIFICADOR: return "IDENTIFICADOR";
        case TOK_INTEIRO_LITERAL: return "INTEIRO_LITERAL";
        case TOK_REAL_LITERAL: return "REAL_LITERAL";
        case TOK_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOK_MENOR: return "<";
        case TOK_MAIOR: return ">";
        case TOK_MENOR_IGUAL: return "<=";
        case TOK_MAIOR_IGUAL: return ">=";
        case TOK_IGUAL: return "=";
        case TOK_DIFERENTE: return "<>";
        case TOK_MAIS: return "+";
        case TOK_MENOS: return "-";
        case TOK_MULT: return "*";
        case TOK_DIV_REAL: return "/";
        case TOK_DIV_INTEIRO: return "div";
        case TOK_AND: return "and";
        case TOK_OR: return "or";
        case TOK_NOT: return "not";
        case TOK_ATRIBUICAO: return ":=";
        case TOK_FECHA_PAREN: return ")";
        case TOK_ABRE_PAREN: return "(";
        case TOK_VIRGULA: return ",";
        case TOK_PONTO_VIRGULA: return ";";
        case TOK_PONTO: return ".";
        case TOK_DOIS_PONTOS: return ":";
        case TOK_PROGRAM: return "program";
        case TOK_IF: return "if";
        case TOK_THEN: return "then";
        case TOK_ELSE: return "else";
        case TOK_WHILE: return "while";
        case TOK_DO: return "do";
        case TOK_REPEAT: return "repeat";
        case TOK_UNTIL: return "until";
        case TOK_INTEGER: return "integer";
        case TOK_REAL: return "real";
        case TOK_CHAR: return "char";
        case TOK_BEGIN: return "begin";
        case TOK_END: return "end";
        case TOK_WRITE: return "write";
        case TOK_VAR: return "var";
        default: return "DESCONHECIDO";
    }
}

static char *duplicar_string(const char *inicio, size_t tamanho) {
    char *s = (char *)malloc(tamanho + 1);
    if (!s) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s, inicio, tamanho);
    s[tamanho] = '\0';
    return s;
}

static void lexer_avancar(void) {
    if (lexer.caractere == '\n') {
        lexer.linha++;
        lexer.coluna = 1;
    } else if (lexer.caractere != EOF) {
        lexer.coluna++;
    }
    lexer.caractere = fgetc(lexer.arquivo);
}

static int eh_letra(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int eh_digito(int c) {
    return c >= '0' && c <= '9';
}

static int eh_branco(int c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

static Token criar_token(TokenType tipo, const char *inicio, size_t tamanho,
                         int linha, int coluna) {
    Token t;
    t.tipo = tipo;
    t.lexema = duplicar_string(inicio, tamanho);
    t.linha = linha;
    t.coluna = coluna;
    return t;
}

static Token criar_token_simples(TokenType tipo, const char *lexema,
                                 int linha, int coluna) {
    return criar_token(tipo, lexema, strlen(lexema), linha, coluna);
}

static void erro_lexico(int c) {
    if (c == EOF) {
        fprintf(stderr, "Erro léxico no caracter [EOF]\n");
    } else if (c == '\n') {
        fprintf(stderr, "Erro léxico no caracter [\\n]\n");
    } else if (c == '\t') {
        fprintf(stderr, "Erro léxico no caracter [\\t]\n");
    } else {
        fprintf(stderr, "Erro léxico no caracter [%c]\n", c);
    }
    lexer.erro = 1;
}

static Token lexer_proximo_token(void) {
    /* Ignora brancos e comentários //, usados nos exemplos do enunciado. */
    while (1) {
        while (eh_branco(lexer.caractere)) {
            lexer_avancar();
        }

        if (lexer.caractere == '/') {
            int linha = lexer.linha;
            int coluna = lexer.coluna;
            (void)linha;
            (void)coluna;

            lexer_avancar();
            if (lexer.caractere == '/') {
                while (lexer.caractere != '\n' && lexer.caractere != EOF) {
                    lexer_avancar();
                }
                continue;
            }

            return criar_token_simples(TOK_DIV_REAL, "/", linha, coluna);
        }
        break;
    }

    int linha = lexer.linha;
    int coluna = lexer.coluna;
    int c = lexer.caractere;

    if (c == EOF) {
        return criar_token_simples(TOK_EOF, "EOF", linha, coluna);
    }

    /* Identificadores e palavras reservadas. */
    if (eh_letra(c)) {
        char buffer[1024];
        size_t n = 0;

        while (eh_letra(lexer.caractere) || eh_digito(lexer.caractere)) {
            if (n < sizeof(buffer) - 1) {
                buffer[n++] = (char)lexer.caractere;
            }
            lexer_avancar();
        }
        buffer[n] = '\0';

        TokenType tipo = TOK_IDENTIFICADOR;
        if (strcmp(buffer, "program") == 0) tipo = TOK_PROGRAM;
        else if (strcmp(buffer, "if") == 0) tipo = TOK_IF;
        else if (strcmp(buffer, "then") == 0) tipo = TOK_THEN;
        else if (strcmp(buffer, "else") == 0) tipo = TOK_ELSE;
        else if (strcmp(buffer, "while") == 0) tipo = TOK_WHILE;
        else if (strcmp(buffer, "do") == 0) tipo = TOK_DO;
        else if (strcmp(buffer, "repeat") == 0) tipo = TOK_REPEAT;
        else if (strcmp(buffer, "until") == 0) tipo = TOK_UNTIL;
        else if (strcmp(buffer, "integer") == 0) tipo = TOK_INTEGER;
        else if (strcmp(buffer, "real") == 0) tipo = TOK_REAL;
        else if (strcmp(buffer, "char") == 0) tipo = TOK_CHAR;
        else if (strcmp(buffer, "begin") == 0) tipo = TOK_BEGIN;
        else if (strcmp(buffer, "end") == 0) tipo = TOK_END;
        else if (strcmp(buffer, "write") == 0) tipo = TOK_WRITE;
        else if (strcmp(buffer, "var") == 0) tipo = TOK_VAR;
        else if (strcmp(buffer, "div") == 0) tipo = TOK_DIV_INTEIRO;
        else if (strcmp(buffer, "and") == 0) tipo = TOK_AND;
        else if (strcmp(buffer, "or") == 0) tipo = TOK_OR;
        else if (strcmp(buffer, "not") == 0) tipo = TOK_NOT;

        return criar_token(tipo, buffer, strlen(buffer), linha, coluna);
    }

    /* Numeros inteiros e reais. */
    if (eh_digito(c)) {
        char buffer[1024];
        size_t n = 0;

        while (eh_digito(lexer.caractere)) {
            if (n < sizeof(buffer) - 1) buffer[n++] = (char)lexer.caractere;
            lexer_avancar();
        }

        if (lexer.caractere == '.') {
            if (n < sizeof(buffer) - 1) buffer[n++] = '.';
            lexer_avancar();

            if (!eh_digito(lexer.caractere)) {
                /* 10. sozinho nao forma REAL_LITERAL segundo a especificacao. */
                erro_lexico('.');
                return criar_token(TOK_INTEIRO_LITERAL, buffer, n - 1, linha, coluna);
            }

            while (eh_digito(lexer.caractere)) {
                if (n < sizeof(buffer) - 1) buffer[n++] = (char)lexer.caractere;
                lexer_avancar();
            }
            buffer[n] = '\0';
            return criar_token(TOK_REAL_LITERAL, buffer, n, linha, coluna);
        }

        buffer[n] = '\0';
        return criar_token(TOK_INTEIRO_LITERAL, buffer, n, linha, coluna);
    }

    /* Literal char: 'a', '0', '\n' ou '\t'. */
    if (c == '\'') {
        char buffer[16];
        size_t n = 0;
        buffer[n++] = '\'';
        lexer_avancar();

        if (lexer.caractere == '\\') {
            buffer[n++] = '\\';
            lexer_avancar();
            if (lexer.caractere != 'n' && lexer.caractere != 't') {
                erro_lexico(lexer.caractere);
                if (lexer.caractere != EOF) lexer_avancar();
                return criar_token(TOK_CHAR_LITERAL, buffer, n, linha, coluna);
            }
            buffer[n++] = (char)lexer.caractere;
            lexer_avancar();
        } else if (eh_letra(lexer.caractere) || eh_digito(lexer.caractere)) {
            buffer[n++] = (char)lexer.caractere;
            lexer_avancar();
        } else {
            erro_lexico(lexer.caractere);
            if (lexer.caractere != EOF) lexer_avancar();
            return criar_token(TOK_CHAR_LITERAL, buffer, n, linha, coluna);
        }

        if (lexer.caractere != '\'') {
            erro_lexico(lexer.caractere);
            while (lexer.caractere != '\'' && lexer.caractere != EOF && lexer.caractere != '\n') {
                lexer_avancar();
            }
            if (lexer.caractere == '\'') lexer_avancar();
            return criar_token(TOK_CHAR_LITERAL, buffer, n, linha, coluna);
        }

        buffer[n++] = '\'';
        lexer_avancar();
        buffer[n] = '\0';
        return criar_token(TOK_CHAR_LITERAL, buffer, n, linha, coluna);
    }

    /* Operadores e simbolos especiais. */
    switch (c) {
        case '<':
            lexer_avancar();
            if (lexer.caractere == '=') {
                lexer_avancar();
                return criar_token_simples(TOK_MENOR_IGUAL, "<=", linha, coluna);
            }
            if (lexer.caractere == '>') {
                lexer_avancar();
                return criar_token_simples(TOK_DIFERENTE, "<>", linha, coluna);
            }
            return criar_token_simples(TOK_MENOR, "<", linha, coluna);

        case '>':
            lexer_avancar();
            if (lexer.caractere == '=') {
                lexer_avancar();
                return criar_token_simples(TOK_MAIOR_IGUAL, ">=", linha, coluna);
            }
            return criar_token_simples(TOK_MAIOR, ">", linha, coluna);

        case '=':
            lexer_avancar();
            return criar_token_simples(TOK_IGUAL, "=", linha, coluna);

        case '+':
            lexer_avancar();
            return criar_token_simples(TOK_MAIS, "+", linha, coluna);

        case '-':
            lexer_avancar();
            return criar_token_simples(TOK_MENOS, "-", linha, coluna);

        case '*':
            lexer_avancar();
            return criar_token_simples(TOK_MULT, "*", linha, coluna);

        case ':':
            lexer_avancar();
            if (lexer.caractere == '=') {
                lexer_avancar();
                return criar_token_simples(TOK_ATRIBUICAO, ":=", linha, coluna);
            }
            return criar_token_simples(TOK_DOIS_PONTOS, ":", linha, coluna);

        case ')':
            lexer_avancar();
            return criar_token_simples(TOK_FECHA_PAREN, ")", linha, coluna);
        case '(':
            lexer_avancar();
            return criar_token_simples(TOK_ABRE_PAREN, "(", linha, coluna);
        case ',':
            lexer_avancar();
            return criar_token_simples(TOK_VIRGULA, ",", linha, coluna);
        case ';':
            lexer_avancar();
            return criar_token_simples(TOK_PONTO_VIRGULA, ";", linha, coluna);
        case '.':
            lexer_avancar();
            return criar_token_simples(TOK_PONTO, ".", linha, coluna);
        default:
            erro_lexico(c);
            lexer_avancar();
            return criar_token_simples(TOK_EOF, "EOF", linha, coluna);
    }
}

static void liberar_token(Token *t) {
    free(t->lexema);
    t->lexema = NULL;
}

static void proximo_token(void) {
    liberar_token(&token_atual);
    token_atual = lexer_proximo_token();
}

static void erro_sintatico(void) {
    fprintf(stderr, "Erro de sintaxe no token [%s]\n", token_atual.lexema);
    erros_sintaticos++;
}

static int aceitar(TokenType tipo) {
    if (token_atual.tipo == tipo) {
        proximo_token();
        return 1;
    }
    return 0;
}

static int esperar(TokenType tipo) {
    if (token_atual.tipo == tipo) {
        proximo_token();
        return 1;
    }
    erro_sintatico();
    return 0;
}

/* ---------- Parser recursivo descendente ----------
   Expressao foi transformada para eliminar recursao a esquerda:

   expressao       -> nivel_logico { (or|and) nivel_rel }
   nivel_rel       -> nivel_add { relop nivel_add }
   nivel_add       -> nivel_mult { (+|-) nivel_mult }
   nivel_mult      -> basica { (*|/|div) basica }
   basica          -> (expressao) | not expressao | literal | IDENTIFICADOR

   Assim, todos os operadores sao associativos a esquerda e respeitam
   os niveis de precedencia pedidos no enunciado.
*/

static int parse_expressao(void);
static int parse_comando(void);
static int parse_bloco(void);

static int parse_basica(void) {
    if (aceitar(TOK_ABRE_PAREN)) {
        int ok = parse_expressao();
        if (!esperar(TOK_FECHA_PAREN)) ok = 0;
        return ok;
    }

    if (aceitar(TOK_NOT)) {
        return parse_expressao();
    }

    if (aceitar(TOK_INTEIRO_LITERAL) ||
        aceitar(TOK_REAL_LITERAL) ||
        aceitar(TOK_CHAR_LITERAL) ||
        aceitar(TOK_IDENTIFICADOR)) {
        return 1;
    }

    erro_sintatico();
    return 0;
}

static int parse_nivel_mult(void) {
    int ok = parse_basica();
    while (token_atual.tipo == TOK_MULT ||
           token_atual.tipo == TOK_DIV_REAL ||
           token_atual.tipo == TOK_DIV_INTEIRO) {
        proximo_token();
        if (!parse_basica()) ok = 0;
    }
    return ok;
}

static int parse_nivel_add(void) {
    int ok = parse_nivel_mult();
    while (token_atual.tipo == TOK_MAIS || token_atual.tipo == TOK_MENOS) {
        proximo_token();
        if (!parse_nivel_mult()) ok = 0;
    }
    return ok;
}

static int eh_relacional(TokenType t) {
    return t == TOK_IGUAL || t == TOK_DIFERENTE ||
           t == TOK_MENOR || t == TOK_MAIOR ||
           t == TOK_MENOR_IGUAL || t == TOK_MAIOR_IGUAL;
}

static int parse_nivel_rel(void) {
    int ok = parse_nivel_add();
    while (eh_relacional(token_atual.tipo)) {
        proximo_token();
        if (!parse_nivel_add()) ok = 0;
    }
    return ok;
}

static int parse_expressao(void) {
    /* No enunciado, and e or estao agrupados no mesmo nivel 4.
       Portanto, ambos sao tratados com a mesma precedencia e
       associatividade a esquerda. */
    int ok = parse_nivel_rel();
    while (token_atual.tipo == TOK_AND || token_atual.tipo == TOK_OR) {
        proximo_token();
        if (!parse_nivel_rel()) ok = 0;
    }
    return ok;
}

static int parse_secao_var(void) {
    if (!aceitar(TOK_VAR)) return 1;

    while (token_atual.tipo == TOK_IDENTIFICADOR) {
        proximo_token();

        while (aceitar(TOK_VIRGULA)) {
            if (!esperar(TOK_IDENTIFICADOR)) return 0;
        }

        if (!esperar(TOK_DOIS_PONTOS)) return 0;

        if (!(aceitar(TOK_INTEGER) || aceitar(TOK_REAL) || aceitar(TOK_CHAR))) {
            erro_sintatico();
            return 0;
        }

        if (!esperar(TOK_PONTO_VIRGULA)) return 0;
    }

    return 1;
}

static int parse_atribuicao(void) {
    if (!esperar(TOK_IDENTIFICADOR)) return 0;
    if (!esperar(TOK_ATRIBUICAO)) return 0;
    if (!parse_expressao()) return 0;
    if (!esperar(TOK_PONTO_VIRGULA)) return 0;
    return 1;
}

static int parse_escrita(void) {
    if (!esperar(TOK_WRITE)) return 0;
    if (!esperar(TOK_ABRE_PAREN)) return 0;
    if (!parse_expressao()) return 0;
    if (!esperar(TOK_FECHA_PAREN)) return 0;
    if (!esperar(TOK_PONTO_VIRGULA)) return 0;
    return 1;
}

static int parse_iteracao(void) {
    if (aceitar(TOK_WHILE)) {
        if (!parse_expressao()) return 0;
        if (!esperar(TOK_DO)) return 0;
        return parse_comando();
    }

    if (aceitar(TOK_REPEAT)) {
        if (!parse_comando()) return 0;
        if (!esperar(TOK_UNTIL)) return 0;
        if (!parse_expressao()) return 0;
        if (!esperar(TOK_PONTO_VIRGULA)) return 0;
        return 1;
    }

    erro_sintatico();
    return 0;
}

static int parse_decisao(void) {
    if (!esperar(TOK_IF)) return 0;
    if (!parse_expressao()) return 0;
    if (!esperar(TOK_THEN)) return 0;
    if (!parse_comando()) return 0;

    if (aceitar(TOK_ELSE)) {
        if (!parse_comando()) return 0;
    }

    return 1;
}

static int parse_bloco(void) {
    if (!esperar(TOK_BEGIN)) return 0;

    while (token_atual.tipo != TOK_END && token_atual.tipo != TOK_EOF) {
        if (!parse_comando()) return 0;
    }

    if (!esperar(TOK_END)) return 0;
    return 1;
}

static int parse_comando(void) {
    switch (token_atual.tipo) {
        case TOK_BEGIN:
            if (!parse_bloco()) return 0;
            if (!esperar(TOK_PONTO_VIRGULA)) return 0;
            return 1;
        case TOK_IDENTIFICADOR:
            return parse_atribuicao();
        case TOK_WHILE:
        case TOK_REPEAT:
            return parse_iteracao();
        case TOK_IF:
            return parse_decisao();
        case TOK_WRITE:
            return parse_escrita();
        default:
            erro_sintatico();
            return 0;
    }
}

static int parse_programa(void) {
    int ok = 1;

    if (!esperar(TOK_PROGRAM)) ok = 0;
    if (!esperar(TOK_IDENTIFICADOR)) ok = 0;
    if (!esperar(TOK_PONTO_VIRGULA)) ok = 0;
    if (!parse_secao_var()) ok = 0;
    if (!parse_bloco()) ok = 0;
    if (!esperar(TOK_PONTO)) ok = 0;

    if (token_atual.tipo != TOK_EOF) {
        erro_sintatico();
        ok = 0;
    }

    return ok && !lexer.erro && erros_sintaticos == 0;
}

static void imprimir_tokens(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo '%s'.\n", nome_arquivo);
        return;
    }

    Lexer antiga = lexer;
    lexer.arquivo = f;
    lexer.caractere = fgetc(f);
    lexer.linha = 1;
    lexer.coluna = 1;
    lexer.erro = 0;

    Token t;
    do {
        t = lexer_proximo_token();
        printf("%-20s lexema=[%s] linha=%d coluna=%d\n",
               nome_token(t.tipo), t.lexema, t.linha, t.coluna);
        liberar_token(&t);
    } while (t.tipo != TOK_EOF);

    fclose(f);
    lexer = antiga;
}

static int analisar_arquivo(const char *nome_arquivo) {
    lexer.arquivo = fopen(nome_arquivo, "r");
    if (!lexer.arquivo) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo '%s'.\n", nome_arquivo);
        return 0;
    }

    lexer.caractere = fgetc(lexer.arquivo);
    lexer.linha = 1;
    lexer.coluna = 1;
    lexer.erro = 0;
    erros_sintaticos = 0;

    token_atual.tipo = TOK_EOF;
    token_atual.lexema = duplicar_string("", 0);
    token_atual.linha = 1;
    token_atual.coluna = 1;
    proximo_token();

    int sucesso = parse_programa();

    liberar_token(&token_atual);
    fclose(lexer.arquivo);
    lexer.arquivo = NULL;

    return sucesso;
}

int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) {
        printf("Uso:\n");
        printf("  %s arquivo.pas          Analisa o programa\n", argv[0]);
        printf("  %s arquivo.pas --tokens Mostra os tokens\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 3 && strcmp(argv[2], "--tokens") == 0) {
        imprimir_tokens(argv[1]);
        return EXIT_SUCCESS;
    }

    if (argc == 3) {
        fprintf(stderr, "Opcao desconhecida: %s\n", argv[2]);
        return EXIT_FAILURE;
    }

    if (analisar_arquivo(argv[1])) {
        printf("Analise concluida com sucesso. Programa valido.\n");
        return EXIT_SUCCESS;
    }

    printf("Analise concluida com erro.\n");
    return EXIT_FAILURE;
}
