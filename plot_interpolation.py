import matplotlib.pyplot as plt

QUERY_X = float(open('query_x.txt').read())

def read_data(file):
    x, y = [], []
    for line in open(file):
        a, b = map(float, line.split())
        x.append(a)
        y.append(b)
    return x, y

actual_x, actual_y = read_data('actual_data.txt')

def plot(interp_file, title, out):
    xi, yi = read_data(interp_file)

    # Find interpolated value at QUERY_X
    y_query = None
    for i in range(len(xi) - 1):
        if xi[i] <= QUERY_X <= xi[i + 1]:
            y_query = yi[i]
            break

    plt.plot(actual_x, actual_y, 'bo', label='Actual Data')
    plt.plot(xi, yi, 'k-', label='Interpolated Curve')
    plt.plot(QUERY_X, y_query, 'rx', markersize=10, label='Interpolated Value')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.savefig(out)
    plt.clf()

plot('lagrange_data.txt', 'Lagrange Interpolation', 'lagrange.png')
plot('newton_dd_data.txt', 'Newton Divided Difference', 'newton_dd.png')
plot('newton_forward_data.txt', 'Newton Forward Interpolation', 'newton_forward.png')
