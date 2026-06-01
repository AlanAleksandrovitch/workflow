#include "min_circle.h"

static constexpr double EPS = 1e-8;

double distanceSquared(const Point2D &p1, const Point2D &p2){
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return dx * dx + dy * dy;
}

double distance(const Point2D &p1, const Point2D &p2){
	return std::sqrt(distanceSquared(p1, p2));
}

Point2D findMidPoint(const LineSegment &segment){
	double mx = (segment.start.x + segment.end.x) / 2;
	double my = (segment.start.y + segment.end.y) / 2;
	return Point2D{mx, my};
}

Circle circleFromDiameter(const Point2D &p1, const Point2D &p2){
	Point2D center{(p1.x + p2.x) / 2, (p1.y + p2.y) / 2};
	return Circle{center, distance(p1, p2) / 2};
}

Circle circleFrom3Points(const Point2D &p1, const Point2D &p2, const Point2D &p3){
	double ax = p2.x - p1.x;
	double ay = p2.y - p1.y;
	double bx = p3.x - p1.x;
	double by = p3.y - p1.y;
	double d = 2 * (ax * by - ay * bx);
	if (std::fabs(d) < EPS){
		double d12 = distanceSquared(p1, p2);
		double d13 = distanceSquared(p1, p3);
		double d23 = distanceSquared(p2, p3);
		if (d12 >= d13 && d12 >= d23)
			return circleFromDiameter(p1, p2);
		if (d13 >= d23)
			return circleFromDiameter(p1, p3);
		return circleFromDiameter(p2, p3);
	}
	double a2 = ax * ax + ay * ay;
	double b2 = bx * bx + by * by;
	double cx = p1.x + (by * a2 - ay * b2) / d;
	double cy = p1.y + (ax * b2 - bx * a2) / d;
	Point2D center{cx, cy};
	return Circle{center, distance(center, p1)};
}

std::vector<Point2D> extractPointsFromSegments(const std::vector<LineSegment> &segments){
	std::vector<Point2D> points;
	points.reserve(segments.size() * 3);
	for (const auto &segment : segments){
		points.push_back(segment.start);
		points.push_back(segment.end);
		points.push_back(findMidPoint(segment));
	}
	return points;
}

Circle minCircleHelper(std::vector<Point2D> &points, std::vector<Point2D> boundary, size_t n){
	if (n == 0 || boundary.size() == 3){
		switch (boundary.size()){
			case 1:
				return Circle{boundary[0], 0};
			case 2:
				return circleFromDiameter(boundary[0], boundary[1]);
			case 3:
				return circleFrom3Points(boundary[0], boundary[1], boundary[2]);
			default:
				return Circle{{0, 0}, 0};
		}
	}
	Circle circle = minCircleHelper(points, boundary, n - 1);
	if (circle.contains(points[n - 1])){
		return circle;
	}
	boundary.push_back(points[n - 1]);
	return minCircleHelper(points, boundary, n - 1);
}

Circle minCircle(std::vector<Point2D> points){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(points.begin(), points.end(), gen);
	return minCircleHelper(points, {}, points.size());
}

Circle MinimumEnclosingCircleForSegments(const std::vector<LineSegment> &segments){
	if (segments.empty()){
		return Circle{{0, 0}, -1};
	}
	std::vector<Point2D> points = extractPointsFromSegments(segments);
	return minCircle(points);
}
