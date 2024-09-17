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

            LuzDirecional(Vetor3 direcao = Vetor3(), IntensidadeLuz intensidade = IntensidadeLuz());

        // --- GETTERS E SETTERS ---

            Vetor3 get_direcao() const;
            void set_direcao(Vetor3 d);

        // --- MÉTODOS ---

            // Retorna o vetor direção que aponta do ponto para a fonte de luz.
            Vetor3 direcao_ponto_luz(const Ponto3& ponto) const override;

            // Retorna a distância entre o ponto e a fonte de luz pontual.
            double distancia_ponto_luz(const Ponto3& ponto) const override;

            // Retorna o fator de atenuação da luz num ponto.
            double fator_atenuacao(const Ponto3& ponto) const override;

};

#endif