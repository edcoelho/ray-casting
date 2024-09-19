#include "geometria/Cilindro.hpp"
#include <cmath>
#include <algorithm>

Cilindro::Cilindro(Material material) : Solido("Cilíndro", material) {

    this->centro_base = Ponto3(0.0, -1.0, 0.0);
    this->centro_topo = Ponto3(0.0, 1.0, 0.0);
    this->direcao = Vetor3(0.0, 1.0, 0.0);
    this->altura = 2;
    this->raio = 1;

}

Cilindro::Cilindro(Ponto3 centro_base, Ponto3 centro_topo, double raio, Material material) : Solido("Cilíndro", material) {

    Vetor3 vetor_altura = centro_topo - centro_base;

    this->centro_base = centro_base;
    this->centro_topo = centro_topo;
    this->direcao = vetor_altura;
    this->altura = vetor_altura.norma();
    this->raio = raio;

}

Cilindro::Cilindro(Ponto3 centro_base, Vetor3 direcao, double raio, double altura, Material material) : Solido("Cilíndro", material) {

    this->centro_base = centro_base;
    this->direcao = direcao;
    this->raio = raio;
    this->altura = altura;
    this->centro_topo = centro_base + (this->get_direcao() * altura);
    
}

Ponto3 Cilindro::get_centro_base() const {

    return this->centro_base;

}
void Cilindro::set_centro_base(Ponto3 cb, bool recalcular_atributos) {

    Vetor3 vetor_altura;

    this->centro_base = cb;

    if (recalcular_atributos) {

        vetor_altura = this->get_centro_topo() - cb;

        this->direcao = vetor_altura.unitario();
        this->altura = vetor_altura.norma();

    }

}

Ponto3 Cilindro::get_centro_topo() const {

    return this->centro_topo;

}
void Cilindro::set_centro_topo(Ponto3 ct, bool recalcular_atributos) {

    Vetor3 vetor_altura;

    this->centro_topo = ct;

    if (recalcular_atributos) {

        vetor_altura = ct - this->get_centro_base();

        this->direcao = vetor_altura.unitario();
        this->altura = vetor_altura.norma();

    }

}

Vetor3 Cilindro::get_direcao() const {

    return this->direcao;

}
void Cilindro::set_direcao(Vetor3 d, bool recalcular_atributos) {

    d.normalizar();
    this->direcao = d;

    if (recalcular_atributos) {

        this->centro_topo = this->get_centro_base() + (d * this->get_altura());

    }

}

double Cilindro::get_altura() const {

    return this->altura;

}
void Cilindro::set_altura(double a, bool recalcular_atributos) {

    this->altura = a;

    if (recalcular_atributos) {

        this->centro_topo = this->get_centro_base() + (this->get_direcao() * a);

    }

}

double Cilindro::get_raio() const {

    return this->raio;
    
}
void Cilindro::set_raio(double r) {

    this->raio = r;

}

double Cilindro::escalar_interseccao(Raio& raio) {

    // w = P0 - centro_base
    Vetor3 w = raio.get_ponto_inicial() - this->get_centro_base();

    Plano base(this->get_centro_base(), this->get_direcao(), this->get_material()),
          topo(this->get_centro_topo(), this->get_direcao(), this->get_material());

    double delta = 0.0, // Delta da equação de 2º grau.
           a = 0.0, b = 0.0, c = 0.0, // Coscientes da equação de 2º grau.
           t_int = 0.0, // Distância do início do raio até o ponto de intersecção mais próximo.
           t_int1 = 0.0, t_int2 = 0.0, // Distâncias do início do raio até os pontos de intersecção.
           t_int_base = 0.0, // Distância do início do raio até o ponto de intersecção com a base do cilíndro.
           t_int_topo = 0.0, // Distância do início do raio até o ponto de intersecção com o topo do cilíndro.

           dr_escalar_dc = raio.get_direcao().escalar(this->get_direcao()), // Resultado do produto escalar da direção do raio com a direção do cilíndro.
           w_escalar_dc = w.escalar(this->get_direcao()), // Resultado do produto escalar do vetor w com a direção do cilíndro.
           v_escalar_dc = 0.0; // Resultado do produto escalar do vetor v (ponto_inicial_raio - centro_base) com a direção do cilíndro.

    // Checando o raio é paralelo ao eixo do cilíndro.
    if (std::abs(dr_escalar_dc) != 1.0) {

        // a = direcao_raio . direcao_raio - (direcao_raio . direcao_cilindro)²
        a = raio.get_direcao().escalar(raio.get_direcao()) - std::pow(dr_escalar_dc, 2);

        // b = 2(w . direcao_raio) - 2(w . direcao_cilindro)(direcao_raio . direcao_cilindro)
        b = 2.0 * w.escalar(raio.get_direcao()) - 2.0 * w_escalar_dc * dr_escalar_dc;

        // c = (w . w) - (w . direcao_cilindro)² - raio_cilindro²
        c = w.escalar(w) - std::pow(w_escalar_dc, 2) - std::pow(this->get_raio(), 2);

        delta = std::pow(b, 2) - 4.0 * a * c;

        if (delta > 0.0) {

            t_int1 = (-b + std::sqrt(delta)) / (2.0 * a);
            t_int2 = (-b - std::sqrt(delta)) / (2.0 * a);

            // Anotando se a primeira raíz é uma intersecção válida.
            v_escalar_dc = (raio.ponto_do_raio(t_int1) - this->get_centro_base()).escalar(this->get_direcao());

            if (0.0 > v_escalar_dc || v_escalar_dc > this->get_altura()) {

                t_int1 = -1.0;

            }

            // Anotando se a segunda raíz é uma intersecção válida.
            v_escalar_dc = (raio.ponto_do_raio(t_int2) - this->get_centro_base()).escalar(this->get_direcao());

            if (0.0 > v_escalar_dc || v_escalar_dc > this->get_altura()) {

                t_int2 = -1.0;

            }

            // Escolhendo a raíz que represente a intersecção válida mais próxima.

            if (t_int1 >= 0.0 && t_int2 >= 0.0) {

                if (t_int1 < t_int2) {

                    t_int = t_int1;

                } else {

                    t_int = t_int2;

                }

            } else {

                t_int_base = base.escalar_interseccao(raio);
                t_int_topo = topo.escalar_interseccao(raio);

                // Checando se a intersecção com a base do cilíndro é válida.
                if (t_int_base >= 0.0 && (raio.ponto_do_raio(t_int_base) - this->get_centro_base()).norma() > this->get_raio()) {

                    t_int_base = -1.0;

                }

                // Checando se a intersecção com o topo do cilíndro é válida.
                if (t_int_topo >= 0.0 && (raio.ponto_do_raio(t_int_topo) - this->get_centro_topo()).norma() > this->get_raio()) {

                    t_int_topo = -1.0;

                }

                // Checando se o raio intersectou a base e o topo ao mesmo tempo.
                if (t_int_base >= 0.0 && t_int_topo >= 0.0) {

                    // Anotando a intersecção mais próxima.
                    if (t_int_base < t_int_topo) {

                        t_int = t_int_base;

                    } else {

                        t_int = t_int_topo;

                    }

                } else {

                    // Checando se o raio intersectou a base ou o topo ou nenhum dos dois.
                    if (t_int_base >= 0.0) {

                        t_int = t_int_base;

                    } else if (t_int_topo >= 0.0) {

                        t_int = t_int_topo;
                        
                    } else {
                        
                        t_int = -1.0;

                    }

                    // Checando se a base ou o topo foram intersectados.
                    if (t_int >= 0.0) {

                        // Checando qual das intersecções na superfíce do cilíndro é válida.
                        if (t_int1 >= 0.0) {

                            // Checando se a intersecção na superfície do cilíndro está mais próxima do que a intersecção na base ou no topo.
                            if (t_int1 < t_int) {

                                t_int = t_int1;

                            }

                        } else if (t_int2 >= 0.0) {

                            // Checando se a intersecção na superfície do cilíndro está mais próxima do que a intersecção na base ou no topo.
                            if (t_int2 < t_int) {

                                t_int = t_int2;

                            }

                        }

                    }

                } 


            }

        } else if (delta == 0.0) {

            // Calculando a única intersecção com o corpo do cilíndro.
            t_int1 = -b / (2.0 * a);

            v_escalar_dc = (raio.ponto_do_raio(t_int1) - this->get_centro_base()).escalar(this->get_direcao());

            // Checando se a intersecção é válida.
            if (0.0 <= v_escalar_dc && v_escalar_dc <= this->get_altura()) {

                t_int = t_int1;

            } else {

                t_int = -1.0;

            }

        } else {

            t_int = -1.0;

        }

    } else {

        t_int_base = base.escalar_interseccao(raio);
        t_int_topo = topo.escalar_interseccao(raio);

        // Anotando se a intersecção com a base do cilíndro é válida.
        if (t_int_base >= 0.0 && (raio.ponto_do_raio(t_int_base) - this->get_centro_base()).norma() > this->get_raio()) {

            t_int_base = -1.0;

        }

        // Anotando se a intersecção com o topo do cilíndro é válida.
        if (t_int_topo >= 0.0 && (raio.ponto_do_raio(t_int_topo) - this->get_centro_topo()).norma() > this->get_raio()) {

            t_int_topo = -1.0;

        }

        if (t_int_base >= 0.0 && t_int_topo >= 0.0) {

            if (t_int_base < t_int_topo) {

                t_int = t_int_base;

            } else {

                t_int = t_int_topo;

            }

        } else if (t_int_base >= 0.0) {

            t_int = t_int_base;

        } else if (t_int_topo >= 0.0) {

            t_int = t_int_topo;
            
        } else {

            t_int = -1.0;

        }

    }

    return t_int;

}

Vetor3 Cilindro::vetor_normal_ponto(Ponto3 ponto) const {

    Vetor3 v, p_v, h;
    double v_escalar_dc, erro = 1e-12;

    v = ponto - this->get_centro_base();
    v_escalar_dc = v.escalar(this->get_direcao());
    p_v = this->get_direcao() * v_escalar_dc;
    h = v - p_v;

    if (0.0 + erro < v_escalar_dc && v_escalar_dc < this->get_altura() - erro) {

        h.normalizar();

        return h;

    } else if (v_escalar_dc <= 0.0 + erro) {

        return this->get_direcao() * -1.0;

    } else {

        return this->get_direcao();

    }

}

IntensidadeLuz Cilindro::cor_textura(Ponto3 ponto) { return this->get_material().get_k_D(); }

std::pair<Ponto3, Ponto3> Cilindro::pontos_min_max () const {

    std::vector<Ponto3> pontos;
    std::pair<Ponto3, Ponto3> resultado;

    pontos.reserve(8);

    Vetor3
        j = this->direcao,
        k = j.vetorial(this->centro_base.vetor()).unitario() * this->raio,
        i = j.vetorial(k).unitario() * this->raio;

    pontos.push_back(this->centro_base + i);
    pontos.push_back(this->centro_base - i);

    pontos.push_back(this->centro_base + k);
    pontos.push_back(this->centro_base - k);

    pontos.push_back(this->centro_topo + i);
    pontos.push_back(this->centro_topo - i);

    pontos.push_back(this->centro_topo + k);
    pontos.push_back(this->centro_topo - k);

    for (std::size_t eixo = 0; eixo < 3; eixo++) {

        resultado.first[eixo] = std::min({pontos[0][eixo], pontos[1][eixo], pontos[2][eixo], pontos[3][eixo], pontos[4][eixo]});
        resultado.second[eixo] = std::max({pontos[0][eixo], pontos[1][eixo], pontos[2][eixo], pontos[3][eixo], pontos[4][eixo]});

    }

    return resultado;

}