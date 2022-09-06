#pragma once

#include <cstdarg>
#include <iostream>
#include <stdexcept>

using std::out_of_range;
using std::cout;
using std::endl;


/**
 * Represents a point in Dim dimensional space.
 */
template <int Dim>
class Point {
  public:
    Point();

    Point(double arr[Dim]);
    template <typename T>
    explicit Point(T x, ...);
    template <typename T>
    explicit Point(T x0, T x1, T x2);


    Point& operator=(const Point& other);

    /**
     * Gets the value of the Point object in the given dimension
     */
    double operator[](int index) const;

    /**
     * Gets the value of the Point object in the given dimension
     * @param index The dimension of the Point to grab.
     * @return The value of the Point in the indexth dimension, by
     *	reference (so that it may be modified).
     */
    double& operator[](int index);

    bool operator==(const Point<Dim> p) const;
    bool operator!=(const Point<Dim> p) const;

    /**
     * Compares whether the given Point is smaller than the current
     * Point.
     *
     * @param p The other point to compare with.
     * @return A boolean value indicating whether the current Point is
     *  smaller than the provided Point p.
     */
    bool operator<(const Point<Dim> p) const;

    bool operator<=(const Point<Dim> p) const;
    bool operator>(const Point<Dim> p) const;
    bool operator>=(const Point<Dim> p) const;

    void set(int index, double val);
    void print(std::ostream& out = std::cout) const;

    bool isMine() const;

  private:
    double vals[Dim];



  public:
    // Pure virtual class for an action when a mine is touched.
    class MineAction {
    public:
      virtual void onMine(const Point<Dim> & point) const = 0;
    };

    // Ctor with a mine value
    Point(double arr[Dim], bool isMine, const MineAction * mineAction);

  private:
    // Default value of a mine is false.
    bool isMine_ = false;

    // Default value of the action is to do nothing.
    class MineAction_DoNothingImpl : public MineAction {
    public:
      void onMine(const Point<Dim> & point) const { }
    };

    MineAction_DoNothingImpl MineAction_DoNothing;
    const MineAction* mineAction_ = &MineAction_DoNothing;

};


template <int Dim>
std::ostream& operator<<(std::ostream& out, const Point<Dim>& p);

#include "point.cpp"
