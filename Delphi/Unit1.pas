unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs;

type
  TForm1 = class(TForm)
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var
  SL: TStringList;
  Temp: string;
  i: Integer;
begin
  SL := TStringList.Create;
  SL.Delimiter := '|';
  SL.QuoteChar := '"';
  SL.DelimitedText := 'O=0 | ! .!=!.!';
  Temp := 'Count: ' + IntToStr(SL.Count) + #13;
  for i := 0 to SL.Count - 1 do
    Temp := Temp + Format('Name: %s Value: %s'#13,
              [SL.Names[i], SL.ValueFromIndex[i]]);
  ShowMessage(Temp);
  // ShowMessage(SL.Values['O']);
end;

end.
