// Source & Copyright: http://www.redblobgames.com/grids/hexagons/

unit HexGridUnit;

interface

type
  TTest = class
  private
    i: integer;
  public
    constructor Create(x, y: integer);
  end;

const

  // ================================================================
  // ��������� ����� ��������� ���������� (0 = ���������)
  // ================================================================
  HEX_ROUND_ALGORITHM_RANDOM = 1;

  // ================================================================
  // ���������� ���������� �������� ���������� (�������� 0..2)
  // ================================================================
  HEX_ROUND_ALGORITHM = 0;

  // ================================================================
  // ���������� �����������
  // ================================================================
  HEX_DIRECTION_COUNT = 6;

  // ================================================================
  // ���������� ����� �����
  // ================================================================
  HEX_CORNERS_COUNT = 6;

  //a: array[0..2] of TTest = (3,1,7,2,12,-1);

type
  // ================================================================
  // ��� �������� �������
  // ================================================================
  // OFFSET_TYPE_ODD = ��������
  // OFFSET_TYPE_EVEN = ׸����
  // ================================================================
  OffsetType_t = (
    OFFSET_TYPE_ODD = -1,
    OFFSET_TYPE_EVEN = 1
  );

  // ================================================================
  // ���������� ����� �� ���������
  // ================================================================
  // HEX_ORIENTATION_FLAT = �������
  // HEX_ORIENTATION_POINTY = ����������
  // ================================================================
  HexOrientation_t = (
    HEX_ORIENTATION_FLAT = 0,
    HEX_ORIENTATION_POINTY = 1
  );

implementation

constructor TTest.Create(x, y: integer);
begin
  i := x * y;
end;

procedure Test;
var
  b: TTest;
{$J+}
const
  c: TTest;// = Create(0, 3);
{$J-}
begin
  b := TTest.Create(0, 3);
end;

end.



// ================================================================
// ������ �����������
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
// ������ ����������
// ================================================================
const vector<Hex> HEX_DIAGONALS = {
    Hex( 2, -1 ),
    Hex( 1, -2 ),
    Hex( -1, -1 ),
    Hex( -2, 1 ),
    Hex( -1, 2 ),
    Hex( 1, 1 )
};

#include <ctime>

// �������� ����� ��
#define M_PI 3.141592653589793L

using std::abs;
using std::max;
using std::logic_error;

// ================================================================
// �������� ��� �������
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
// �������� ��� ��������� ������������
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
// �������� ��� ����������� ������������
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
// ����������� �� ������
// ================================================================
unsigned AbsDirection( int direction ) {
    direction %= HEX_DIRECTION_COUNT;

    if ( direction < 0 ) {
        direction += HEX_DIRECTION_COUNT;
    }

    return direction;
}

// ================================================================
// �����������
// ================================================================
const Hex& HexDirection( int direction ) {
    return HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// �������� ����
// ================================================================
Hex Hex::HexNeighbor( int direction ) const {
    return (*this) + HEX_DIRECTIONS[ AbsDirection( direction ) ];
}

// ================================================================
// �������� �����
// ================================================================
vector<Hex> Hex::HexNeighbors() const {
    vector<Hex> neighbors;

    for ( const auto& hexDirection : HEX_DIRECTIONS ) {
        neighbors.push_back( (*this) + hexDirection );
    }

    return neighbors;
}

// ================================================================
// ������������ ����
// ================================================================
Hex Hex::HexDiagonal( int direction ) const {
    return (*this) + HEX_DIAGONALS[ AbsDirection( direction ) ];
}

// ================================================================
// ������������ �����
// ================================================================
vector<Hex> Hex::HexDiagonals() const {
    vector<Hex> diagonals;

    for ( const auto& hexDiagonal : HEX_DIAGONALS ) {
        diagonals.push_back( (*this) + hexDiagonal );
    }

    return diagonals;
}

// ================================================================
// ���������� ����� => ���������� �� ��������� (���������� ������)
// ================================================================
Point Hex::HexToPixel( const HexLayout& layout ) const {
    const double x = ( layout.QX() * Q() + layout.RX() * R() ) * layout.size.x;
    const double y = ( layout.QY() * Q() + layout.RY() * R() ) * layout.size.y;
    return Point( x + layout.origin.x, y + layout.origin.y );
}

// ================================================================
// ���������� �� ��������� => ���������� �����
// ================================================================
FractionalHex PixelToHex( const HexLayout& layout, const Point& pixel ) {
    const double x = ( pixel.x - layout.origin.x ) / layout.size.x;
    const double y = ( pixel.y - layout.origin.y ) / layout.size.y;
    const double q = ( layout.XQ() * x + layout.YQ() * y );
    const double r = ( layout.XR() * x + layout.YR() * y );
    return FractionalHex( q, r );
}

// ================================================================
// ���� ����� �� ��������� (������� �������)
// ================================================================
Point HexCornerBase( const HexLayout& layout, int corner ) {
    double angle = M_PI * ( layout.start_angle + corner ) / 3.0L;
    return Point( layout.size.x * cos( angle ), layout.size.y * sin( angle ) );
}

// ================================================================
// ���� ����� �� ���������
// ================================================================
Point Hex::HexCorner( const HexLayout& layout, int corner ) const {
    return HexToPixel( layout ) + HexCornerBase( layout, corner );
}

// ================================================================
// ���� ����� �� ���������
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
// ����� ��������� ���������� ���������� ���������
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
// ������� ���������� (����������)
// ================================================================
FractionalHex::FractionalHex( double q_, double r_, double s_ ) : coord( { q_, r_, s_ } ) {
    if ( round(Q() + R() + S()) != 0) {
        throw logic_error("Q + R + S must be 0.");
    }
}

// ================================================================
// ���������� ���������� ���������
// ================================================================
Hex FractionalHex::Round( int round_algorithm ) const {
    // ��������� ����������
    const double q = int( round( Q() ) );
    const double r = int( round( R() ) );
    const double s = int( round( S() ) );

    // �������� ������ ����������
    double q_diff = abs( q - Q() );
    double r_diff = abs( r - R() );
    double s_diff = abs( s - S() );

    // ����� ��������� ����������, ���� �� �� ������
    if ( round_algorithm < 0 ) {
        round_algorithm = HexRoundAlgorithm();
    }

    // ����������� ���������� � ���������� �������
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
// �������� ������������
// ================================================================
inline double LinearInterpolation( double a, double b, double t ) {
    return a * ( 1 - t ) + b * t;
}

// ================================================================
// �������� ������������ (���������� ����������)
// ================================================================
FractionalHex HexLinearInterpolation( const Hex& hex_a, const Hex& hex_b, double t ) {
    return FractionalHex(
               LinearInterpolation( hex_a.Q(), hex_b.Q(), t ),
               LinearInterpolation( hex_a.R(), hex_b.R(), t ),
               LinearInterpolation( hex_a.S(), hex_b.S(), t ) );
}

// ================================================================
// �������� ������������ (������� ����������)
// ================================================================
FractionalHex HexLinearInterpolation( const FractionalHex& hex_a, const FractionalHex& hex_b, double t ) {
    return FractionalHex(
               LinearInterpolation( hex_a.Q(), hex_b.Q(), t ),
               LinearInterpolation( hex_a.R(), hex_b.R(), t ),
               LinearInterpolation( hex_a.S(), hex_b.S(), t ) );
}

// ================================================================
// ����� ������
// ================================================================
vector<Hex> HexLine( const Hex& hex_a, const Hex& hex_b, bool use_range, unsigned range ) {
    // ����� ��������� ����������
    const int round_algorithm = HexRoundAlgorithm();

    // ���������� ����� �������
    const unsigned distance = HexDistance( hex_a, hex_b );

    // ������������ ������
    if ( !use_range || range > distance ) {
        range = distance;
    }

    // ������ ����� ������ (�������� ������������)
    vector<Hex> hex_line;
    double step = 1.0L / max( distance, 1U );

    for ( unsigned i = 0; i <= range; ++i ) {
        hex_line.push_back( HexLinearInterpolation( hex_a, hex_b, step * i ).Round( round_algorithm ) );
    }

    return hex_line;
}

// ================================================================
// �������� ���������� -> ���������� ����������
// ================================================================
// ���������� ����� �� ��������� "�������" (HEX_ORIENTATION_FLAT)
// ================================================================
Hex Q_Offset_to_Cube( OffsetType_t offset_type, const OffsetHex& offset ) {
    int q = offset.col;
    int r = offset.row - ( offset.col + static_cast<int>( offset_type ) * ( offset.col & 1 ) ) / 2;
    return Hex( q, r );
}

// ================================================================
// �������� ���������� -> ���������� ����������
// ================================================================
// ���������� ����� �� ��������� "����������" (HEX_ORIENTATION_POINTY)
// ================================================================
Hex R_Offset_to_Cube( OffsetType_t offset_type, const OffsetHex& offset ) {
    int q = offset.col - ( offset.row + static_cast<int>( offset_type ) * ( offset.row & 1 ) ) / 2;
    int r = offset.row;
    return Hex( q, r );
}

// ================================================================
// �������� ���������� -> ���������� ����������
// ================================================================
Hex Offset_to_Cube( const HexLayout& layout, const OffsetHex& offset ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Q_Offset_to_Cube( layout.offset_type, offset );
    } else {
        return R_Offset_to_Cube( layout.offset_type, offset );
    }
}

// ================================================================
// ���������� ���������� -> �������� ����������
// ================================================================
// ���������� ����� �� ��������� "�������" (HEX_ORIENTATION_FLAT)
// ================================================================
OffsetHex Cube_to_Offset_Q( OffsetType_t offset_type, const Hex& hex ) {
    int col = hex.Q();
    int row = hex.R() + ( hex.Q() + static_cast<int>( offset_type ) * ( hex.Q() & 1 ) ) / 2;
    return OffsetHex( col, row );
}

// ================================================================
// ���������� ���������� -> �������� ����������
// ================================================================
// ���������� ����� �� ��������� "����������" (HEX_ORIENTATION_POINTY)
// ================================================================
OffsetHex Cube_to_Offset_R( OffsetType_t offset_type, const Hex& hex ) {
    int col = hex.Q() + ( hex.R() + static_cast<int>( offset_type ) * ( hex.R() & 1 ) ) / 2;
    int row = hex.R();
    return OffsetHex( col, row );
}

// ================================================================
// ���������� ���������� -> �������� ����������
// ================================================================
OffsetHex Cube_to_Offset( const HexLayout& layout, const Hex& hex ) {
    if ( layout.orientation == HEX_ORIENTATION_FLAT ) {
        return Cube_to_Offset_Q( layout.offset_type, cube );
    } else {
        return Cube_to_Offset_R( layout.offset_type, cube );
    }
}

// ================================================================
// ������� ����� �����
// ================================================================
Hex Hex::HexRotateLeft() const {
    return Hex( -S(), -Q() );
}

// ================================================================
// ������� ����� ������
// ================================================================
Hex Hex::HexRotateRight() const {
    return Hex( -R(), -S() );
}



#include <cmath>
#include <map>
#include <iostream>
#include <vector>
using std::map;
using std::ostream;
using std::vector;

// ================================================================
// ������� ��� �������������� ��������� ����� � ���������� �� ���������
// ================================================================
const map<HexOrientation_t, vector<double>> HEX_ORIENTATION_MATRIX_1 = {
    { HEX_ORIENTATION_FLAT, { 1.5L, 0.0L, 0.5L * sqrt( 3.0L ), sqrt( 3.0L ) } },
    { HEX_ORIENTATION_POINTY, { sqrt( 3.0L ), 0.5L * sqrt( 3.0L ), 0.0L, 1.5L } }
};

// ================================================================
// ������� ��� �������������� ��������� �� ��������� � ���������� �����
// ================================================================
const map<HexOrientation_t, vector<double>> HEX_ORIENTATION_MATRIX_2 = {
    { HEX_ORIENTATION_FLAT, { 2.0L / 3.0L, 0.0L, -1.0L / 3.0L, sqrt( 3.0L ) / 3.0L } },
    { HEX_ORIENTATION_POINTY, { sqrt( 3.0L ) / 3.0L,  -1.0L / 3.0L, 0.0L, 2.0L / 3.0L } }
};

// ================================================================
// ������������ ������ �� ���������
// ================================================================
class HexLayout {
public:
    HexLayout( HexOrientation_t orientation_, OffsetType_t offset_type_, Point size_, Point origin_ ) :
        orientation( orientation_ ), offset_type( offset_type_ ), size( size_ ), origin( origin_ ),
        start_angle( ( orientation_ == HEX_ORIENTATION_POINTY ? 0.5L : 0.0L ) ) {}

    // ���������� ����� �� ���������
    const HexOrientation_t orientation;

    // ������������ �������� �������
    const OffsetType_t offset_type;

    // ������� �����
    const Point size;

    // ������ ���������
    const Point origin;

    // ��������� ����
    const double start_angle;

    // ������������ ��� �������������� ��������� ����� � ���������� �� ���������
    inline double QX() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[0]; }
    inline double RX() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[1]; }
    inline double QY() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[2]; }
    inline double RY() const { return HEX_ORIENTATION_MATRIX_1.at( orientation )[3]; }

    // ������������ ��� �������������� ��������� �� ��������� � ���������� �����
    inline double XQ() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[0]; }
    inline double YQ() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[1]; }
    inline double XR() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[2]; }
    inline double YR() const { return HEX_ORIENTATION_MATRIX_2.at( orientation )[3]; }
};

// ================================================================
// ����� �� ���������
// ================================================================
class Point {
public:
    Point( double x_, double y_ ) : x( x_ ), y( y_ ) {}
    const double x;
    const double y;
};

// ================================================================
// �������� ��� �������
// ================================================================
bool operator ==( const Point& left, const Point& right );
bool operator !=( const Point& left, const Point& right );
Point operator +( const Point& left, const Point& right );
Point operator -( const Point& left, const Point& right );
Point operator *( const Point& left, int right );
ostream& operator <<( ostream& os, const Point& right );

// ================================================================
// �������� ����������
// ================================================================
class OffsetHex {
public:
    OffsetHex( int col_, int row_ ) : col( col_ ), row( row_ ) {}
    const int col;
    const int row;
};

// ================================================================
// �������� ��� ��������� ������������
// ================================================================
bool operator ==( const OffsetHex& left, const OffsetHex& right );
bool operator !=( const OffsetHex& left, const OffsetHex& right );
ostream& operator <<( ostream& os, const OffsetHex& right );

// ================================================================
// ���� (���������� ����������)
// ================================================================
class Hex {
public:
    Hex( int q_, int r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    inline int Q( void ) const { return coord[ 0 ]; }
    inline int R( void ) const { return coord[ 1 ]; }
    inline int S( void ) const { return coord[ 2 ]; }
    const vector<int> coord;

    // �������� ����
    Hex HexNeighbor( int direction ) const;

    // �������� �����
    vector<Hex> HexNeighbors() const;

    // ������������ ����
    Hex HexDiagonal( int direction ) const;

    // ������������ �����
    vector<Hex> HexDiagonals() const;

    // ���������� ����� => ���������� �� ��������� (���������� ������)
    Point HexToPixel( const HexLayout& layout ) const;

    // ���� ����� �� ���������
    Point HexCorner( const HexLayout& layout, int corner ) const;

    // ���� ����� �� ���������
    vector<Point> HexCorners( const HexLayout& layout ) const;

    // ������� ����� �����
    Hex HexRotateLeft() const;

    // ������� ����� ������
    Hex HexRotateRight() const;
};

// ================================================================
// �������� ��� ����������� ������������
// ================================================================
bool operator ==( const Hex& left, const Hex& right );
bool operator !=( const Hex& left, const Hex& right );
Hex operator +( const Hex& left, const Hex& right );
Hex operator -( const Hex& left, const Hex& right );
Hex operator *( const Hex& left, int right );
ostream& operator <<( ostream& os, const Hex& right );

// ================================================================
// ������� ���������� (����������)
// ================================================================
class FractionalHex {
public:
    FractionalHex( double q_, double r_ ) : coord( { q_, r_, -q_ - r_ } ) {}
    FractionalHex( double q_, double r_, double s_ );
    inline double Q( void ) const { return coord[ 0 ]; }
    inline double R( void ) const { return coord[ 1 ]; }
    inline double S( void ) const { return coord[ 2 ]; }
    const vector<double> coord;

    // ���������� ������� ���������
    Hex Round( int round_algorithm ) const;
};

// ================================================================
// ���������� � ������
// ================================================================
inline unsigned HexDistance( const Hex& hex_a, const Hex& hex_b ) {
    return ( abs( hex_a.Q() - hex_b.Q() )
             + abs( hex_a.R() - hex_b.R() )
             + abs( hex_a.S() - hex_b.S() ) ) / 2.0L;
}

// ================================================================
// �����������
// ================================================================
const Hex& HexDirection( int direction );

// ================================================================
// ���������� �� ��������� => ���������� �����
// ================================================================
FractionalHex PixelToHex( const HexLayout& layout, const Point& pixel );

// ================================================================
// �������� ������������ (���������� ����������)
// ================================================================
FractionalHex HexLinearInterpolation( const Hex& hex_a, const Hex& hex_b, double t );

// ================================================================
// �������� ������������ (������� ����������)
// ================================================================
FractionalHex HexLinearInterpolation( const FractionalHex& hex_a, const FractionalHex& hex_b, double t );

// ================================================================
// ����� ������
// ================================================================
vector<Hex> HexLine( const Hex& hex_a, const Hex& hex_b, bool use_range, unsigned range );

// ================================================================
// �������� ���������� -> ���������� ����������
// ================================================================
Hex Offset_to_Cube( const HexLayout& layout, const OffsetHex& offset );

// ================================================================
// ���������� ���������� -> �������� ����������
// ================================================================
OffsetHex Cube_to_Offset( const HexLayout& layout, const Hex& hex );

