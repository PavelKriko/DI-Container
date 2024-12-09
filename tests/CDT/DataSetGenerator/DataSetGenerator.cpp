#include "DataSetGenerator.h"

/////// NEED TO REMAKE, VERY BAD WAY TO DETECT COLLISION
void normalize(double& x, double& y) {
    const double size = sqrt(x*x+y*y);
    x = x / size;
    y = y / size;
}

void turnPoint(Point& point_to_turn, double angle, Point turn_center)
{
    point_to_turn = {
        (point_to_turn.x - turn_center.x) * cos(angle) - (point_to_turn.y - turn_center.y) * cos(angle) + turn_center.x,
        (point_to_turn.x - turn_center.x) * sin(angle) + (point_to_turn.y - turn_center.y) * cos(angle) + turn_center.y
    };
}


Projection projectionsOxOy(const std::vector<Point>& points) {

    std::vector<double> ox_value;
    std::vector<double> oy_value;
    for (auto& elem : points) {
        ox_value.push_back(elem.x);
        oy_value.push_back(elem.y);
    }

    return { *std::min_element(ox_value.begin(),ox_value.end()),
             *std::max_element(ox_value.begin(),ox_value.end()),
             *std::min_element(oy_value.begin(),oy_value.end()),
             *std::max_element(oy_value.begin(),oy_value.end())
    };
}



bool isCollision(const Rectangle& rec1, const Rectangle& rec2) {
    //build projections of figures on the axis
    auto projection_1 = projectionsOxOy(rec1.getVertexes());

    auto projection_2 = projectionsOxOy(rec2.getVertexes());

    return ((projection_1.min_x <= projection_2.min_x && projection_2.min_x <= projection_1.max_x) || (projection_1.min_x <= projection_2.max_x && projection_2.max_x <= projection_1.max_x))
        &&
        ((projection_1.min_y <= projection_2.min_y && projection_2.min_y <= projection_1.max_y) || (projection_1.min_y <= projection_2.max_y && projection_2.max_y <= projection_1.max_y));

}


void createDataSet(const std::string& name, int sizeOfDataSet) {

    csvfile csv(name);

    GaussGenerator gen_coordinate(0, 100);
    GaussGenerator gen_size(20, 6);

    int countTrue = 0;
    int countFalse = 0;

    csv << "X1-X2" << "Y1-Y2" << "Height_1 - Height_2" << "Width_1 - Width_2" << "x_dir1 - x_dir2"<<"y_dir1 - y_dir2" << "isCollision" << endrow;
    while ((countTrue + countFalse) < sizeOfDataSet) {

        double x1 = gen_coordinate();
        double y1 = gen_coordinate();

        double height1 = std::trunc(abs(gen_size()));
        double width1 = std::trunc(abs(gen_size()));
        
        double x_dir1 = gen_coordinate();
        double y_dir1 = gen_coordinate();

        normalize(x_dir1, y_dir1);

        double x2 = gen_coordinate();
        double y2 = gen_coordinate();

        double height2 = std::trunc(abs(gen_size()));
        double width2 = std::trunc(abs(gen_size()));
        
        double x_dir2 = gen_coordinate();
        double y_dir2 = gen_coordinate();

        normalize(x_dir2, y_dir2);

        bool isColl = isCollision({ x1,y1,height1,width1,x_dir1,y_dir1}, { x2,y2,height2,width2,x_dir2,y_dir2});

        if (countTrue < sizeOfDataSet / 2 && isColl) {
            countTrue++;
            csv << x1 - x2 << y1 - y2 << height1 - height2 << width1 - width2 << x_dir1 - x_dir2<<y_dir1-y_dir2 << true << endrow;
        }
        if (countFalse < sizeOfDataSet / 2 && !isColl) {
            countFalse++;
            csv << x1 - x2 << y1 - y2 << height1 - height2 << width1 - width2 << x_dir1 - x_dir2 << y_dir1 - y_dir2 << false << endrow;
        }
    }
}