/*
 * HexGrid.h
 *
 * Source & Copyright: http://www.redblobgames.com/grids/hexagons/
 */

#pragma once
#ifndef HEXGRID_H___VLADBOYR
#define HEXGRID_H___VLADBOYR

#include <cmath>
#include <map>
#include <iostream>
#include <vector>

using std::map;
using std::ostream;
using std::vector;

// ================================================================
// HEX_ROUND_ALGORITHM_RANDOM
// ================================================================
// Случайный выбор алгоритма округления (0 = отключить)
// ================================================================
#define HEX_ROUND_ALGORITHM_RANDOM 1

// ================================================================
// HEX_ROUND_ALGORITHM
// ================================================================
// Установить конкретный алгоритм округления (значение 0..2)
// ================================================================
#define HEX_ROUND_ALGORITHM 0

// ================================================================
// OffsetType_t
// ================================================================
// Тип офсетной системы
// ================================================================
// OFFSET_TYPE_ODD = Нечётная
// OFFSET_TYPE_EVEN = Чётная
// ================================================================
enum OffsetType_t {
    OFFSET_TYPE_ODD = -1,
    OFFSET_TYPE_EVEN = 1
};

// ================================================================
// HexOrientation_t
// ================================================================
// Ориентация гекса на плоскости
// ================================================================
// HEX_ORIENTATION_FLAT = Плоский
// HEX_ORIENTATION_POINTY = Заострённый
// ================================================================
enum HexOrientation_t {
    HEX_ORIENTATION_FLAT = 0,
    HEX_ORIENTATION_POINTY = 1
};

// ================================================================
// Point
// ================================================================
// Точка на плоскости
// ================================================================
class Point {
public:
    Point( double x_, double y_ ) : x( x_ ), y( y_ ) {}
    const double x;
    const double y;
};

// ================================================================
// Point
// ================================================================
// Операции над точками
// ================================================================
bool operator ==( const Point& left, const Point& right );
bool operator !=( const Point& left, const Point& right );
Point operator +( const Point& left, const Point& right );
Point operator -( const Point& left, const Point& right );
Point operator *( const Point& left, int right );
ostream& operator <<( ostream& os, const Point& right );

// ================================================================
// HexOffset
// ================================================================
// Офсетные координаты
// ================================================================
class HexOffset {
public:
    HexOffset( int col_, int row_ ) : col( col_ ), row( row_ ) {}
    const int col;
    const int row;
};

// ================================================================
// HexOffset
// ================================================================
// Операции над офсетными координатами
// ================================================================
bool operator ==( const HexOffset& left, const HexOffset& right );
bool operator !=( const HexOffset& left, const HexOffset& right );
ostream& operator <<( ostream& os, const HexOffset& right );

// ================================================================
// HexCube
// ================================================================
// Кубические координаты
// ================================================================
class HexCube {
public:
    HexCube( int q_, int r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    inline int Q( void ) const { return coord[ 0 ]; }
    inline int R( void ) const { return coord[ 1 ]; }
    inline int S( void ) const { return coord[ 2 ]; }
    const vector<int> coord;
};

// ================================================================
// HexCube
// ================================================================
// Операции над кубическими координатами
// ================================================================
bool operator ==( const HexCube& left, const HexCube& right );
bool operator !=( const HexCube& left, const HexCube& right );
HexCube operator +( const HexCube& left, const HexCube& right );
HexCube operator -( const HexCube& left, const HexCube& right );
HexCube operator *( const HexCube& left, int right );
ostream& operator <<( ostream& os, const HexCube& right );

// ================================================================
// FractionalCube
// ================================================================
// Кубические координаты (дробные)
// ================================================================
class FractionalCube {
public:
    FractionalCube( double q_, double r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    FractionalCube( double q_, double r_, double s_ );
    inline double Q( void ) const { return coord[ 0 ]; }
    inline double R( void ) const { return coord[ 1 ]; }
    inline double S( void ) const { return coord[ 2 ]; }
    const vector<double> coord;

    // Округление кубических координат
    HexCube Round( int round_algorithm ) const;
};

// ================================================================
// HEX_ORIENTATION_MATRIX_1
// ================================================================
// Матрица для преобразования координат гекса в координаты на плоскости
// ================================================================
const map<HexOrientation_t, vector<double>> HEX_ORIENTATION_MATRIX_1 = {
    { HEX_ORIENTATION_FLAT, { 1.5L, 0.0L, 0.5L * sqrt( 3.0L ), sqrt( 3.0L ) } },
    { HEX_ORIENTATION_POINTY, { sqrt( 3.0L ), 0.5L * sqrt( 3.0L ), 0.0L, 1.5L } }
};

// ================================================================
// HEX_ORIENTATION_MATRIX_2
// ================================================================
// Матрица для преобразования координат на плоскости в координаты гекса
// ================================================================
const map<HexOrientation_t, vector<double>> HEX_ORIENTATION_MATRIX_2 = {
    { HEX_ORIENTATION_FLAT, { 2.0L / 3.0L, 0.0L, -1.0L / 3.0L, sqrt( 3.0L ) / 3.0L } },
    { HEX_ORIENTATION_POINTY, { sqrt( 3.0L ) / 3.0L,  -1.0L / 3.0L, 0.0L, 2.0L / 3.0L } }
};

// ================================================================
// HexLayout
// ================================================================
// Расположение гексов на плоскости
// ================================================================
class HexLayout {
public:
    HexLayout( HexOrientation_t orientation_, OffsetType_t offset_type_, Point size_, Point origin_ ) :
        orientation( orientation_ ), offset_type( offset_type_ ), size( size_ ), origin( origin_ ),
        start_angle( ( orientation_ == HEX_ORIENTATION_POINTY ? 0.5L : 0.0L ) ) {}

    // Ориентация гекса на плоскости
    const HexOrientation_t orientation;

    // Используемая офсетная система
    const OffsetType_t offset_type;

    // Размеры гекса
    const Point size;

    // Начало плоскости
    const Point origin;

    // Начальный угол
    const double start_angle;

    // Коэффициенты для преобразования координат гекса в координаты на плоскости
    inline double QX() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[0]; }
    inline double RX() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[1]; }
    inline double QY() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[2]; }
    inline double RY() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[3]; }

    // Коэффициенты для преобразования координат на плоскости в координаты гекса
    inline double XQ() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[0]; }
    inline double YQ() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[1]; }
    inline double XR() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[2]; }
    inline double YR() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[3]; }
};

// ================================================================
// HexDistance
// ================================================================
// Расстояние в гексах
// ================================================================
inline unsigned HexDistance( const HexCube& cube_a, const HexCube& cube_b ) {
    return ( abs( cube_a.Q() - cube_b.Q() )
             + abs( cube_a.R() - cube_b.R() )
             + abs( cube_a.S() - cube_b.S() ) ) / 2.0L;
}

// ================================================================
// HexDirection
// ================================================================
// Направление
// ================================================================
const HexCube& HexDirection( int direction );

// ================================================================
// HexNeighbor
// ================================================================
// Соседний гекс
// ================================================================
HexCube HexNeighbor( const HexCube& cube, int direction );

// ================================================================
// HexNeighbors
// ================================================================
// Соседние гексы
// ================================================================
vector<HexCube> HexNeighbors( const HexCube& cube );

// ================================================================
// HexDiagonal
// ================================================================
// Диагональный гекс
// ================================================================
HexCube HexDiagonal( const HexCube& cube, int direction );

// ================================================================
// HexDiagonals
// ================================================================
// Диагональные гексы
// ================================================================
vector<HexCube> HexDiagonals( const HexCube& cube );

// ================================================================
// HexToPixel
// ================================================================
// Координаты гекса => Координаты на плоскости (координаты центра)
// ================================================================
Point HexToPixel( const HexLayout& layout, const HexCube& cube );

// ================================================================
// PixelToHex
// ================================================================
// Координаты на плоскости => Координаты гекса
// ================================================================
FractionalCube PixelToHex( const HexLayout& layout, const Point& pixel );

// ================================================================
// HexCorner
// ================================================================
// Угол гекса на плоскости
// ================================================================
Point HexCorner( const HexLayout& layout, const HexCube& cube, int corner );

// ================================================================
// HexCorners
// ================================================================
// Углы гекса на плоскости
// ================================================================
vector<Point> HexCorners( const HexLayout& layout, const HexCube& cube );

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalCube HexLinearInterpolation( const HexCube& cube_a, const HexCube& cube_b, double t );

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalCube HexLinearInterpolation( const FractionalCube& cube_a, const FractionalCube& cube_b, double t );

// ================================================================
// HexLine
// ================================================================
// Линия гексов
// ================================================================
vector<HexCube> HexLine( const HexCube& cube_a, const HexCube& cube_b, bool use_range, unsigned range );

// ================================================================
// Offset_to_Cube
// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
HexCube Offset_to_Cube( const HexLayout& layout, const HexOffset& offset );

// ================================================================
// Cube_to_Offset
// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
HexOffset Cube_to_Offset( const HexLayout& layout, const HexCube& cube );

// ================================================================
// HexRotateLeft
// ================================================================
// Поворот гекса влево
// ================================================================
HexCube HexRotateLeft( const HexCube& cube );

// ================================================================
// HexRotateRight
// ================================================================
// Поворот гекса вправо
// ================================================================
HexCube HexRotateRight( const HexCube& cube );

#endif /* HEXGRID_H___VLADBOYR */
