#ifndef UTILS_TIPOS_HPP
#define UTILS_TIPOS_HPP

#include <array>
#include <vector>

// --- DEFINIÇÕES DE TIPOS E ESTRUTURAS ---

// Canal de cor RGB. Funciona como um inteiro de 1 byte sem sinal, ou seja, um inteiro no intervalo [0, 255].
typedef unsigned char canal_RGB;

// Estrutura para ajudar na representação de uma cor.
typedef std::array<canal_RGB, 3> rgb;

typedef std::vector<std::vector<rgb>> matriz_cores;

#endif