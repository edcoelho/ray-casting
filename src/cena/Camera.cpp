#include "cena/Camera.hpp"

// --- CONSTRUTORES ---

Camera::Camera(Ponto3 posicao, Ponto3 ponto_visada, Ponto3 ponto_amarra, double x_max, double x_min, double y_max, double y_min, double distancia_focal) {

    this->posicao = posicao;
    this->ponto_visada = ponto_visada;
    this->ponto_amarra = ponto_amarra;
    this->x_max = x_max;
    this->x_min = x_min;
    this->y_max = y_max;
    this->y_min = y_min;
    this->distancia_focal = distancia_focal;
    this->distancia_focal_padrao = distancia_focal;

    this->recalcular_matrizes();

}

void Camera::recalcular_matrizes() {

    // Vetores da base canônica da câmera.
    Vetor3 i_camera, j_camera, k_camera;

    k_camera = (this->get_posicao() - this->get_ponto_visada()).unitario();
    i_camera = ((this->get_ponto_amarra() - this->get_posicao()) * k_camera).unitario();
    j_camera = (k_camera * i_camera).unitario();

    for (int i = 0; i < 3; i++) {

        this->matriz_camera_mundo(i, 0) = i_camera[i];
        this->matriz_camera_mundo(i, 1) = j_camera[i];
        this->matriz_camera_mundo(i, 2) = k_camera[i];
        this->matriz_camera_mundo(i, 3) = this->get_posicao()[i];

        this->matriz_mundo_camera(0, i) = i_camera[i];
        this->matriz_mundo_camera(1, i) = j_camera[i];
        this->matriz_mundo_camera(2, i) = k_camera[i];

    }

    this->matriz_mundo_camera(0, 3) = -1.0*i_camera.escalar(this->get_posicao().vetor());
    this->matriz_mundo_camera(1, 3) = -1.0*j_camera.escalar(this->get_posicao().vetor());
    this->matriz_mundo_camera(2, 3) = -1.0*k_camera.escalar(this->get_posicao().vetor());

}

Ponto3 Camera::get_posicao() const {

    return this->posicao;

}
void Camera::set_posicao(Ponto3 p, bool atualizar_matrizes) {

    this->posicao = p;
    if (atualizar_matrizes) this->recalcular_matrizes();

}

Ponto3 Camera::get_ponto_visada() const {

    return this->ponto_visada;

}
void Camera::set_ponto_visada(Ponto3 pv, bool atualizar_matrizes) {

    this->ponto_visada = pv;
    if (atualizar_matrizes) this->recalcular_matrizes();

}

Ponto3 Camera::get_ponto_amarra() const {

    return this->ponto_amarra;

}
void Camera::set_ponto_amarra(Ponto3 pa, bool atualizar_matrizes) {

    this->ponto_amarra = pa;
    if(atualizar_matrizes) this->recalcular_matrizes();

}

double Camera::get_x_max() const {

    return this->x_max;

}
void Camera::set_x_max(double x) {

    this->x_max = x;

}

double Camera::get_x_min() const {

    return this->x_min;

}
void Camera::set_x_min(double x) {

    this->x_min = x;

}

double Camera::get_y_max() const {

    return this->y_max;

}
void Camera::set_y_max(double y) {

    this->y_max = y;

}

double Camera::get_y_min() const {

    return this->y_min;

}
void Camera::set_y_min(double y) {

    this->y_min = y;

}

double Camera::get_distancia_focal() const {

    return this->distancia_focal;

}
void Camera::set_distancia_focal(double d) {

    this->distancia_focal = d;

}

Matriz4 const& Camera::get_matriz_camera_mundo() const {

    return this->matriz_camera_mundo;

}

Matriz4 const& Camera::get_matriz_mundo_camera() const {

    return this->matriz_mundo_camera;

}

void Camera::aumentar_zoom(double fator) {

    if (fator >= 1.0) {

        this->distancia_focal = this->distancia_focal * fator;

    }

}

void Camera::diminuir_zoom(double divisor) {

    if (divisor >= 1.0) {

        if (this->distancia_focal / divisor >= this->distancia_focal_padrao) {

            this->distancia_focal = this->distancia_focal / divisor;

        } else {

            this->distancia_focal = this->distancia_focal_padrao;

        }

    }

}