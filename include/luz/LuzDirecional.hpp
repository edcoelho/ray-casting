#ifndef LUZ_LUZDIRECIONAL_HPP
#define LUZ_LUZDIRECIONAL_HPP

#include "luz/FonteLuz.hpp"
#include "algebra/Vetor3.hpp"

class LuzDirecional : public FonteLuz {

     private:
        // Direção dos raios de luz.
        Vetor3 direcao;

    public:

        // --- CONSTRUTORES ---

            LuzDirecional();

            LuzDirecional(IntensidadeLuz intensidade, Vetor3 direcao);

        // --- GETTERS E SETTERS ---

            Vetor3 get_direcao() const;
            void set_direcao(Vetor3 d);

        // --- MÉTODOS ---

            // Retorna true se o ponto for capaz de ser iluminado pela fonte de luz no caso de não haver obstrução de luz.
            bool ponto_valido(const Ponto3& ponto) const override;

            // Retorna o vetor direção que aponta do ponto para a fonte de luz.
            Vetor3 direcao_ponto_luz(const Ponto3& ponto) const override;

            // Retorna a distância entre o ponto e a fonte de luz pontual.
            double distancia_ponto_luz(const Ponto3& ponto) const override;

};

#endif