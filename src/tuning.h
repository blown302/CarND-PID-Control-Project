//
// Created by Thomas Milas on 9/30/18.
//

#ifndef PID_TUNING_H
#define PID_TUNING_H

#include <iostream>
#include <cmath>
#include <limits>
#include <vector>
#include <numeric>      // std::accumulate

using namespace std;

/**
 * Implements a twiddle like algorithm to optimize parameters for a PID controller.
 */
class Tuner {
    enum TuneState {WarmUp, KpTune, KdTune, Completed};
    enum DeltaDirectionState {Positive, Negative};
private:
    TuneState state{WarmUp};
    DeltaDirectionState directionState{Positive};
    vector<double> params;
    vector<double> deltas{-.1,-1};
    unsigned int current_index{0};
    double best_error{numeric_limits<double>::infinity()};
    vector<double> best_params{0, 0};
    double running_error{0};
    unsigned int running_count{0};
public:
    /**
     * Constructor to initialize Dp and kd with initial values.
     * @param paramKp Initial Kp value
     * @param paramKd Initial Kd value
     */
    explicit Tuner(double paramKp, double paramKd) {
        params = vector<double>{paramKp, paramKd};
    }

    /**
     * Gets parms current param vector for params to test.
     * @return double vector with current parameter vector of the tuner.
     */
    vector<double> getParam();

    /**
     * Add error to current test session.
     * @param error The error to add to this session.
     */
    void updateParamError(double error);

    /**
     * Evaluate the session error.
     */
    void evaluate();

private:
    void warmUpEval();
    void resetError();
    void evalError();
    void resetBestParam();
    void changeState();
    void transitionToKp();
    void transitionToKd();
    void transitionToCompleted();
    void increment();
    void decrement();
};

#endif //PID_TUNING_H
