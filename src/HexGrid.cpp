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
// Количество направлений
// ================================================================
const unsigned HEX_DIRECTION_COUNT = 6;

// ================================================================
// Количество углов гекса
// ================================================================
const unsigned HEX_CORNERS_COUNT = 6;

// ================================================================
// Список направлений
// ================================================================
const vector<Hex> HEX_DIRECTIONS = {
    Hex( 1, 0 ),
    Hex( 1, -1 ),
    Hex( 0, -1 ),
    Hex( -1, 0 ),
    Hex( -1, 1 ),
    Hex( 0, 1 )
};

// ================================================================
// Список диагоналей
// ================================================================
const vector<Hex> HEX_DIAGONALS = {
    Hex( 2, -1 ),
    Hex( 1, -2 ),
    Hex( -1, -1 ),
    Hex( -2, 1 ),
    Hex( -1, 2 ),
    Hex( 1, 1 )
};

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
// Операции над офсетными координатами
// ================================================================
bool operator ==( const OffsetHex& left, const OffsetHex& right ) {
    return ( left.col == right.col && left.row == right.row );
}

bool operator !=( const OffsetHex& left, const OffsetHex& right ) {
    return ( left.col != right.col || left.row != right.row );
}

ostream& operator <<( ostream& os, const OffsetHex& right ) {
    os << "OffsetHex(" << right.col << "," << right.row << ")";
    return os;
}

// ================================================================
// Операции над кубическими координатами
// ================================================================
bool operator ==( const Hex& left, const Hex& right ) {
    return ( left.coord == right.coord );
}

bool operator !=( const Hex& left, const Hex& right ) {
    return ( left.coord != right.coord );
}

Hex operator +( const Hex& left, const Hex& right ) {
    return Hex( left.Q() + right.Q(), left.R() + right.R() );
}

Hex operator -( const Hex& left, const Hex& right ) {
    return Hex( left.Q() - right.Q(), left.R() - right.R() );
}

Hex operator *( const Hex& left, int right ) {
    return Hex( left.Q() * right, left.R() * right );
}

ostream& operator <<( ostream& os, const Hex& right ) {
    os << "Hex(" << right.Q() << "," << right.R() << "," << right.S() << ")";
    return os;
}

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
// Направление
// ================================================================
const Hex& HexDirection( int direction ) {
    return HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// Соседний гекс
// ================================================================
Hex Hex::HexNeighbor( int direction ) const {
    return (*this) + HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// Соседние гексы
// ================================================================
vector<Hex> Hex::HexNeighbors() const {
    vector<Hex> neighbors;

    for ( const auto& hexDirection : HEX_DIRECTIONS ) {
        neighbors.push_back( (*this) + hexDirection );
    }

    return neighbors;
}

// ================================================================
// Диагональный гекс
// ================================================================
Hex Hex::HexDiagonal( int direction ) const {
    return (*this) + HEX_DIAGONALS[ AbsDirection( direction ) ];
}

// ================================================================
// Диагональные гексы
// ================================================================
vector<Hex> Hex::HexDiagonals() const {
    vector<Hex> diagonals;

    for ( const auto& hexDiagonal : HEX_DIAGONALS ) {
        diagonals.push_back( (*this) + hexDiagonal );
    }

    return diagonals;
}

// ================================================================
// Координаты гекса => Координаты на плоскости (координаты центра)
// ================================================================
Point Hex::HexToPixel( const HexLayout& layout ) const {
    const double x = ( layout.QX() * Q() + layout.RX() * R() ) * layout.size.x;
    const double y = ( layout.QY() * Q() + layout.RY() * R() ) * layout.size.y;
    return Point( x + layout.origin.x, y + layout.origin.y );
}

// ================================================================
// Координаты на плоскости => Координаты гекса
// ================================================================
FractionalHex PixelToHex( const HexLayout& layout, const Point& pixel ) {
    const double x = ( pixel.x - layout.origin.x ) / layout.size.x;
    const double y = ( pixel.y - layout.origin.y ) / layout.size.y;
    const double q = ( layout.XQ() * x + layout.YQ() * y );
    const double r = ( layout.XR() * x + layout.YR() * y );
    return FractionalHex( q, r );
}

// ================================================================
// Угол гекса на плоскости (Базовая функция)
// ================================================================
Point HexCornerBase( const HexLayout& layout, int corner ) {
    double angle = M_PI * ( layout.start_angle + corner ) / 3.0L;
    return Point( layout.size.x * cos( angle ), layout.size.y * sin( angle ) );
}

// ================================================================
// Угол гекса на плоскости
// ================================================================
Point Hex::HexCorner( const HexLayout& layout, int corner ) const {
    return HexToPixel( layout ) + HexCornerBase( layout, corner );
}

// ================================================================
// Углы гекса на плоскости
// ================================================================
vector<Point> Hex::HexCorners( const HexLayout& layout ) const {
    vector<Point> corners;
    const Point& center = HexToPixel( layout );

    for ( size_t i = 0; i != HEX_CORNERS_COUNT; i++ ) {
        corners.push_back( center + HexCornerBase( layout, i ) );
    }

    return corners;
}

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
// Дробные координаты (Кубические)
// ================================================================
FractionalHex::FractionalHex( double q_, double r_, double s_ ) : coord( { q_, r_, s_ } ) {
    if ( round(Q() + R() + S()) != 0) {
        throw logic_error("Q + R + S must be 0.");
    }
}

// ================================================================
// Округление кубических координат
// ================================================================
Hex FractionalHex::Round( int round_algorithm ) const {
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
                return Hex( -r - s, r );
            } else if ( r_diff > s_diff ) {
                return Hex( q, -q - s );
            } else {
                return Hex( q, r );
            }

        case 1:
            if ( r_diff > q_diff && r_diff > s_diff ) {
                return Hex( q, -q - s );
            } else if ( s_diff > q_diff ) {
                return Hex( q, r );
            } else {
                return Hex( -r - s, r );
            }

        default:
            if ( s_diff > q_diff && s_diff > r_diff ) {
                return Hex( q, r );
            } else if ( q_diff > r_diff ) {
                return Hex( -r - s, r );
            } else {
                return Hex( q, -q - s );
            }
    }
}

// ================================================================
// Линейная интерполяция
// ================================================================
inline double LinearInterpolation( double a, double b, double t ) {
    return a * ( 1 - t ) + b * t;
}

// ================================================================
// Линейная интерполяция (Кубические координаты)
// ================================================================
FractionalHex HexLinearInterpolation( const Hex& hex_a, const Hex& hex_b, double t ) {
    return FractionalHex(
               LinearInterpolation( hex_a.Q(), hex_b.Q(), t ),
               LinearInterpolation( hex_a.R(), hex_b.R(), t ),
               LinearInterpolation( hex_a.S(), hex_b.S(), t ) );
}

// ================================================================
// Линейная интерполяция (Дробные координаты)
// ================================================================
FractionalHex HexLinearInterpolation( const FractionalHex& hex_a, const FractionalHex& hex_b, double t ) {
    return FractionalHex(
               LinearInterpolation( hex_a.Q(), hex_b.Q(), t ),
               LinearInterpolation( hex_a.R(), hex_b.R(), t ),
               LinearInterpolation( hex_a.S(), hex_b.S(), t ) );
}

// ================================================================
// Линия гексов
// ================================================================
vector<Hex> HexLine( const Hex& hex_a, const Hex& hex_b, bool use_range, unsigned range ) {
    // Выбор алгоритма округления
    const int round_algorithm = HexRoundAlgorithm();

    // Расстояние между гексами
    const unsigned distance = HexDistance( hex_a, hex_b );

    // Максимальный радиус
    if ( !use_range || range > distance ) {
        range = distance;
    }

    // Строим линию гексов (Линейная интерполяция)
    vector<Hex> hex_line;
    double step = 1.0L / max( distance, 1U );

    for ( unsigned i = 0; i <= range; ++i ) {
        hex_line.push_back( HexLinearInterpolation( hex_a, hex_b, step * i ).Round( round_algorithm ) );
    }

    return hex_line;
}

// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
// Ориентация гекса на плоскости "Плоский" (HEX_ORIENTATION_FLAT)
// ================================================================
Hex Q_Offset_to_Cube( OffsetType_t offset_type, const OffsetHex& offset ) {
    int q = offset.col;
    int r = offset.row - ( offset.col + static_cast<int>( offset_type ) * ( offset.col & 1 ) ) / 2;
    return Hex( q, r );
}

// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
// Ориентация гекса на плоскости "Заострённый" (HEX_ORIENTATION_POINTY)
// ================================================================
Hex R_Offset_to_Cube( OffsetType_t offset_type, const OffsetHex& offset ) {
    int q = offset.col - ( offset.row + static_cast<int>( offset_type ) * ( offset.row & 1 ) ) / 2;
    int r = offset.row;
    return Hex( q, r );
}

// ================================================================
// Офсетные координаты -> Кубические координаты
// ================================================================
Hex Offset_to_Cube( const HexLayout& layout, const OffsetHex& offset ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Q_Offset_to_Cube( layout.offset_type, offset );
    } else {
        return R_Offset_to_Cube( layout.offset_type, offset );
    }
}

// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
// Ориентация гекса на плоскости "Плоский" (HEX_ORIENTATION_FLAT)
// ================================================================
OffsetHex Cube_to_Offset_Q( OffsetType_t offset_type, const Hex& hex ) {
    int col = hex.Q();
    int row = hex.R() + ( hex.Q() + static_cast<int>( offset_type ) * ( hex.Q() & 1 ) ) / 2;
    return OffsetHex( col, row );
}

// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
// Ориентация гекса на плоскости "Заострённый" (HEX_ORIENTATION_POINTY)
// ================================================================
OffsetHex Cube_to_Offset_R( OffsetType_t offset_type, const Hex& hex ) {
    int col = hex.Q() + ( hex.R() + static_cast<int>( offset_type ) * ( hex.R() & 1 ) ) / 2;
    int row = hex.R();
    return OffsetHex( col, row );
}

// ================================================================
// Кубические координаты -> Офсетные координаты
// ================================================================
OffsetHex Cube_to_Offset( const HexLayout& layout, const Hex& hex ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Cube_to_Offset_Q( layout.offset_type, cube );
    } else {
        return Cube_to_Offset_R( layout.offset_type, cube );
    }
}

// ================================================================
// Поворот гекса влево
// ================================================================
Hex Hex::HexRotateLeft() const {
    return Hex( -S(), -Q() );
}

// ================================================================
// Поворот гекса вправо
// ================================================================
Hex Hex::HexRotateRight() const {
    return Hex( -R(), -S() );
}
