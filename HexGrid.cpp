/*
 * HexGrid.cpp
 *
 * Source & Copyright: http://www.redblobgames.com/grids/hexagons/
 */

#include "HexGrid.h"

#include <ctime>

// Значение числа Пи
#ifndef M_PI
#define M_PI 3.141592653589793L
#endif

using std::abs;
using std::max;
using std::logic_error;

// ================================================================
// HEX_DIRECTION_COUNT
// ================================================================
// Количество направлений
// ================================================================
const unsigned HEX_DIRECTION_COUNT = 6;

// ================================================================
// HEX_CORNERS_COUNT
// ================================================================
// Количество углов гекса
// ================================================================
const unsigned HEX_CORNERS_COUNT = 6;

// ================================================================
// HEX_DIRECTIONS
// ================================================================
// Список направлений
// ================================================================
const vector<HexCube> HEX_DIRECTIONS = {
    HexCube( 1, 0 ),
    HexCube( 1, -1 ),
    HexCube( 0, -1 ),
    HexCube( -1, 0 ),
    HexCube( -1, 1 ),
    HexCube( 0, 1 )
};

// ================================================================
// HEX_DIAGONALS
// ================================================================
// Список диагоналей
// ================================================================
const vector<HexCube> HEX_DIAGONALS = {
    HexCube( 2, -1 ),
    HexCube( 1, -2 ),
    HexCube( -1, -1 ),
    HexCube( -2, 1 ),
    HexCube( -1, 2 ),
    HexCube( 1, 1 )
};

// ================================================================
// Point
// ================================================================
// Операции над точками
// ================================================================
bool operator ==( const Point& left, const Point& right ) {
    return ( left.x == right.x && left.y == right.y );
}

bool operator !=( const Point& left, const Point& right ) {
    return ( left.x != right.x || left.y != right.y );
}

Point operator +( const Point& left, const Point& right ) {
    return Point( left.x + right.x, left.y + right.y );
}

Point operator -( const Point& left, const Point& right ) {
    return Point( left.x - right.x, left.y - right.y );
}

Point operator *( const Point& left, int right ) {
    return Point( left.x * right, left.y * right );
}

ostream& operator <<( ostream& os, const Point& right ) {
    os << "Point(" << right.x << "," << right.y << ")";
    return os;
}

// ================================================================
// HexOffset
// ================================================================
// Операции над офсетными координатами
// ================================================================
bool operator ==( const HexOffset& left, const HexOffset& right ) {
    return ( left.col == right.col && left.row == right.row );
}

bool operator !=( const HexOffset& left, const HexOffset& right ) {
    return ( left.col != right.col || left.row != right.row );
}

ostream& operator <<( ostream& os, const HexOffset& right ) {
    os << "HexOffset(" << right.col << "," << right.row << ")";
    return os;
}

// ================================================================
// HexCube
// ================================================================
// Операции над кубическими координатами
// ================================================================
bool operator ==( const HexCube& left, const HexCube& right ) {
    return ( left.coord == right.coord );
}

bool operator !=( const HexCube& left, const HexCube& right ) {
    return ( left.coord != right.coord );
}

HexCube operator +( const HexCube& left, const HexCube& right ) {
    return HexCube( left.Q() + right.Q(), left.R() + right.R() );
}

HexCube operator -( const HexCube& left, const HexCube& right ) {
    return HexCube( left.Q() - right.Q(), left.R() - right.R() );
}

HexCube operator *( const HexCube& left, int right ) {
    return HexCube( left.Q() * right, left.R() * right );
}

ostream& operator <<( ostream& os, const HexCube& right ) {
    os << "HexCube(" << right.Q() << "," << right.R() << "," << right.S() << ")";
    return os;
}

// ================================================================
// HexRoundAlgorithm
// ================================================================
// Выбор алгоритма округления кубических координат
// ================================================================
int HexRoundAlgorithm() {
#if HEX_ROUND_ALGORITHM_RANDOM
    srand( time( 0 ) );
    return rand() % 3;
#else
    return HEX_ROUND_ALGORITHM;
#endif
}

// ================================================================
// FractionalCube
// ================================================================
// Кубические координаты (дробные)
// ================================================================
FractionalCube::FractionalCube( double q_, double r_, double s_ ) : coord( { q_, r_, s_ } ) {
    if ( round(Q() + R() + S()) != 0) {
        throw logic_error("Q + R + S must be 0.");
    }
}

// ================================================================
// FractionalCube::Round
// ================================================================
// Округление кубических координат
// ================================================================
HexCube FractionalCube::Round( int round_algorithm ) const {
    // Округляем координаты
    const double q = int( round( Q() ) );
    const double r = int( round( R() ) );
    const double s = int( round( S() ) );

    // Вычислим дельту округления
    double q_diff = abs( q - Q() );
    double r_diff = abs( r - R() );
    double s_diff = abs( s - S() );

    // Выбор алгоритма округления, если он не выбран
    if ( round_algorithm < 0 ) {
        round_algorithm = HexRoundAlgorithm();
    }

    // Отбрасываем координату с наибольшей дельтой
    switch ( round_algorithm ) {
        case 0:
            if ( q_diff > r_diff && q_diff > s_diff ) {
                return HexCube( -r - s, r );
            } else if ( r_diff > s_diff ) {
                return HexCube( q, -q - s );
            } else {
                return HexCube( q, r );
            }

        case 1:
            if ( r_diff > q_diff && r_diff > s_diff ) {
                return HexCube( q, -q - s );
            } else if ( s_diff > q_diff ) {
                return HexCube( q, r );
            } else {
                return HexCube( -r - s, r );
            }

        default:
            if ( s_diff > q_diff && s_diff > r_diff ) {
                return HexCube( q, r );
            } else if ( q_diff > r_diff ) {
                return HexCube( -r - s, r );
            } else {
                return HexCube( q, -q - s );
            }
    }
}

// ================================================================
// AbsDirection
// ================================================================
// Направление по модулю
// ================================================================
unsigned AbsDirection( int direction ) {
    direction %= HEX_DIRECTION_COUNT;

    if ( direction < 0 ) {
        direction += HEX_DIRECTION_COUNT;
    }

    return direction;
}

// ================================================================
// HexDirection
// ================================================================
// Направление
// ================================================================
const HexCube& HexDirection( int direction ) {
    return HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// HexNeighbor
// ================================================================
// Соседний гекс
// ================================================================
HexCube HexNeighbor( const HexCube& cube, int direction ) {
    return cube + HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// HexNeighbors
// ================================================================
// Соседние гексы
// ================================================================
vector<HexCube> HexNeighbors( const HexCube& cube ) {
    vector<HexCube> neighbors;

    for ( const auto& hexDirection : HEX_DIRECTIONS ) {
        neighbors.push_back( cube + hexDirection );
    }

    return neighbors;
}

// ================================================================
// HexDiagonal
// ================================================================
// Диагональный гекс
// ================================================================
HexCube HexDiagonal( const HexCube& cube, int direction ) {
    return cube + HEX_DIAGONALS[ AbsDirection( direction ) ];
}

// ================================================================
// HexDiagonals
// ================================================================
// Диагональные гексы
// ================================================================
vector<HexCube> HexDiagonals( const HexCube& cube ) {
    vector<HexCube> diagonals;

    for ( const auto& hexDiagonal : HEX_DIAGONALS ) {
        diagonals.push_back( cube + hexDiagonal );
    }

    return diagonals;
}

// ================================================================
// HexToPixel
// ================================================================
// Координаты гекса => Координаты на плоскости (координаты центра)
// ================================================================
Point HexToPixel( const HexLayout& layout, const HexCube& cube ) {
    const double x = ( layout.QX() * cube.Q() + layout.RX() * cube.R() ) * layout.size.x;
    const double y = ( layout.QY() * cube.Q() + layout.RY() * cube.R() ) * layout.size.y;
    return Point( x + layout.origin.x, y + layout.origin.y );
}

// ================================================================
// PixelToHex
// ================================================================
// Координаты на плоскости => Координаты гекса
// ================================================================
FractionalCube PixelToHex( const HexLayout& layout, const Point& pixel ) {
    const double x = ( pixel.x - layout.origin.x ) / layout.size.x;
    const double y = ( pixel.y - layout.origin.y ) / layout.size.y;
    const double q = ( layout.XQ() * x + layout.YQ() * y );
    const double r = ( layout.XR() * x + layout.YR() * y );
    return FractionalCube( q, r );
}

// ================================================================
// HexCornerBase
// ================================================================
// Угол гекса на плоскости (Базовая функция)
// ================================================================
Point HexCornerBase( const HexLayout& layout, int corner ) {
    double angle = M_PI * ( layout.start_angle + corner ) / 3.0L;
    return Point( layout.size.x * cos( angle ), layout.size.y * sin( angle ) );
}

// ================================================================
// HexCorner
// ================================================================
// Угол гекса на плоскости
// ================================================================
Point HexCorner( const HexLayout& layout, const HexCube& cube, int corner ) {
    return HexToPixel( layout, cube ) + HexCornerBase( layout, corner );
}

// ================================================================
// HexCorners
// ================================================================
// Углы гекса на плоскости
// ================================================================
vector<Point> HexCorners( const HexLayout& layout, const HexCube& cube ) {
    vector<Point> corners;
    const Point& center = HexToPixel( layout, cube );

    for ( size_t i = 0; i != HEX_CORNERS_COUNT; i++ ) {
        corners.push_back( center + HexCornerBase( layout, i ) );
    }

    return corners;
}

// ================================================================
// LinearInterpolation
// ================================================================
// Линейная интерполяция
// ================================================================
inline double LinearInterpolation( double a, double b, double t ) {
    return a * ( 1 - t ) + b * t;
}

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalCube HexLinearInterpolation( const HexCube& cube_a, const HexCube& cube_b, double t ) {
    return FractionalCube(
               LinearInterpolation( cube_a.Q(), cube_b.Q(), t ),
               LinearInterpolation( cube_a.R(), cube_b.R(), t ),
               LinearInterpolation( cube_a.S(), cube_b.S(), t ) );
}

// ================================================================
// HexLinearInterpolation
// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalCube HexLinearInterpolation( const FractionalCube& cube_a, const FractionalCube& cube_b, double t ) {
    return FractionalCube(
               LinearInterpolation( cube_a.Q(), cube_b.Q(), t ),
               LinearInterpolation( cube_a.R(), cube_b.R(), t ),
               LinearInterpolation( cube_a.S(), cube_b.S(), t ) );
}

// ================================================================
// HexLine
// ================================================================
// Линия гексов
// ================================================================
vector<HexCube> HexLine( const HexCube& cube_a, const HexCube& cube_b, bool use_range, unsigned range ) {
    // Выбор алгоритма округления
    const int round_algorithm = HexRoundAlgorithm();

    // Расстояние между гексами
    const unsigned distance = HexDistance( cube_a, cube_b );

    // Максимальный радиус
    if ( !use_range || range > distance ) {
        range = distance;
    }

    // Строим линию гексов (Линейная интерполяция)
    vector<HexCube> hex_line;
    double step = 1.0L / max( distance, 1U );

    for ( unsigned i = 0; i <= range; ++i ) {
        hex_line.push_back( HexLinearInterpolation( cube_a, cube_b, step * i ).Round( round_algorithm ) );
    }

    return hex_line;
}

// ================================================================
// Q_Offset_to_Cube
// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
// Ориентация гекса на плоскости "Плоский" (HEX_ORIENTATION_FLAT)
// ================================================================
HexCube Q_Offset_to_Cube( OffsetType_t offset_type, const HexOffset& offset ) {
    int q = offset.col;
    int r = offset.row - ( offset.col + static_cast<int>( offset_type ) * ( offset.col & 1 ) ) / 2;
    return HexCube( q, r );
}

// ================================================================
// R_Offset_to_Cube
// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
// Ориентация гекса на плоскости "Заострённый" (HEX_ORIENTATION_POINTY)
// ================================================================
HexCube R_Offset_to_Cube( OffsetType_t offset_type, const HexOffset& offset ) {
    int q = offset.col - ( offset.row + static_cast<int>( offset_type ) * ( offset.row & 1 ) ) / 2;
    int r = offset.row;
    return HexCube( q, r );
}

// ================================================================
// Offset_to_Cube
// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
HexCube Offset_to_Cube( const HexLayout& layout, const HexOffset& offset ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Q_Offset_to_Cube( layout.offset_type, offset );
    } else {
        return R_Offset_to_Cube( layout.offset_type, offset );
    }
}

// ================================================================
// Cube_to_Offset_Q
// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
// Ориентация гекса на плоскости "Плоский" (HEX_ORIENTATION_FLAT)
// ================================================================
HexOffset Cube_to_Offset_Q( OffsetType_t offset_type, const HexCube& cube ) {
    int col = cube.Q();
    int row = cube.R() + ( cube.Q() + static_cast<int>( offset_type ) * ( cube.Q() & 1 ) ) / 2;
    return HexOffset( col, row );
}

// ================================================================
// Cube_to_Offset_R
// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
// Ориентация гекса на плоскости "Заострённый" (HEX_ORIENTATION_POINTY)
// ================================================================
HexOffset Cube_to_Offset_R( OffsetType_t offset_type, const HexCube& cube ) {
    int col = cube.Q() + ( cube.R() + static_cast<int>( offset_type ) * ( cube.R() & 1 ) ) / 2;
    int row = cube.R();
    return HexOffset( col, row );
}

// ================================================================
// Cube_to_Offset
// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
HexOffset Cube_to_Offset( const HexLayout& layout, const HexCube& cube ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Cube_to_Offset_Q( layout.offset_type, cube );
    } else {
        return Cube_to_Offset_R( layout.offset_type, cube );
    }
}

// ================================================================
// HexRotateLeft
// ================================================================
// Поворот гекса влево
// ================================================================
HexCube HexRotateLeft( const HexCube& cube ) {
    return HexCube( -cube.S(), -cube.Q() );
}

// ================================================================
// HexRotateRight
// ================================================================
// Поворот гекса вправо
// ================================================================
HexCube HexRotateRight( const HexCube& cube ) {
    return HexCube( -cube.R(), -cube.S() );
}
