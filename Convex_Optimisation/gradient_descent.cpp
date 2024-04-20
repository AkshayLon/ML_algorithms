#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;

const double epsilon = 0.0000001;
const double termination_tolerance = 0.000001;

// Example function is f(x,y) = x^3 - 12xy + 8y^3

double fx(vector<double> x) {
    return (pow(x[0], 3) - (12*x[0]*x[1]) + (8*pow(x[1], 3)));
}

// Change above depending on problem

string display_coordinate(vector<double> x) {
    string display = "(";
    for (int i=0; i<x.size(); i++) {
        display += to_string(x[i]);
        if (i!=x.size()-1) {
            display += ",";
        }
    }
    display += ")";
    return display;
}

double dfdx(vector<double> x, int var_wrt) {
    vector<double> xph = x;
    xph[var_wrt] += epsilon;
    return (fx(xph)-fx(x))/epsilon;
}

vector<double> gradf(vector<double> x) {
    vector<double> grad;
    for (int i=0; i<x.size(); i++) {
        grad.push_back(dfdx(x,i));
    }
    return grad;
}

vector<double> calculate_x_kp1(vector<double> x_k, double learning_rate) {
    vector<double> x_kp1 = x_k;
    for (int i=0; i<x_k.size(); i++) {
        x_kp1[i] -= learning_rate*(gradf(x_k)[i]);
    }
    return x_kp1;
}

double eucl_dist(vector<double> x, vector<double> y) {
    double eucl = 0;
    for (int i=0; i<x.size(); i++) {
        eucl += pow(x[i]-y[i], 2);
    }
    return eucl;
}

void compute_algorithm(vector<double> x0, double learning_rate) {
    vector<double> x_k = x0;
    vector<double> x_kp1;
    int iter = 0;
    cout << "x_" << iter << " = " << display_coordinate(x_k) << " " << endl;
    while (true) {
        x_kp1 = calculate_x_kp1(x_k, learning_rate);
        iter++;
        cout << "x_" << iter << " = " << display_coordinate(x_kp1) << " " << endl;
        if (eucl_dist(x_kp1, x_k) < termination_tolerance) {
            break;
        }
        x_k = x_kp1; 
    }
    cout << "Computed minimum at x = " << display_coordinate(x_kp1) << endl;
}

int main() {
    vector<double> x0 = {0,0};
    compute_algorithm(x0, 0.01);
    return 0;
}
