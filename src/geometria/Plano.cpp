#include "geometria/Plano.hpp"
#include "utils/tipos.hpp"
#include <cmath>

Plano::Plano(Material material) : Solido("Plano", material) {

    this->set_ponto(Ponto3(0.0, 0.0, 0.0));
    this->set_normal(Vetor3(0.0, 1.0, 0.0));

}

Plano::Plano(Ponto3 ponto, Vetor3 normal, Material material) : Solido("Plano", material) {

    this->set_ponto(ponto);
    this->set_normal(normal);

}

Ponto3 Plano::get_ponto() const {

    return this->ponto;

}
void Plano::set_ponto(Ponto3 p) {

    this->ponto = p;

}

Vetor3 Plano::get_normal() const {

    return this->normal;

}
void Plano::set_normal(Vetor3 n) {

    this->normal = n.unitario();

}

double Plano::escalar_interseccao(Raio& raio) {
    
    // Escalar distância até a intersecção.
    double t_int;

    // pX: ponto conhecido do plano
    // p0: ponto inicial do raio
    // vN: vetor normal ao plano
    // dr: vetor direção do raio

    // t_int = ((pX - p0) . vN) / (dr . vN)

    // t_int = (pX - p0) . vN)
    t_int = (this->get_ponto() - raio.get_ponto_inicial()).escalar(this->get_normal());
    // t_int = t_int / (d . dr);
    t_int = t_int / raio.get_direcao().escalar(this->get_normal());

    if (t_int > 0) return t_int; else return -1;

}

Vetor3 Plano::vetor_normal_ponto(Ponto3 ponto) const {

    return this->get_normal();

}

IntensidadeLuz Plano::cor_textura(Ponto3 ponto) {

    // Base para coordenadas no plano.
    Vetor3 v1 = this->normal, v2;
    // Coordenadas de textura.
    std::size_t x, y;
    // Índice com menor valor no vetor normal.
    std::size_t menor_indice = 0;
    // Dimensões da textura.
    std::size_t altura_textura = this->get_material().get_textura()->get_altura_pixels(), largura_textura = this->get_material().get_textura()->get_largura_pixels();
    // Cor RGB do pixel da textura correspondende à intersecção.
    rgb cor_pixel;

    // Escolhendo índice com menor valor do vetor normal ao plano.
    for (int i = 0; i < 3; i++) {

        if (std::abs(this->normal[menor_indice]) > std::abs(this->normal[i])) {

            menor_indice = i;

        }
        
    }

    // Trocando os dois maiores valores do vetor normal ao plano de lugar e invertendo o sinal de um dos valores para obter um vetor ortogonal pertencente ao plano.
    if (menor_indice == 0) {

        v1[1] = this->normal[2];
        v1[2] = -this->normal[1];

    } else if (menor_indice == 1) {

        v1[0] = this->normal[2];
        v1[2] = -this->normal[0];

    } else {

        v1[0] = this->normal[1];
        v1[1] = -this->normal[0];

    }

    v1.normalizar();

    // Obtendo um vetor ortogonal ao primeiro que também esteja no plano para formar base de coordenadas de textura.
    v2 = v1.vetorial(this->normal);
    v2.normalizar();

    // Obtendo coordenadas de textura.
    x = (std::size_t) (ponto - this->ponto).escalar(v1) % altura_textura;
    y = (std::size_t) std::abs((ponto - this->ponto).escalar(v2)) % largura_textura;

    // Obtendo a cor do pixel da textura.
    cor_pixel = this->get_material().get_textura()->get_cor_pixel(x, y);

    return IntensidadeLuz(cor_pixel);

}