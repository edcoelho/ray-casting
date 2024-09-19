#include "geometria/BoundingVolume.hpp"
#include <cmath>
#include <algorithm>

BoundingVolume::BoundingVolume (double _ajuste_volume) : volume(Cilindro()), ponto_minimo(Ponto3(INFINITY)), ponto_maximo(Ponto3(-INFINITY)), ajuste_volume(_ajuste_volume < 1.0 ? 1.0 : _ajuste_volume) {

    this->volume.set_direcao(Vetor3(0.0, 1.0, 0.0));

}

Ponto3 BoundingVolume::get_ponto_minimo () const {

    return this->ponto_minimo;

}

Ponto3 BoundingVolume::get_ponto_maximo () const {

    return this->ponto_maximo;

}

double BoundingVolume::get_ajuste_volume () const {

    return this->ajuste_volume;

}
void BoundingVolume::set_ajuste_volume (double _ajuste_volume) {

    this->ajuste_volume = _ajuste_volume < 1.0 ? 1.0 : _ajuste_volume;

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

        double altura = this->ponto_maximo[1] - this->ponto_minimo[1];
        Vetor3 vetor_ajuste(0.0, this->ajuste_volume * altura, 0.0);

        this->ponto_minimo = this->ponto_minimo - vetor_ajuste;
        this->ponto_maximo = this->ponto_maximo + vetor_ajuste;

        altura = this->ponto_maximo[1] - this->ponto_minimo[1];

        this->volume.set_centro_base(Ponto3(
            (this->ponto_maximo[0] + this->ponto_minimo[0])/2.0,
            this->ponto_minimo[1],
            (this->ponto_maximo[2] + this->ponto_minimo[2])/2.0
        ), false);
        this->volume.set_altura(altura, false);
        this->volume.set_raio((this->ponto_minimo - this->volume.get_centro_base()).norma());
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

    std::pair<Ponto3, Ponto3> min_max = malha->pontos_min_max();

    this->inserir(min_max.first);
    this->inserir(min_max.second);

    this->malhas.push_back(std::move(malha));

}

void BoundingVolume::inserir (const std::shared_ptr<BoundingVolume>& sub_grupo) {

    this->sub_grupos.push_back(std::move(sub_grupo));

}

hit_info BoundingVolume::calcula_interseccao (Raio& raio) {

    hit_info resultado;
    double distancia = -1.0, min_distancia = INFINITY;
    bool intersectou_volume = this->ponto_dentro(raio.get_ponto_inicial()) || this->volume.escalar_interseccao(raio) > -1.0;

    if (intersectou_volume) {

        for (std::size_t i = 0; i < this->solidos.size(); i++) {

            distancia = this->solidos[i]->escalar_interseccao(raio);

            if (distancia >= 0.0 && distancia < min_distancia) {

                min_distancia = distancia;
                resultado.intersectou = true;
                resultado.solido = this->solidos[i];
                resultado.distancia = distancia;

            }

        }

        for (std::size_t i = 0; i < this->malhas.size(); i++) {

            distancia = this->malhas[i]->escalar_interseccao(raio);

            if (distancia >= 0.0 && distancia < min_distancia) {

                min_distancia = distancia;
                resultado.intersectou = true;
                resultado.solido = std::make_shared<Triangulo>(this->malhas[i]->get_ultima_face_intersectada());
                resultado.malha = this->malhas[i];
                resultado.distancia = distancia;

            }

        }

    }

    if (resultado.intersectou) {

        resultado.bounding_volume = std::make_shared<BoundingVolume>(*this);

    }

    return resultado;

}

bool BoundingVolume::ponto_dentro (const Ponto3& ponto) const {

    Vetor3 v, p_v, h;
    double v_escalar_dc, r;
    // double erro = 1.0e-12;

    v = ponto - this->volume.get_centro_base();
    v_escalar_dc = v.escalar(this->volume.get_direcao());
    p_v = this->volume.get_direcao() * v_escalar_dc;
    r = (v - p_v).norma();

    return r <= this->volume.get_raio() && 0.0 < v_escalar_dc && v_escalar_dc < this->volume.get_altura();

}