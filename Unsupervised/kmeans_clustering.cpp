#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct kmeans_result {
    vector<double> lables;
    vector<vector<double>> centres;
};

class Kmeans {
    // ALL VECTORS PASSED IN ARE DOUBLE VECTORS 
    int max_iterations = 20;

    private:
        bool validate_input_array(vector<vector<double>> vector_space) {
            if (vector_space.size() < n_clusters || vector_space.size() < 2) {
                return false;
            }
            int sz = vector_space[0].size();
            if (sz == 0) {
                return false;
            }
            for (vector<double> v: vector_space) {
                if (v.size() != sz) {
                    return false;
                }
            }
            return true;
        }

        vector<double> add(vector<double> x, vector<double> y) {
            // No need to validate as whole input vector space is validated.
            vector<double> xpy;
            for (int i=0; i<x.size(); i++) {
                xpy.push_back(x[i]+y[i]);
            }
            return xpy;
        }

        vector<double> minus(vector<double> x, vector<double> y) {
            // No need to validate as whole input vector space is validated.
            vector<double> xmy;
            for (int i=0; i<x.size(); i++) {
                xmy.push_back(x[i]-y[i]);
            }
            return xmy;
        }

        vector<double> multiply(double lambda, vector<double> x) {
            // No need to validate as whole input vector space is validated.
            vector<double> lx;
            for (int i=0; i<x.size(); i++) {
                lx.push_back(x[i]*lambda);
            }
            return lx;
        }

        double euclidean_distance(vector<double> v, vector<double> w) {
            vector<double> x = minus(v,w);
            int sz = 0;
            for (double i: x) {
                sz += pow(i, 2);
            }
            return sqrt(sz);
        }

        vector<double> average_vector(vector<vector<double>> vector_set) {
            vector<double> a = vector_set[0];
            double sz = vector_set.size();
            for (int i=1; i<vector_set.size(); i++) {
                a = add(a, vector_set[i]);
            }
            return multiply(1/sz, a);
        }

        void print(vector<double> v) {
            cout << '(';
            for (int i=1; i<v.size(); i++) {
                cout << v[i-1] << ',';
            }
            cout << v[v.size()-1] << ')';
        }

        vector<vector<double>> assign_clusters(vector<vector<double>> centres, vector<vector<double>> vector_space) {
            vector<double> lables;
            vector<double> distances;
            for (vector<double> v: vector_space) {
                double min_distance = euclidean_distance(v, centres[0]);
                double lable = 0;
                for (int c=0; c<centres.size(); c++) {
                    if (euclidean_distance(v,centres[c])<min_distance) {
                        min_distance = euclidean_distance(v,centres[c]);
                        lable = c;
                    }
                }
                lables.push_back(lable);
                distances.push_back(min_distance);
            }
            return {lables, distances};
        }

        vector<vector<double>> get_new_centres(vector<double> lables, vector<vector<double>> vector_space) {
            vector<vector<double>> clusters[n_clusters];
            for (int i=0; i<lables.size(); i++) {
                int l = lables[i];
                clusters[l].push_back(vector_space[i]);
            }
            vector<vector<double>> new_centres;
            for (vector<vector<double>> cluster: clusters) {
                new_centres.push_back(average_vector(cluster));
            }
            return new_centres;
        }

        bool check_convergence(vector<vector<double>> new_centres, vector<vector<double>> old_centres) {
            for (int i=0; i<new_centres.size(); i++) {
                if (euclidean_distance(new_centres[i], old_centres[i])>0) {
                    return false;
                }
            }
            return true;
        }
        

    public:
        int n_clusters=0;

        kmeans_result fit(vector<vector<double>> vector_space) {
            if (validate_input_array(vector_space) == false) {
                cout << "Enter a valid array and cluster size" << endl;
                return {};
            }
            vector<vector<double>> initial_centres;
            for (int i=0; i<n_clusters; i++) {
                initial_centres.push_back(vector_space[i]);
            }
            vector<vector<double>> old_centres = initial_centres;
            vector<vector<double>> new_centres;
            vector<double> new_lables;
            for (int iter=0; iter<max_iterations; iter++) {
                new_lables = assign_clusters(old_centres, vector_space)[0];
                new_centres = get_new_centres(new_lables, vector_space);
                if (check_convergence(new_centres, old_centres)==true) {
                    break;
                }
                cout << iter << endl;
                old_centres = new_centres;
            }
            kmeans_result result;
            result.lables = new_lables;
            result.centres = new_centres;
            return result;
        } 

};

int main() {
    Kmeans test_class;
    test_class.n_clusters = 2;
    kmeans_result f = test_class.fit({{0,0},{1,0},{0,1},{1,1}});
    for (double i: f.lables) {
        cout << i << ' ';
    }
    return 0;
}
