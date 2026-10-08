# Projeto MicroPascal - Parte 1

Implementacao em C do analisador lexico e do analisador sintatico pedidos no projeto.

## Compilacao

GCC:

```bash
gcc -std=c11 -Wall -Wextra -pedantic micro_pascal.c -o micro_pascal
```

No Windows com MinGW:

```powershell
gcc -std=c11 -Wall -Wextra -pedantic micro_pascal.c -o micro_pascal.exe
```

## Execucao

Analisar um programa:

```text
micro_pascal SomaImpares.pas
```

Mostrar os tokens produzidos pelo lexer:

```text
micro_pascal SomaImpares.pas --tokens
```

## O que foi implementado

- Identificadores conforme `letra (letra | digito)*`.
- Operadores relacionais: `<`, `>`, `<=`, `>=`, `=`, `<>`.
- Operadores `+`, `-`, `*`, `/`, `div`, `and`, `or`, `not`.
- Atribuicao `:=`.
- Simbolos `)`, `(`, `,`, `;`, `.`, `:`.
- Palavras reservadas do enunciado.
- Literais inteiros, reais e char.
- Sensibilidade a maiusculas/minusculas.
- Ignora espacos, `\\n`, `\\t` e `\\r`.
- Mensagens de erro lexicas no formato pedido.
- Parser recursivo descendente.
- Blocos aninhados.
- Declaracoes de variaveis.
- Atribuicao, `while`, `repeat/until`, `if/then/else`, `write`.
- Expressoes com precedencia e associatividade a esquerda.
- Mensagens de erro sintaticas no formato pedido.

## Observacao sobre `//`

Os exemplos do proprio PDF usam comentarios `//`, embora a lista formal de tokens nao defina comentarios. Para que os exemplos do enunciado possam ser analisados diretamente, esta implementacao trata `//` ate o fim da linha como comentario e o ignora.

## Observacao sobre `-`

A gramatica da expressao apresenta `+`, mas a tabela de precedencia do enunciado tambem determina `+` e `-` no nivel 2. Por isso o parser implementa ambos, seguindo a tabela de precedencia.
