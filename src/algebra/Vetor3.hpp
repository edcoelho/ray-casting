#ifndef ALGEBRA_VETOR3_HPP
#define ALGEBRA_VETOR3_HPP

#include <array>

class Ponto3;

class Vetor3 {

    private:
        double x;
        double y;
        double z;

    public:

        // --- CONSTRUTORES ---

            Vetor3(double valor = 1.0);

            Vetor3(double x, double y, double z);

        // --- OPERADORES ---

            // Operador para acessar um elemento de um Vetor3.
            double operator [] (std::size_t indice) const;

            // Operador para definir o valor de um elemento de um Vetor3.
            double& operator [] (std::size_t indice);

            // Operador para adição de dois Vetor3.
            Vetor3 operator + (Vetor3 const& vetor) const;

            // Operador para subtração de dois Vetor3.
            Vetor3 operator - (Vetor3 const& vetor) const;

            // Operador para produto vetorial do Vetor3 por outro Vetor3.
            Vetor3 operator * (Vetor3 const vetor) const;

            // Operador para produto de um Vetor3 por um escalar.
            Vetor3 operator * (double const escalar) const;

            // Operador para igualdade de Vetor3.
            bool operator == (Vetor3 const& vetor) const;

            // Operador para desigualdade de Vetor3.
            bool operator != (Vetor3 const& vetor) const;

        // --- OUTROS MÉTODOS ---

            // Retorna a norma/módulo do Vetor3.
            double norma() const;

            // Normaliza o vetor.
            void normalizar();

            // Retorna um Ponto3 cujas coordenadas são os elementos do Vetor3.
            Ponto3 ponto() const;

            // Retorna o Vetor3 unitário de mesmo sentido e direção.
            Vetor3 unitario() const;

            // Retorna o produto escalar do Vetor3 por um outro Vetor3 "v".
            double escalar(Vetor3 v) const;

            // Retorna o produto vetorial do Vetor3 por um outro Vetor3 "v".
            Vetor3 vetorial(Vetor3 v) const;

            // Retorna o Vetor3 refletido em relação a um outro Vetor3 "v".
            Vetor3 reflexo(Vetor3 v) const;

};

#endif