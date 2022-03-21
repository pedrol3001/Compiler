


# Compiler

## Geral
### Requisitos
- GNU Make 4.2.1 (Não sei se versões menores funcionam)
- Flex v2.6.4 (Não sei se versões menores funcionam)
- GNU Bison v3.8 (Não sei se versões menores funcionam, mas não funciounou para a versão v3.5)
### Execução
Ao usar GNU Make:

	make all

Será gerado o executável "compiler". Para executar o programa:

    ./compiler {file}.txt
De modo que {file} corresponde ao nome do arquivo de entrada a ser compilado.

### Teste
Para compilar e executar os testes, use:

	make test
	
Para adicionar testes, apenas crie arquivos .txt no diretório de testes.

    .
    ├── tests
    └── ├── {test_1}.txt
		├── {test_2}.txt
		├── {test_3}.txt
		├── {test_4}.txt
		├── ...
		└── {test_n}.txt

## Analisador léxico
## Analisador sintático


