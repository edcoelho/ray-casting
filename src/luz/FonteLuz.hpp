#ifndef LUZ_FONTELUZ_HPP
#define LUZ_FONTELUZ_HPP

#include "luz/IntensidadeLuz.hpp"
#include "geometria/Raio.hpp"
#include "algebra/Vetor3.hpp"
#include "algebra/Ponto3.hpp"

class FonteLuz {

    private:
        IntensidadeLuz intensidade;

        // Coeficiente de atenuação quadrática.
        double atenuacao_a;

        // Coeficiente de atenuação linear.
        double atenuacao_b;

        // Coeficiente de atenuação constante.
        double atenuacao_c;

    public:

        // --- CONSTRUTORES ---

            FonteLuz(IntensidadeLuz intensidade = IntensidadeLuz(), double atenuacao_a = 0.0, double atenuacao_b = 0.0, double atenuacao_c = 1.0);

        // --- GETTERS E SETTERS ---

            IntensidadeLuz get_intensidade() const;
            void set_intensidade(IntensidadeLuz i);

            double get_atenuacao_a() const;
            void set_atenuacao_a(double a);

            double get_atenuacao_b() const;
            void set_atenuacao_b(double b);

            double get_atenuacao_c() const;
            void set_atenuacao_c(double c);

        // --- MÉTODOS ---

            // Retorna true se o ponto for capaz de ser iluminado pela fonte de luz no caso de não haver obstrução de luz.
            virtual bool ponto_valido(const Ponto3& ponto) const;

            // Retorna o vetor direção que aponta do ponto para a fonte de luz.
            virtual Vetor3 direcao_ponto_luz(const Ponto3& ponto) const = 0;

            // Retorna a distância entre o ponto e a fonte de luz.
            virtual double distancia_ponto_luz(const Ponto3& ponto) const = 0;

            // Retorna o fator de atenuação da luz num ponto.
            virtual double fator_atenuacao(const Ponto3& ponto) const = 0;

};

#endif