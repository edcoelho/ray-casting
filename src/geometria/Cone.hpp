#ifndef GEOMETRIA_CONE_HPP
#define GEOMETRIA_CONE_HPP

#include "geometria/Solido.hpp"
#include "geometria/Plano.hpp"
#include "utils/Material.hpp"
#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"

class Cone : public Solido {

    private:
        Ponto3 centro_base;
        Ponto3 vertice;
        Vetor3 direcao;
        double raio_base;
        double altura;

    public:
        // --- CONSTRUTORES ---

        Cone(Material material = Material());

        Cone(Ponto3 centro_base, Ponto3 vertice, double raio_base, Material material = Material());

        Cone(Ponto3 centro_base, Vetor3 direcao, double raio_base, double altura, Material material = Material());

        // --- GETTERS E SETTERS ---

        Ponto3 get_centro_base() const;
        void set_centro_base(Ponto3 cb, bool recalcular_atributos = true);

        Ponto3 get_vertice() const;
        void set_vertice(Ponto3 v, bool recalcular_atributos = true);

        Vetor3 get_direcao() const;
        // Normaliza o vetor "d" antes de atribuir o seu valor ao vetor direção do cone.
        void set_direcao(Vetor3 d, bool recalcular_atributos = true);

        double get_raio_base() const;
        void set_raio_base(double r);

        double get_altura() const;
        void set_altura(double a, bool recalcular_atributos = true);

        // --- MÉTODOS ---

        // Se o cone for intersectado pelo Raio "raio", retorna o escalar que é a distância entre o ponto inicial do raio e o ponto de intersecção mais próximo do ponto inicial do raio.
        // Se não houver intersecção, retorna -1.
        double escalar_interseccao(Raio& raio) override;

        // Retorna o vetor unitário normal a superfície do cone num ponto.
        Vetor3 vetor_normal_ponto(Ponto3 ponto) const override;

        // Retorna cor em formato de IntensidadeLuz do pixel da textura correspondente a um ponto.
        IntensidadeLuz cor_textura(Ponto3 ponto) override;

        // Retorna o ponto mínimo e ponto máximo do cone.
        std::pair<Ponto3, Ponto3> pontos_min_max () const override;

};

#endif