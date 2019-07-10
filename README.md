# README
## Projeto Open Source
 ### Problemas:
  Limitacoes: teste deve estar dentro do diretorio. ok!
  Problema: 
    Instancia de Obj ?
    Nao possui string manip
    anewarray
    metodo nao estatico ?
  Nao tem:
    Interface
    Exception
    multiarray

## Tipos de Execução
  - -e: Exibidor
  - -i: Interpretador

## Como Executar
  1. Compilar utilizando o Make
  2. Executar o "a.out" passando como argumentos o caminho do .class e o tipo de execução

## Exemplo
  Interpretação:

```
  a.out Teste/Fibonacci.class -i
```

## Utilidade
Lendo bytecode de maneira legível para humanos:
```
javap -v -p -s -sysinfo -constants ClaseExemplo.class
```
