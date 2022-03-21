# Compiler
## Execução
Ao usar GNU Make:

    make all
Será gerado o executável "compiler". Para executar o programa:

    ./compiler {test}.txt
De modo que {test} corresponde ao nome do arquivo de entrada a ser compilado.

## Analisador léxico
### Requisitos
 - Flex v2.6.4 (Não sei se versões menores funcionam)
## Analisador sintático
### Requisitos
- GNU Bison v3.8 (Não sei se versões menores funcionam, mas não funciounou para a versão v3.5)

