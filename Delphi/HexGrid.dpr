program HexGrid;

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  HexGridUnit in 'HexGridUnit.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.

