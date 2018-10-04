//
// Created by Thomas Milas on 9/30/18.
//

#include "tuning.h"

vector<double> Tuner::getParam() {
    return params;
}

void Tuner::updateParamError(double error) {
    running_error += abs(error);
    running_count++;
}

void Tuner::evaluate() {
    switch(state){
        case KpTune:
        case KdTune:
            evalError();
            break;
        case WarmUp:
            warmUpEval();
            break;
        case Completed:
            resetError();
            return;
    }
    cout << "TUNER::testing param with values: " << endl;
    for(auto param : params) {
        cout << param << endl;
    }

}

void Tuner::warmUpEval() {
    resetError();
    cout << "TUNER::warm-up complete" << endl;
    changeState();
}

void Tuner::resetError() {
    running_count = 0;
    running_error = 0;
}

void Tuner::evalError() {
    auto average_error = running_error/running_count;
    cout << "average error for test session " << average_error << endl;
    resetError();
    if (average_error < best_error) {
        cout << "improved error: " << average_error << endl;
        best_error = average_error;
        best_params = params;
        changeState();
        params[current_index] += deltas[current_index];
    } else {
        resetBestParam();
        switch (directionState) {
            case Positive:
                decrement();
                directionState = Negative;
                break;
            case Negative:
                deltas[current_index] *= .9;
                cout << "adjusted deltas to " << deltas[0] << " " << deltas[1] << endl;
                changeState();
                increment();
                break;
        }
    }
}

void Tuner::resetBestParam() {
    params = best_params;
}

void Tuner::changeState() {
    directionState = Positive;
    switch (state) {
        case WarmUp:
            transitionToKp();
            break;
        case KpTune:
            transitionToKd();
            break;
        case KdTune:
            if (accumulate(deltas.begin(), deltas.end(), 0.) > -.1) {
                return transitionToCompleted();
            }
            transitionToKp();
        case Completed:
            return;
    }
}

void Tuner::transitionToKp() {
    current_index = 0;
    state = KpTune;
}

void Tuner::transitionToKd() {
    current_index = 1;
    state = KdTune;
}

void Tuner::transitionToCompleted() {
    resetBestParam();
    state = Completed;
    cout << "completed with params:" << endl;

    for (auto param : params) {
        cout << param << endl;
    }
}

void Tuner::increment() {
    params[current_index] += deltas[current_index];
}

void Tuner::decrement() {
    params[current_index] -= deltas[current_index];
}

