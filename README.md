


# Compiler

## Geral
### Requisitos
- GNU Make 4.2.1 (Não sei se versões menores funcionam)
- Flex v2.6.4 (Não sei se versões menores funcionam)
- GNU Bison v3.8 (Não sei se versões menores funcionam, mas não funcionou para a versão v3.5)
- Sistema Operacional Linux
### Compilação
Para recompilar por completo o projeto, use: 

    make compile

### Execução
Para executar, use:

    ./compiler [nome do programa.txt]

Isso irá gerar o arquivo objeto "programa.tm". Um exemplo para o programa fatorial:

    ./compiler Resources/Final/fat.txt

### Exemplos

Exemplos de código que são compilados sem erros ou avisos estão em Resources/Final. Alguns dos algoritmos implementados:

    Mergesort (Resources/Final/mergesort.txt)
    Exponenciação binária (Resources/Final/exponenciacao.txt)
    Maior divisior comum (Resources/Final/gcd.txt)

Exemplos de códigos que reportam erros estão em Resources/Erros.
O arquivo erros.txt gera todos os erros e avisos detectáveis pelo analisador semântico:

    ./compiler Resources/Erros/erros.txt