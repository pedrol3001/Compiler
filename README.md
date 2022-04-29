
# Compiler

## Geral
### Requisitos
- GNU Make 4.2.1 (Não sei se versões menores funcionam)
- Flex v2.6.4 (Não sei se versões menores funcionam)
- GNU Bison v3.8 (Não sei se versões menores funcionam, mas não funciounou para a versão v3.5)
### Compilação
Para recompilar por completo o projeto, use: 

    make compile
Para recompilar com aproveitamento dos arquivos objetos, use:

    make fast
### Escrita de testes
Escreva testes como arquivos .cpp no diretório Tests/, sempre adicionando mais um nível de profundidade no diretório (ex: Tests/example/example.cpp). Use o arquivo test_format.cpp para padronizar o teste, conforme os outros testes já escritos.

### Execução de testes
Para executar os testes do diretório Tests (depois de compilar o projeto), use:

    make test
    
### Debug
Para executar a linha de compilação suprema, use:

    make compile debug=yes
    make fast debug=yes
    make test debug=yes

## Analisador léxico
## Analisador sintático


