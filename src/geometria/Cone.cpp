#include "geometria/Cone.hpp"
#include <cmath>

Cone::Cone(Material material) : Solido("Cone", material) {

    this->centro_base = Ponto3(0.0);
    this->vertice = Ponto3(0.0, 1.0, 0.0);
    this->direcao = Vetor3(0.0, 1.0, 0.0);
    this->raio_base = 1;
    this->altura = 2;

}

Cone::Cone(Ponto3 centro_base, Ponto3 vertice, double raio_base, Material material) : Solido("Cone", material) {

    Vetor3 vetor_altura = vertice - centro_base;

    this->centro_base = centro_base;
    this->vertice = vertice;
    this->direcao = vetor_altura;
    this->raio_base = raio_base;
    this->altura = vetor_altura.norma();

}

Cone::Cone(Ponto3 centro_base, Vetor3 direcao, double raio_base, double altura, Material material) : Solido("Cone", material) {

    this->centro_base = centro_base;
    this->vertice = centro_base + (direcao * altura);
    this->direcao = direcao;
    this->raio_base = raio_base;
    this->altura = altura;

}

Ponto3 Cone::get_centro_base() const {

    return this->centro_base;

}
void Cone::set_centro_base(Ponto3 cb, bool recalcular_atributos) {

    Vetor3 vetor_altura;

    this->centro_base = cb;

    if (recalcular_atributos) {

        vetor_altura = this->get_vertice() - cb;

        this->direcao = vetor_altura;
        this->altura = vetor_altura.norma();

    }

}

Ponto3 Cone::get_vertice() const {

    return this->vertice;

}
void Cone::set_vertice(Ponto3 v, bool recalcular_atributos) {

    Vetor3 vetor_altura;

    this->vertice = v;

    if (recalcular_atributos) {

        vetor_altura = v - this->get_centro_base();

        this->direcao = vetor_altura;
        this->altura = vetor_altura.norma();

    }

}

Vetor3 Cone::get_direcao() const {

    return this->direcao;

}
void Cone::set_direcao(Vetor3 d, bool recalcular_atributos) {

    d.normalizar();
    this->direcao = d;

    if (recalcular_atributos) {

        this->vertice = this->get_centro_base() + (d * this->get_altura());

    }

}

double Cone::get_raio_base() const {

    return this->raio_base;

}
void Cone::set_raio_base(double r) {

    this->raio_base = r;

}

double Cone::get_altura() const {

    return this->altura;

}
void Cone::set_altura(double a, bool recalcular_atributos) {

    this->altura = a;

    if (recalcular_atributos) {

        this->vertice = this->get_centro_base() + (this->get_direcao() * a);

    }

}

double Cone::escalar_interseccao(Raio& raio) {

    // w = raio.p0 - centro_base
    Vetor3 w = raio.get_ponto_inicial() - this->get_centro_base();

    Plano base(this->get_centro_base(), this->get_direcao(), this->get_material());

    double delta = 0.0, // Delta da equação de 2º grau.
           a = 0.0, b = 0.0, c = 0.0, // Coscientes da equação de 2º grau.
           t_int = 0.0, // Distância do início do raio até o ponto de intersecção mais próximo.
           t_int1 = 0.0, t_int2 = 0.0, // Distâncias do início do raio até os pontos de intersecção.
           t_int_base = 0.0, // Distância do início do raio até o ponto de intersecção com a base do cone.

           beta = std::pow(this->get_altura(), 2) / std::pow(this->get_raio_base(), 2), // beta = altura_cone² / raio_cone²
           dr_escalar_dc = raio.get_direcao().escalar(this->get_direcao()), // Resultado do produto escalar da direção do raio com a direção do cone.
           w_escalar_dr = w.escalar(raio.get_direcao()), // Resultado do produto escalar do vetor com o vetor direção do raio.
           w_escalar_dc = w.escalar(this->get_direcao()), // Resultado do produto escalar do vetor w com o vetor direção do cone.
           v_escalar_dc = 0.0; // Resultado do produto escalar do vetor v (ponto_inicial_raio - centro_base) com a direção do cone.

    // a = beta - beta*(dr . dc)² - (dr . dc)²
    a = beta - std::pow(dr_escalar_dc, 2)*(beta + 1.0);

    // b = 2*beta*(w . dr) - 2*beta*(w . dc)*(dr . dc) - 2*(w . dc)*(dr . dc) + 2*altura_cone*(dr . dc)
    b = 2.0*(beta*(w_escalar_dr - w_escalar_dc*dr_escalar_dc) - w_escalar_dc*dr_escalar_dc + this->get_altura()*dr_escalar_dc);
    
    // c = beta*(w . w) - beta*(w . dc)² - (w . dc)² - altura_cone² + 2*altura_cone*(w . dc)
    c = beta*w.escalar(w) - std::pow(w_escalar_dc, 2)*(beta + 1.0) - std::pow(this->get_altura(), 2) + 2.0*this->get_altura()*w_escalar_dc;
    
    delta = std::pow(b, 2) - 4.0 * a * c;

    // Checando se houve intersecção.
    if (delta > 0.0) {

        t_int1 = (-b + std::sqrt(delta)) / (2.0 * a);
        t_int2 = (-b - std::sqrt(delta)) / (2.0 * a);

        // Checando se a primeira raíz é uma intersecção válida.
        v_escalar_dc = (raio.ponto_do_raio(t_int1) - this->get_centro_base()).escalar(this->get_direcao());  

        if (0.0 > v_escalar_dc || v_escalar_dc > this->get_altura()) {

            t_int1 = -1.0;

        }

        // Checando se a segunda raíz é uma intersecção válida.
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

        } else if (t_int1 >= 0.0 || t_int2 >= 0.0) {

            // Anotando o t_int válido no corpo do cone.
            if (t_int1 >= 0.0) {

                t_int = t_int1;

            } else {

                t_int = t_int2;

            }

            // Calculando a intersecção no plano da base.
            t_int_base = base.escalar_interseccao(raio);

            // Anotando se a intersecção com a base do cone é válida.
            if (t_int_base >= 0.0 && (raio.ponto_do_raio(t_int_base) - this->get_centro_base()).norma() > this->get_raio_base()) {

                t_int_base = -1.0;

            }

            // Checando se a intersecção na base do cone foi válida e se ela é mais próxima que a intersecção já anotada do corpo.
            if (t_int_base >= 0.0 && t_int_base < t_int) {

                t_int = t_int_base;

            }

        } else {

            t_int = -1.0;

        }

    } else if (delta == 0.0) {

        t_int1 = -b / (2.0 * a);

        v_escalar_dc = (raio.ponto_do_raio(t_int1) - this->get_centro_base()).escalar(this->get_direcao());

        if (0.0 <= v_escalar_dc && v_escalar_dc <= this->get_altura()) {

            t_int = t_int1;

        } else {

            t_int = -1.0;

        }

    } else {

        t_int = -1.0;

    }

    return t_int;

}

Vetor3 Cone::vetor_normal_ponto(Ponto3 ponto) const {

    Vetor3 ponto_vertice, n, v;
    double v_escalar_dc, erro = 1e-12;

    ponto_vertice = this->get_vertice() - ponto;
    v = ponto - this->get_centro_base();
    v_escalar_dc = v.escalar(this->get_direcao());

    if (0.0 + erro < v_escalar_dc) {

        n = ponto_vertice.vetorial(this->get_direcao());
        n = n.vetorial(ponto_vertice);
        n.normalizar();

        return n;

    } else {

        return this->get_direcao() * -1.0;

    }

}

IntensidadeLuz Cone::cor_textura(Ponto3 ponto) { return this->get_material().get_k_D(); }