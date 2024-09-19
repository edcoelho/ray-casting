#ifndef GEOMETRIA_PLANO_HPP
#define GEOMETRIA_PLANO_HPP

#include "utils/Material.hpp"
#include "geometria/Solido.hpp"

class Plano : public Solido {

    private:
        // Ponto conhecido do plano.
        Ponto3 ponto;
        // Vetor unitário normal ao plano.
        Vetor3 normal;

    public:

        // --- CONSTRUTORES ---

        Plano(Material material = Material());

        Plano(Ponto3 ponto, Vetor3 normal, Material material = Material());

        // --- GETTERS E SETTERS ---

        Ponto3 get_ponto() const;
        void set_ponto(Ponto3 p);

        Vetor3 get_normal() const;
        void set_normal(Vetor3 n);

        // --- MÉTODOS ---

        // Se o plano for intersectado pelo Raio "raio", retorna o escalar que é a distância entre o ponto inicial do raio e o ponto de intersecção mais próximo.
        // Se não houver intersecção, retorna -1.
        double escalar_interseccao(Raio& raio) override;

        // Retorna o vetor unitário normal a superfície do plano num ponto.
        Vetor3 vetor_normal_ponto(Ponto3 ponto) const override;

        // Retorna cor em formato de IntensidadeLuz do pixel da textura correspondente a um ponto.
        IntensidadeLuz cor_textura(Ponto3 ponto) override;

        // Retorna o ponto mínimo e ponto máximo do plano.
        // Retorna pontos inválidos.
        std::pair<Ponto3, Ponto3> pontos_min_max () const override;

};

#endif