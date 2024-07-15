#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath> 

using namespace std;

class Point {
    public:
        int x;
        int y;
};

class AngleComparator {
    public:
        Point centroid;
        AngleComparator(const Point& c) : centroid(c) {}
        bool operator()(const Point& a, const Point& b) 
        {
            double angleA = atan2(a.y - centroid.y, a.x - centroid.x);
            double angleB = atan2(b.y - centroid.y, b.x - centroid.x);
            return angleA > angleB; 
        }
};

class Shape {
    public:
        string name;
        vector<Point> points;
        Point calculateCentroid(const vector<Point>& points) {
            int xSum = 0, ySum = 0;
            for (size_t i = 0; i < points.size(); ++i) {
                xSum += points[i].x;
                ySum += points[i].y;
            }
            int n = points.size();
            return Point{xSum / n, ySum / n};
        }
        void sortPointsClockwise(Shape& shape) {
            Point centroid = calculateCentroid(shape.points);
            sort(shape.points.begin(), shape.points.end(), AngleComparator(centroid));
            shape.points.push_back(shape.points[0]); 
        }
        double calculateArea(const Shape& shape) {
            double area = 0.0;
            const vector<Point>& points = shape.points;
            int n = points.size() - 1; 
            for (int i = 0; i < n; ++i) {
                area += points[i].x * points[i + 1].y - points[i].y * points[i + 1].x;
            }
            return abs(area) / 2.0;
        }
};

vector<Shape> readFromFile() {
    ifstream infile("data.txt");
    vector<Shape> shapes; //create a vector of shapes
    string line;
    while (getline(infile, line)) {
        size_t commaPos = line.find(','); //find the first comma
        string shapeName = line.substr(0, commaPos);
        string coordsStr = line.substr(commaPos + 2, line.size() - commaPos - 3);
        coordsStr.erase(remove(coordsStr.begin(), coordsStr.end(), ' '), coordsStr.end());
        vector<Point> points;
        size_t pos = 0;
        string delimiter = "),(";
        while ((pos = coordsStr.find(delimiter)) != string::npos) {
            string coord = coordsStr.substr(0, pos + 1);
            coordsStr.erase(0, pos + delimiter.length() - 1);
            coord.erase(remove(coord.begin(), coord.end(), '('), coord.end());
            coord.erase(remove(coord.begin(), coord.end(), ')'), coord.end());
            size_t comma = coord.find(',');
            int x = atoi(coord.substr(0, comma).c_str());
            int y = atoi(coord.substr(comma + 1).c_str());
            points.push_back(Point{x, y});
        }

        coordsStr.erase(remove(coordsStr.begin(), coordsStr.end(), '('), coordsStr.end());
        coordsStr.erase(remove(coordsStr.begin(), coordsStr.end(), ')'), coordsStr.end());
        size_t comma = coordsStr.find(',');
        if (comma != string::npos) {
            int x = atoi(coordsStr.substr(0, comma).c_str()); //convert the string to integer
            int y = atoi(coordsStr.substr(comma + 1).c_str());
            points.push_back(Point{x, y});
        }

        Shape shape = {shapeName, points};
        shapes.push_back(shape); //push back the shape
    }
    infile.close();
    return shapes;
}

int main() {
    vector<Shape> shapes = readFromFile();

    for (size_t i = 0; i < shapes.size(); ++i) {
        shapes[i].sortPointsClockwise(shapes[i]);
    }

    for (size_t i = 0; i < shapes.size(); ++i) 
    {
        double area = shapes[i].calculateArea(shapes[i]);
        printf("Area of %s: %.2f\n", shapes[i].name.c_str(), area);
        /*for (size_t j = 0; j < shapes[i].points.size(); ++j) {
            printf("(%d, %d) ", shapes[i].points[j].x, shapes[i].points[j].y);
        }
        printf("\n");
        printf("Area: %.2f\n", area);
        */
    }

    return 0;
}
