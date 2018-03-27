/*
 * main.cpp
 *
 * Source & Copyright: http://www.redblobgames.com/grids/hexagons/
 */

#include "test_runner.h"
#include "HexGrid.h"

void Test_HexArithmetic() {
    AssertEqual( HexCube( 1, -3 ) + HexCube( 3, -7 ), HexCube( 4, -10 ), "HexCube + HexCube" );
    AssertEqual( HexCube( 1, -3 ) - HexCube( 3, -7 ), HexCube( -2, 4 ), "HexCube - HexCube" );
}

void Test_HexDirection() {
    AssertEqual( HexDirection( 2 ), HexCube( 0, -1 ), "HexDirection" );
}

void Test_HexNeighbor() {
    AssertEqual( HexNeighbor( HexCube( 1, -2 ), 2 ), HexCube( 1, -3 ), "HexNeighbor" );
}

void Test_HexDiagonal() {
    AssertEqual( HexDiagonal( HexCube( 1, -2 ), 3 ), HexCube( -1, -1 ), "HexDiagonal" );
}

void Test_HexDistance() {
    AssertEqual( HexDistance( HexCube( 3, -7 ), HexCube( 0, 0 ) ), 7U, "HexDistance" );
}

void Test_HexRotate() {
    AssertEqual( HexRotateRight( HexCube( 1, -3 ) ), HexCube( 3, -2 ), "HexRotate Right" );
    AssertEqual( HexRotateLeft( HexCube( 1, -3 ) ), HexCube( -2, -1 ), "HexRotate Left" );
}

void Test_HexRound() {
    FractionalCube a( 0.0L, 0.0L, 0.0L );
    FractionalCube b( 1.0L, -1.0L, 0.0L );
    FractionalCube c( 0.0L, -1.0L, 1.0L );

    HexCube x(0, 0), y( 10, -20 ), z( 5, -10 );
    AssertEqual( HexLinearInterpolation( x, y, 0.5L ).Round( -1 ), z, "HexRound 1 (-1)" );
    AssertEqual( HexLinearInterpolation( x, y, 0.5L ).Round( 0 ), z, "HexRound 1 (0)" );
    AssertEqual( HexLinearInterpolation( x, y, 0.5L ).Round( 1 ), z, "HexRound 1 (1)" );
    AssertEqual( HexLinearInterpolation( x, y, 0.5L ).Round( 2 ), z, "HexRound 1 (2)" );

    AssertEqual( HexLinearInterpolation( a, b, 0.499L ).Round( -1 ), a.Round( -1 ), "HexRound 2 (-1)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.499L ).Round( 0 ), a.Round( 0 ), "HexRound 2 (0)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.499L ).Round( 1 ), a.Round( 1 ), "HexRound 2 (1)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.499L ).Round( 2 ), a.Round( 2 ), "HexRound 2 (2)" );

    AssertEqual( HexLinearInterpolation( a, b, 0.501L ).Round( -1 ), b.Round( -1 ), "HexRound 3 (-1)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.501L ).Round( 0 ), b.Round( 0 ), "HexRound 3 (0)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.501L ).Round( 1 ), b.Round( 1 ), "HexRound 3 (1)" );
    AssertEqual( HexLinearInterpolation( a, b, 0.501L ).Round( 2 ), b.Round( 2 ), "HexRound 3 (2)" );

    double q1 = a.Q() * 0.4L + b.Q() * 0.3L + c.Q() * 0.3L;
    double r1 = a.R() * 0.4L + b.R() * 0.3L + c.R() * 0.3L;
    double s1 = a.S() * 0.4L + b.S() * 0.3L + c.S() * 0.3L;
    AssertEqual( FractionalCube( q1, r1, s1 ).Round( -1 ), a.Round( -1 ), "HexRound 4 (-1)" );
    AssertEqual( FractionalCube( q1, r1, s1 ).Round( 0 ), a.Round( 0 ), "HexRound 4 (0)" );
    AssertEqual( FractionalCube( q1, r1, s1 ).Round( 1 ), a.Round( 1 ), "HexRound 4 (1)" );
    AssertEqual( FractionalCube( q1, r1, s1 ).Round( 2 ), a.Round( 2 ), "HexRound 4 (2)" );

    double q2 = a.Q() * 0.3L + b.Q() * 0.3L + c.Q() * 0.4L;
    double r2 = a.R() * 0.3L + b.R() * 0.3L + c.R() * 0.4L;
    double s2 = a.S() * 0.3L + b.S() * 0.3L + c.S() * 0.4L;
    AssertEqual( FractionalCube( q2, r2, s2 ).Round( -1 ), c.Round( -1 ), "HexRound 5 (-1)" );
    AssertEqual( FractionalCube( q2, r2, s2 ).Round( 0 ), c.Round( 0 ), "HexRound 5 (0)" );
    AssertEqual( FractionalCube( q2, r2, s2 ).Round( 1 ), c.Round( 1 ), "HexRound 5 (1)" );
    AssertEqual( FractionalCube( q2, r2, s2 ).Round( 2 ), c.Round( 2 ), "HexRound 5 (2)" );
}

void Test_HexLine() {
    AssertEqual( HexLine( HexCube( 0, 0 ), HexCube( 1, -5 ), false, 0 ),
                vector<HexCube> { HexCube( 0, 0 ), HexCube( 0, -1 ), HexCube( 0, -2 ),
                HexCube( 1, -3 ), HexCube( 1, -4 ), HexCube( 1, -5 )}, "HexLine" );
}

void Test_HexLayout() {
    HexCube hex( 3, 4 );

    HexLayout flat( HEX_ORIENTATION_FLAT, OFFSET_TYPE_ODD, Point( 10, 15 ), Point( 35, 71 ) );
    AssertEqual( PixelToHex( flat, HexToPixel( flat, hex ) ).Round( -1 ), hex , "HexLayout Flat" );
    AssertEqual( PixelToHex( flat, HexToPixel( flat, hex ) ).Round( 0 ), hex , "HexLayout Flat" );
    AssertEqual( PixelToHex( flat, HexToPixel( flat, hex ) ).Round( 1 ), hex , "HexLayout Flat" );
    AssertEqual( PixelToHex( flat, HexToPixel( flat, hex ) ).Round( 2 ), hex , "HexLayout Flat" );

    HexLayout pointy( HEX_ORIENTATION_POINTY, OFFSET_TYPE_ODD, Point( 10, 15 ), Point( 35, 71 ) );
    AssertEqual( PixelToHex( pointy, HexToPixel( pointy, hex ) ).Round( -1 ), hex , "HexLayout Pointy" );
    AssertEqual( PixelToHex( pointy, HexToPixel( pointy, hex ) ).Round( 0 ), hex , "HexLayout Pointy" );
    AssertEqual( PixelToHex( pointy, HexToPixel( pointy, hex ) ).Round( 1 ), hex , "HexLayout Pointy" );
    AssertEqual( PixelToHex( pointy, HexToPixel( pointy, hex ) ).Round( 2 ), hex , "HexLayout Pointy" );
}

void Test_OffsetCubeConversion() {
    HexCube a( 3, 4 );
    HexOffset b( 1, -3 );
    HexLayout even_q( HEX_ORIENTATION_FLAT, OFFSET_TYPE_EVEN, Point(10, 10), Point(0, 0) );
    HexLayout odd_q( HEX_ORIENTATION_FLAT, OFFSET_TYPE_ODD, Point(10, 10), Point(0, 0) );
    HexLayout even_r( HEX_ORIENTATION_POINTY, OFFSET_TYPE_EVEN, Point(10, 10), Point(0, 0) );
    HexLayout odd_r( HEX_ORIENTATION_POINTY, OFFSET_TYPE_ODD, Point(10, 10), Point(0, 0) );

    AssertEqual( Offset_to_Cube( even_q, Cube_to_Offset( even_q, a ) ), a, "CubeConversion even-q" );
    AssertEqual( Cube_to_Offset( even_q, Offset_to_Cube( even_q, b ) ), b, "OffsetConversion even-q" );

    AssertEqual( Offset_to_Cube( odd_q, Cube_to_Offset( odd_q, a ) ), a, "CubeConversion odd-q" );
    AssertEqual( Cube_to_Offset( odd_q, Offset_to_Cube( odd_q, b ) ), b, "OffsetConversion odd-q" );

    AssertEqual( Offset_to_Cube( even_r, Cube_to_Offset( even_r, a ) ), a, "CubeConversion even-r" );
    AssertEqual( Cube_to_Offset( even_r, Offset_to_Cube( even_r, b ) ), b, "OffsetConversion even-r" );

    AssertEqual( Offset_to_Cube( odd_r, Cube_to_Offset( odd_r, a ) ), a, "CubeConversion odd-r" );
    AssertEqual( Cube_to_Offset( odd_r, Offset_to_Cube( odd_r, b ) ), b, "OffsetConversion odd-r" );
}

void Test_CubeToOffset() {
    HexLayout even( HEX_ORIENTATION_FLAT, OFFSET_TYPE_EVEN, Point(10, 10), Point(0, 0) );
    AssertEqual( Cube_to_Offset( even, HexCube( 1, 2 ) ), HexOffset( 1, 3 ), "Cube_to_Offset even-q" );

    HexLayout odd( HEX_ORIENTATION_FLAT, OFFSET_TYPE_ODD, Point(10, 10), Point(0, 0) );
    AssertEqual( Cube_to_Offset( odd, HexCube( 1, 2 ) ), HexOffset( 1, 2 ), "Cube_to_Offset odd-q" );
}

void Test_OffsetToCube() {
    HexLayout even( HEX_ORIENTATION_FLAT, OFFSET_TYPE_EVEN, Point(10, 10), Point(0, 0) );
    AssertEqual( Offset_to_Cube( even, HexOffset( 1, 3 ) ), HexCube( 1, 2 ), "Offset_to_Cube even-q" );

    HexLayout odd( HEX_ORIENTATION_FLAT, OFFSET_TYPE_ODD, Point(10, 10), Point(0, 0) );
    AssertEqual( Offset_to_Cube( odd, HexOffset( 1, 2 ) ), HexCube( 1, 2 ), "Offset_to_Cube odd-q" );
}

int main() {
    TestRunner runner;
    runner.RunTest( Test_HexArithmetic, "Test_HexArithmetic" );
    runner.RunTest( Test_HexDirection, "Test_HexDirection" );
    runner.RunTest( Test_HexNeighbor, "Test_HexNeighbor" );
    runner.RunTest( Test_HexDiagonal, "Test_HexDiagonal" );
    runner.RunTest( Test_HexDistance, "Test_HexDistance" );
    runner.RunTest( Test_HexRotate, "Test_HexRotate" );
    runner.RunTest( Test_HexRound, "Test_HexRound" );
    runner.RunTest( Test_HexLine, "Test_HexLine" );
    runner.RunTest( Test_HexLayout, "Test_HexLayout" );
    runner.RunTest( Test_OffsetCubeConversion, "Test_OffsetCubeConversion" );
    runner.RunTest( Test_CubeToOffset, "Test_CubeToOffset" );
    runner.RunTest( Test_OffsetToCube, "Test_OffsetToCube" );

    return 0;
}
