#include "PID.h"

using namespace std;

PID::PID() {
    p_error = 0;
    i_error = 0;
    d_error = 0;

    previous_cte = 0;
}

PID::~PID() = default;

void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
}

void PID::UpdateError(double cte) {
    p_error = cte;
    i_error += cte;
    d_error = cte - previous_cte;
    previous_cte = cte;
}

double PID::TotalError() {
    return Kp*p_error + Ki*i_error + Kd*d_error;
}

void PID::reset() {
    p_error = 0;
    i_error = 0;
    d_error = 0;
}

