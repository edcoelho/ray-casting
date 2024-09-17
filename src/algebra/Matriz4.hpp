#ifndef ALGEBRA_MATRIZ4_HPP
#define ALGEBRA_MATRIZ4_HPP

#include <array>
#include "utils/tipos.hpp"
#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"

// Uma matriz no R⁴. Inicializa por padrão como uma matriz identidade.
class Matriz4 {

    private:
        double a00, a01, a02, a03;
        double a10, a11, a12, a13;
        double a20, a21, a22, a23;
        double a30, a31, a32, a33;

    public:
        // --- CONSTRUTORES ---

            Matriz4();
    
            Matriz4(double a00, double a01, double a02, double a03, double a10, double a11, double a12, double a13, double a20, double a21, double a22, double a23, double a30, double a31, double a32, double a33);

            Matriz4(double valor);

        // --- OPERADORES ---

            // Operador para acessar um elemento de uma Matriz4.
            double operator () (std::size_t linha, std::size_t coluna) const;

            // Operador para definir o valor de um elemento de uma Matriz4.
            double& operator () (std::size_t linha, std::size_t coluna);

            // Operador para adição de duas Matriz4.
            Matriz4 operator + (Matriz4 const& matriz) const;

            // Operador para subtração de duas Matriz4.
            Matriz4 operator - (Matriz4 const& matriz) const;

            // Operador para multiplicação de duas Matriz4.
            Matriz4 operator * (Matriz4 const& matriz) const;

            // Operador para multiplicação de uma Matriz4 por um Ponto3.
            // A 4ª coordenada que é necessária para a multiplicação pelo Ponto3 é 1.
            Ponto3 operator * (Ponto3 const& ponto) const;

            // Operador para multiplicação de uma Matriz4 por um Vetor3.
            // A 4ª coordenada que é necessária para a multiplicação pelo Vetor3 é 0.
            Vetor3 operator * (Vetor3 const& vetor) const;

            // Operador para produto de uma Matriz4 por um escalar.
            Matriz4 operator * (double const escalar) const;

            // Operador para igualdade de Matriz4.
            bool operator == (Matriz4 const& matriz) const;

            // Operador para desigualdade de Matriz4.
            bool operator != (Matriz4 const& matriz) const;

};


#endif