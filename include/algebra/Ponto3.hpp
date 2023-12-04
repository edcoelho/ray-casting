#ifndef ALGEBRA_PONTO3_HPP
#define ALGEBRA_PONTO3_HPP

#include "Vetor3.hpp"
#include <array>

class Ponto3 {

    private:
        double x;
        double y;
        double z;

    public:
        // --- CONSTRUTORES ---

            Ponto3();

            Ponto3(double x, double y, double z);

            Ponto3(double valor);

        // --- OPERADORES ---

            // Operador para acessar uma coordenada de um Ponto3.
            double operator [] (std::size_t linha) const;

            // Operador para definir o valor de uma coordenada de um Ponto3.
            double& operator [] (std::size_t linha);

            // Operador para adicionar um Vetor3 a um Ponto3.
            Ponto3 operator + (Vetor3 vetor) const;

            // Operador para subtrair um Vetor3 de um Ponto3.
            Ponto3 operator - (Vetor3 vetor) const;

            // Operador para subtrair um Ponto3 de outro Ponto3.
            Vetor3 operator - (Ponto3 ponto) const;

        // --- OUTROS MÉTODOS ---

            // Retorna o Vetor3 cujos elementos são as coordenadas do ponto.
            Vetor3 vetor() const;

};

#endif