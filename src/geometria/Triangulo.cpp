#include "geometria/Triangulo.hpp"
#include "geometria/Plano.hpp"
#include <cmath>
#include <algorithm>

Triangulo::Triangulo(Material material) : Solido("Triângulo", material) {

    this->vertice1 = Ponto3(1.0, 0.0, 0.0);
    this->vertice2 = Ponto3(0.0, 1.0, 0.0);
    this->vertice3 = Ponto3(0.0, 0.0, 1.0);

    this->r1 = this->vertice2 - this->vertice1;
    this->r2 = this->vertice3 - this->vertice1;
    this->normal = this->r1.vetorial(this->r2).unitario();
    this->area_vezes_dois = this->r1.vetorial(this->r2).escalar(this->normal);

}

Triangulo::Triangulo(Ponto3 vertice1, Ponto3 vertice2, Ponto3 vertice3, Material material) : Solido("Triângulo", material) {

    this->vertice1 = vertice1;
    this->vertice2 = vertice2;
    this->vertice3 = vertice3;

    this->r1 = vertice2 - vertice1;
    this->r2 = vertice3 - vertice1;
    this->normal = this->r1.vetorial(this->r2).unitario();
    this->area_vezes_dois = this->r1.vetorial(this->r2).escalar(this->normal);

}

Ponto3 Triangulo::get_vertice1() const {

    return this->vertice1;

}
void Triangulo::set_vertice1(Ponto3 v) {

    this->vertice1 = v;
    this->normal = (vertice2 - vertice1).vetorial(vertice3 - vertice1).unitario();

}

Ponto3 Triangulo::get_vertice2() const {

    return this->vertice2;

}
void Triangulo::set_vertice2(Ponto3 v) {

    this->vertice2 = v;
    this->normal = (vertice2 - vertice1).vetorial(vertice3 - vertice1).unitario();

}

Ponto3 Triangulo::get_vertice3() const {

    return this->vertice3;

}
void Triangulo::set_vertice3(Ponto3 v) {

    this->vertice3 = v;
    this->normal = (vertice2 - vertice1).vetorial(vertice3 - vertice1).unitario();

}

const Ponto3& Triangulo::get_coordenadas_baricentricas() const {

    return this->coordenadas_baricentricas;

}

void Triangulo::calcular_coordenadas_baricentricas(Ponto3 ponto) {

    // Erro tolerado para os cálculos.
    double erro = 1.0e-12;

    // Vetor que vai do vértice 1 ao ponto.
    Vetor3 v;

    // Coordenadas baricêntricas.
    double c1, c2, c3;
    
    v = ponto - this->get_vertice1();

    c1 = v.escalar(this->r2.vetorial(this->normal)) / this->area_vezes_dois;
    if (c1 >= -erro) {

        c1 = c1 < 0.0 ? 0.0 : c1;

        c2 = v.escalar(this->normal.vetorial(this->r1)) / this->area_vezes_dois;
        if (c2 >= -erro) {

            c2 = c2 < 0.0 ? 0.0 : c2;

            c3 = 1.0 - c1 - c2;
            if (c3 >= -erro && std::abs(c1 + c2 + c3 - 1.0) <= erro) {

                c3 = c3 < 0.0 ? 0.0 : c3;

                this->coordenadas_baricentricas = Ponto3(c1, c2, c3);

            } else {

                this->coordenadas_baricentricas = Ponto3(-1.0);

            }

        } else {

            this->coordenadas_baricentricas = Ponto3(-1.0);

        }
        
    } else {

        this->coordenadas_baricentricas = Ponto3(-1.0);

    }

}

double Triangulo::escalar_interseccao(Raio& raio) {

    // Plano do triângulo.
    Plano plano(this->vertice1, this->normal);

    // Escalar distância até a intersecção.
    double t_int;

    // Ponto de intersecção.
    Ponto3 p_int;

    t_int = plano.escalar_interseccao(raio);

    if (t_int >= 0.0) {

        p_int = raio.ponto_do_raio(t_int);

        this->calcular_coordenadas_baricentricas(p_int);
        if (this->coordenadas_baricentricas[0] > -1.0) {
            
            return t_int;

        }

    }

    return -1.0;

}

Vetor3 Triangulo::vetor_normal_ponto(Ponto3 ponto) const {

    return this->normal;

}

IntensidadeLuz Triangulo::cor_textura(Ponto3 ponto) { return this->get_material().get_k_D(); }

std::pair<Ponto3, Ponto3> Triangulo::pontos_min_max () const {

    std::pair<Ponto3, Ponto3> resultado;

    for (std::size_t i = 0; i < 3; i++) {

        resultado.first[i] = std::min({this->vertice1[i], this->vertice2[i], this->vertice3[i]});
        resultado.second[i] = std::max({this->vertice1[i], this->vertice2[i], this->vertice3[i]});

    }

    return resultado;

}