


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

Outros código de exemplo e que compilam estão em Resources/Final.

Exemplos de códigos que dão erro estão em Resources/Erros.
O arquivo erros.txt gera todos os erros e avisos detectáveis pelo analisador semântico:

    ./compiler Resources/Erros/erros.txt
