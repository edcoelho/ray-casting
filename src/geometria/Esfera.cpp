#include "geometria/Esfera.hpp"
#include <cmath>
#include <algorithm>

Esfera::Esfera(Material material) : Solido("Esfera", material) {

    this->set_centro(Ponto3(0.0, 1.0, 0.0));
    this->set_raio(1.0);

}

Esfera::Esfera(Ponto3 centro, double raio, Material material) : Solido("Esfera", material) {

    this->set_centro(centro);
    this->set_raio(raio);

}

Ponto3 Esfera::get_centro() const {

    return this->centro;

}
void Esfera::set_centro(Ponto3 centro) {

    this->centro = centro;

}

double Esfera::get_raio() const {

    return this->raio;

}
void Esfera::set_raio(double raio) {

    this->raio = raio;

}

double Esfera::escalar_interseccao(Raio& raio) {

    double delta = 0.0, // Delta da equação de 2º grau.
           a = 0.0, b = 0.0, c = 0.0, // Coscientes da equação de 2º grau.
           raiz = 0.0, // Raíz da equação.
           aux = 0.0; // Variável para auxiliar nos cálculos.

    Vetor3 v_aux; // Vetor para auxiliar nos cálculos.

    // a = dr . dr
    a = raio.get_direcao().escalar(raio.get_direcao());
    // b = 2 ((ponto_inicial - centro_esfera) . dr)
    v_aux = raio.get_ponto_inicial() - this->get_centro();
    b = 2.0 * v_aux.escalar(raio.get_direcao());
    // c = (ponto_inicial - centro_esfera) . (ponto_inicial - centro_esfera) - raio_esfera²
    c = v_aux.escalar(v_aux) - std::pow(this->get_raio(), 2);

    //delta = b² - 4ac
    delta = std::pow(b, 2) - 4*a*c;

    // raiz = (-b +- sqrt(delta))/2a
    if (delta > 0) {
        
        // Escolhendo a menor raíz.
        raiz = (-1*b + std::sqrt(delta)) / (2.0*a);
        aux = (-1*b - std::sqrt(delta)) / (2.0*a);
        if (aux < raiz) raiz = aux;

    } else if (delta == 0)
        
        raiz = -b / (2.0*a);
    
    else raiz = -1;

    return raiz;

}

Vetor3 Esfera::vetor_normal_ponto(Ponto3 ponto) const {

    return (ponto - this->get_centro()).unitario();

}

IntensidadeLuz Esfera::cor_textura(Ponto3 ponto) { return this->get_material().get_k_D(); }

std::pair<Ponto3, Ponto3> Esfera::pontos_min_max () const {

    std::vector<Ponto3> pontos;
    std::pair<Ponto3, Ponto3> resultado;

    pontos.reserve(6);

    Vetor3
        j(0.0, 1.0, 0.0),
        k = j.vetorial(this->centro.vetor()).unitario() * this->raio,
        i = j.vetorial(k).unitario() * this->raio;

    pontos.push_back(this->centro + i);
    pontos.push_back(this->centro - i);

    pontos.push_back(this->centro + j);
    pontos.push_back(this->centro - j);

    pontos.push_back(this->centro + k);
    pontos.push_back(this->centro - k);

    for (std::size_t eixo = 0; eixo < 3; eixo++) {

        resultado.first[eixo] = std::min({pontos[0][eixo], pontos[1][eixo], pontos[2][eixo], pontos[3][eixo], pontos[4][eixo], pontos[5][eixo]});
        resultado.second[eixo] = std::max({pontos[0][eixo], pontos[1][eixo], pontos[2][eixo], pontos[3][eixo], pontos[4][eixo], pontos[5][eixo]});

    }

    return resultado;

}