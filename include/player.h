#pragma once

class Player {
public:
	Player(int x, int y) : x_(x), y_(y) {};
	int Getx() const { return x_; };
	int Gety() const { return y_; };
	void move(int dx, int dy) { x_ += dx; y_ += dy; };
private:
	int x_;
	int y_;
};