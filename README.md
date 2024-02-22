# Ray Casting Engine

Uma engine simples que implementa a renderização de cenas estáticas utilizando a técnica de Ray Casting.
Este projeto foi desenvolvido para a disciplina de Computação Gráfica I (CK0245) da UFC.

## Dependências

O projeto depende das bibliotecas [SDL2](https://github.com/libsdl-org/SDL) e [nlohmann/json](https://github.com/nlohmann/json). Também é necessário utilizar alguma distribuição Linux como sistema operacional.

## Build

Para compilar o projeto, basta utilizar o comando **make** na raíz do projeto.

## Execução

Para renderizar uma cena é preciso primeiro montá-la utilizando um arquivo no formato JSON (as chaves e valores possíveis estão especificadas no arquivo de exemplo **cenas/cena.json.exemplo**).
Depois de montar a cena, basta rodar o executável passando como argumento o arquivo JSON da cena. Usando a cena de teste presente no repositório, a execução seria feita utilizando o seguinte comando:
```console
./main cenas/casa.json
```
**Obs.:** Pode demorar mais de 1 minuto para calcular e renderizar o exemplo acima.