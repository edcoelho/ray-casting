#include "geometria/BoundingVolume.hpp"
#include <cmath>
#include <algorithm>

BoundingVolume::BoundingVolume () : volume(Cilindro()), ponto_minimo(Ponto3(INFINITY)), ponto_maximo(Ponto3(-INFINITY)) {

    this->volume.set_direcao(Vetor3(0.0, 1.0, 0.0));

}

Ponto3 BoundingVolume::get_ponto_minimo () const {

    return this->ponto_minimo;

}

Ponto3 BoundingVolume::get_ponto_maximo () const {

    return this->ponto_maximo;

}

void BoundingVolume::inserir (const Ponto3 & ponto) {

    bool estrutura_mudou = false;

    for (std::size_t i = 0; i < 3; i++) {

        if (ponto[i] < this->ponto_minimo[i]) {

            this->ponto_minimo[i] = ponto[i];
            estrutura_mudou = true;

        }

        if (ponto[i] > this->ponto_maximo[i]) {

            this->ponto_maximo[i] = ponto[i];
            estrutura_mudou = true;

        }

    }

    if (estrutura_mudou) {

        this->volume.set_raio(std::max(this->ponto_maximo[0] - this->ponto_minimo[0], this->ponto_maximo[2] - this->ponto_minimo[2] )/2.0);
        this->volume.set_altura(this->ponto_maximo[1] - this->ponto_minimo[1], false);
        this->volume.set_centro_base(Ponto3(
            (this->ponto_maximo[0] + this->ponto_minimo[0])/2.0,
            this->ponto_minimo[1],
            (this->ponto_maximo[2] + this->ponto_minimo[2])/2.0
        ), false);
        this->volume.set_centro_topo(this->volume.get_centro_base() + (this->volume.get_direcao() * this->volume.get_altura()), false);

    }

}

void BoundingVolume::inserir (const std::shared_ptr<Solido>& solido) {

    std::pair<Ponto3, Ponto3> min_max = solido->pontos_min_max();

    this->inserir(min_max.first);
    this->inserir(min_max.second);

    this->solidos.push_back(std::move(solido));

}

void BoundingVolume::inserir (const std::shared_ptr<Malha>& malha) {

    this->malhas.push_back(std::move(malha));

}

void BoundingVolume::inserir (const std::shared_ptr<BoundingVolume>& sub_grupo) {

    this->sub_grupos.push_back(std::move(sub_grupo));

}

hit_info BoundingVolume::calcula_interseccao (Raio& raio) {

    hit_info resultado;
    double distancia = -1.0, min_distancia = INFINITY;
    std::size_t indice_solido = 0;

    if (this->volume.escalar_interseccao(raio) > -1.0) {

        for (std::size_t i = 0; i < this->solidos.size(); i++) {

            distancia = this->solidos[i]->escalar_interseccao(raio);

            if (distancia >= 0.0 && distancia < min_distancia) {

                resultado.intersectou = true;
                indice_solido = i;
                resultado.distancia = distancia;

            }

        }

    }

    if (resultado.intersectou) {

        resultado.bounding_volume = std::make_shared<BoundingVolume>(*this);
        resultado.solido = this->solidos[indice_solido];

    }

    return resultado;

}
