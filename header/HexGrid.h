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
// OffsetHex
// ================================================================
// Офсетные координаты
// ================================================================
class OffsetHex {
public:
    OffsetHex( int col_, int row_ ) : col( col_ ), row( row_ ) {}
    const int col;
    const int row;
};

// ================================================================
// OffsetHex
// ================================================================
// Операции над офсетными координатами
// ================================================================
bool operator ==( const OffsetHex& left, const OffsetHex& right );
bool operator !=( const OffsetHex& left, const OffsetHex& right );
ostream& operator <<( ostream& os, const OffsetHex& right );

// ================================================================
// Hex
// ================================================================
// Гекс (Кубические координаты)
// ================================================================
class Hex {
public:
    Hex( int q_, int r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    inline int Q( void ) const { return coord[ 0 ]; }
    inline int R( void ) const { return coord[ 1 ]; }
    inline int S( void ) const { return coord[ 2 ]; }
    const vector<int> coord;

    // Соседний гекс
    Hex HexNeighbor( int direction ) const;

    // Соседние гексы
    vector<Hex> HexNeighbors() const;

    // Диагональный гекс
    Hex HexDiagonal( int direction ) const;

    // Диагональные гексы
    vector<Hex> HexDiagonals() const;

    // Координаты гекса => Координаты на плоскости (координаты центра)
    Point HexToPixel( const HexLayout& layout ) const;

    // Угол гекса на плоскости
    Point HexCorner( const HexLayout& layout, int corner ) const;

    // Углы гекса на плоскости
    vector<Point> HexCorners( const HexLayout& layout ) const;

    // Поворот гекса влево
    Hex HexRotateLeft() const;

    // Поворот гекса вправо
    Hex HexRotateRight() const;
};

// ================================================================
// Hex
// ================================================================
// Операции над кубическими координатами
// ================================================================
bool operator ==( const Hex& left, const Hex& right );
bool operator !=( const Hex& left, const Hex& right );
Hex operator +( const Hex& left, const Hex& right );
Hex operator -( const Hex& left, const Hex& right );
Hex operator *( const Hex& left, int right );
ostream& operator <<( ostream& os, const Hex& right );

// ================================================================
// FractionalHex
// ================================================================
// Дробные координаты (Кубические)
// ================================================================
class FractionalHex {
public:
    FractionalHex( double q_, double r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    FractionalHex( double q_, double r_, double s_ );
    inline double Q( void ) const { return coord[ 0 ]; }
    inline double R( void ) const { return coord[ 1 ]; }
    inline double S( void ) const { return coord[ 2 ]; }
    const vector<double> coord;

    // Округление дробных координат
    Hex Round( int round_algorithm ) const;
};

// ================================================================
// HexDistance
// ================================================================
// Расстояние в гексах
// ================================================================
inline unsigned HexDistance( const Hex& hex_a, const Hex& hex_b ) {
    return ( abs( hex_a.Q() - hex_b.Q() )
             + abs( hex_a.R() - hex_b.R() )
             + abs( hex_a.S() - hex_b.S() ) ) / 2.0L;
}

// ================================================================
// HexDirection
// ================================================================
// Направление
// ================================================================
const Hex& HexDirection( int direction );

// ================================================================
// PixelToHex
// ================================================================
// Координаты на плоскости => Координаты гекса
// ================================================================
FractionalHex PixelToHex( const HexLayout& layout, const Point& pixel );

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalHex HexLinearInterpolation( const Hex& hex_a, const Hex& hex_b, double t );

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Дробные координаты)
// ================================================================
FractionalHex HexLinearInterpolation( const FractionalHex& hex_a, const FractionalHex& hex_b, double t );

// ================================================================
// HexLine
// ================================================================
// Линия гексов
// ================================================================
vector<Hex> HexLine( const Hex& hex_a, const Hex& hex_b, bool use_range, unsigned range );

// ================================================================
// Offset_to_Cube
// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
Hex Offset_to_Cube( const HexLayout& layout, const OffsetHex& offset );

// ================================================================
// Cube_to_Offset
// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
OffsetHex Cube_to_Offset( const HexLayout& layout, const Hex& hex );

#endif /* HEXGRID_H___VLADBOYR */
