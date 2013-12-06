

def print_contours(contours, shape):
    f = open('../PolygonDetector/contours.txt', 'w')
    def writeln(str_):
        f.write(str(str_))
        f.write('\n')
    writeln("{} {}".format(shape[1], shape[0]))
    writeln(len(contours))
    for c in contours:
        writeln(str(len(c)))
        for i in range(0, len(c)):
            x = c[i][0][0]
            y = c[i][0][1]
            writeln("({}, {})".format(x,y))
    f.close()