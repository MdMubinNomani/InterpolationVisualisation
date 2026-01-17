#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using namespace std;

int n;
vector<double> x, y;
double queryX;
int DEGREE;

/* ---------- INTERPOLATION METHODS ---------- */

double lagrange(double xp) {
    double yp = 0;
    for (int i = 0; i <= DEGREE; i++) {
        double term = y[i];
        for (int j = 0; j <= DEGREE; j++) {
            if (j != i)
                term *= (xp - x[j]) / (x[i] - x[j]);
        }
        yp += term;
    }
    return yp;
}


double newtonDD(double xp) {
    int m = DEGREE + 1;
    vector<vector<double>> d(m, vector<double>(m));

    for (int i = 0; i < m; i++)
        d[i][0] = y[i];

    for (int j = 1; j < m; j++)
        for (int i = 0; i < m - j; i++)
            d[i][j] = (d[i + 1][j - 1] - d[i][j - 1]) /
                      (x[i + j] - x[i]);

    double res = d[0][0];
    double term = 1;

    for (int i = 1; i < m; i++) {
        term *= (xp - x[i - 1]);
        res += term * d[0][i];
    }
    return res;
}

double newtonForward(double xp) {
    int m = DEGREE + 1;
    vector<vector<double>> d(m, vector<double>(m));

    for (int i = 0; i < m; i++)
        d[i][0] = y[i];

    for (int j = 1; j < m; j++)
        for (int i = 0; i < m - j; i++)
            d[i][j] = d[i + 1][j - 1] - d[i][j - 1];

    double h = x[1] - x[0];
    double u = (xp - x[0]) / h;

    double res = y[0];
    double term = 1;

    for (int i = 1; i < m; i++) {
        term *= (u - (i - 1)) / i;
        res += term * d[0][i];
    }
    return res;
}

/* ---------- ERROR ESTIMATION ---------- */

void errorEstimation() {
    cout << "\nError Estimation (Lagrange):\n";
    for (int i = 0; i < n; i++) {
        double err = fabs(lagrange(x[i]) - y[i]);
        cout << "x = " << x[i] << " | Error = " << err << "\n";
    }
}

/* ---------- DATA EXPORT FOR PYTHON ---------- */

void generateDataFiles() {
    ofstream fl("lagrange_data.txt"),
             fd("newton_dd_data.txt"),
             ff("newton_forward_data.txt");

    for (double xp = x[0]; xp <= x[n - 1]; xp += 0.1) {
        fl << xp << " " << lagrange(xp) << "\n";
        fd << xp << " " << newtonDD(xp) << "\n";
        ff << xp << " " << newtonForward(xp) << "\n";
    }
    fl.close(); fd.close(); ff.close();
}

/* ---------- PYTHON SCRIPT CREATION ---------- */

void createPythonScript() {
    ofstream py("plot_interpolation.py");

    py <<
"import matplotlib.pyplot as plt\n"
"\n"
"QUERY_X = float(open('query_x.txt').read())\n"
"\n"
"def read_data(file):\n"
"    x, y = [], []\n"
"    for line in open(file):\n"
"        a, b = map(float, line.split())\n"
"        x.append(a)\n"
"        y.append(b)\n"
"    return x, y\n"
"\n"
"actual_x, actual_y = read_data('actual_data.txt')\n"
"\n"
"def plot(interp_file, title, out):\n"
"    xi, yi = read_data(interp_file)\n"
"\n"
"    # Find interpolated value at QUERY_X\n"
"    y_query = None\n"
"    for i in range(len(xi) - 1):\n"
"        if xi[i] <= QUERY_X <= xi[i + 1]:\n"
"            y_query = yi[i]\n"
"            break\n"
"\n"
"    plt.plot(actual_x, actual_y, 'bo', label='Actual Data')\n"
"    plt.plot(xi, yi, 'k-', label='Interpolated Curve')\n"
"    plt.plot(QUERY_X, y_query, 'rx', markersize=10, label='Interpolated Value')\n"
"\n"
"    plt.xlabel('x')\n"
"    plt.ylabel('y')\n"
"    plt.title(title)\n"
"    plt.legend()\n"
"    plt.grid(True)\n"
"    plt.savefig(out)\n"
"    plt.clf()\n"
"\n"
"plot('lagrange_data.txt', 'Lagrange Interpolation', 'lagrange.png')\n"
"plot('newton_dd_data.txt', 'Newton Divided Difference', 'newton_dd.png')\n"
"plot('newton_forward_data.txt', 'Newton Forward Interpolation', 'newton_forward.png')\n";

    py.close();
}

/* ---------- GRAPH GENERATION ---------- */

void generateGraphs() {
    generateDataFiles();

    ofstream actual("actual_data.txt");
    for (int i = 0; i < n; i++)
        actual << x[i] << " " << y[i] << "\n";
    actual.close();

    ofstream qx("query_x.txt");
    qx << queryX;
    qx.close();

    createPythonScript();
    system("python plot_interpolation.py");
    cout << "\nGraphs generated successfully (PNG files).\n";
}

/* ---------- MENU ---------- */

void menu() {
    cout << "\n----- MENU -----\n";
    cout << "1. Lagrange Interpolation\n";
    cout << "2. Newton Divided Difference\n";
    cout << "3. Newton Forward Interpolation\n";
    cout << "4. Compare All Methods\n";
    cout << "5. Export Results to output.txt\n";
    cout << "6. Generate Graphs (Python)\n";
    cout << "7. Error Estimation\n";
    cout << "8. Exit\n";
    cout << "Enter choice: ";
}

/* ---------- MAIN ---------- */

int main() {
    ifstream fin("input.txt");
    fin >> n;
    DEGREE = min(n-1, 6);

    x.resize(n); y.resize(n);
    for (int i = 0; i < n; i++) fin >> x[i] >> y[i];
    fin >> queryX;
    fin.close();

    int ch;
    do {
        menu();
        cin >> ch;

        switch (ch) {
            case 1: cout << lagrange(queryX) << "\n"; break;
            case 2: cout << newtonDD(queryX) << "\n"; break;
            case 3: cout << newtonForward(queryX) << "\n"; break;
            case 4:
                cout << lagrange(queryX) << "\n"
                     << newtonDD(queryX) << "\n"
                     << newtonForward(queryX) << "\n";
                break;
            case 6: generateGraphs(); break;
            case 7: errorEstimation(); break;
        }
    } while (ch != 8);

    return 0;
}
