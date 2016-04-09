unit Simplex;

{$mode objfpc}{$H+}

interface

uses
  util;

const
  NP = 20;   {Maximum value for NDIM=20}
  MP = NP + 1;

type
  MAT = array[1..MP, 1..NP] of double;
  VEC = array[1..MP] of double;
  solvfunc = function(P: VEC): real;

procedure AMOEBA(var P: MAT; var Y: VEC; MP, NP, NDIM: integer; FTOL: double;
var ITER: integer; func: solvfunc);

implementation

{**********************************************************
*   Multidimensional minimization of a function FUNC(X)   *
*  where X is an NDIM-dimensional vector, by the downhill *
*  simplex method of Nelder and Mead.                     *
* ------------------------------------------------------- *
* SAMPLE RUN: Find a minimum of function F(x,y):          *
*             F=Sin(R)/R, where R = Sqrt(x*x+y*y).        *
*                                                         *
*  Number of iterations: 22                               *
*                                                         *
*  Best NDIM+1 points:                                    *
*   4.12268589437008E+0000  1.78691533207893E+0000        *
*   4.16647735983133E+0000  1.68269817531109E+0000        *
*   4.14245440904051E+0000  1.74117587320507E+0000        *
*                                                         *
*  Best NDIM+1 mimimum values:                            *
*  -2.17233626514560E-0001                                *
*  -2.17233628107148E-0001                                *
*  -2.17233627137833E-0001                                *
*                                                         *
* ------------------------------------------------------- *
* REFERENCE: "Numerical Recipes, The Art of Scientific    *
*             Computing By W.H. Press, B.P. Flannery,     *
*             S.A. Teukolsky and W.T. Vetterling,         *
*             Cambridge University Press, 1986"           *
*             [BIBLI 08].                                 *
*                                                         *
*                      TPW Release By J-P Moreau, Paris.  *
**********************************************************}
procedure AMOEBA(var P: MAT; var Y: VEC; MP, NP, NDIM: integer; FTOL: double;
var ITER: integer; func: solvfunc);
{-------------------------------------------------------------------
! Multidimensional minimization of the function FUNC(X) where X is
! an NDIM-dimensional vector, by the downhill simplex method of
! Nelder and Mead. Input is a matrix P whose NDIM+1 rows are NDIM-
! dimensional vectors which are the vertices of the starting simplex
! (Logical dimensions of P are P(NDIM+1,NDIM); physical dimensions
! are input as P(NP,NP)). Also input is the vector Y of length NDIM
! +1, whose components must be pre-initialized to the values of FUNC
! evaluated at the NDIM+1 vertices (rows) of P; and FTOL the fractio-
! nal convergence tolerance to be achieved in the function value. On
! output, P and Y will have been reset to NDIM+1 new points all within
! FTOL of a minimum function value, and ITER gives the number of ite-
! rations taken.
!-------------------------------------------------------------------}
label
  1, 10;
const
  NMAX = 20;
  ALPHA = 1.0;
  BETA = 0.5;
  GAMMA = 2.0;
  ITMAX = 500;
{ Expected maximum number of dimensions, three parameters which define
the expansions and contractions, and maximum allowed number of
iterations. }
var
  PR, PRR, PBAR: VEC;
  I, IHI, ILO, INHI, J, MPTS: integer;
  RTOL, YPR, YPRR: double;
begin
  MPTS := NDIM + 1;
  ITER := 0;
  1:
    ILO := 1;
  if Y[1] > Y[2] then
  begin
    IHI := 1;
    INHI := 2;
  end
  else
  begin
    IHI := 2;
    INHI := 1;
  end;
  for I := 1 to MPTS do
  begin
    if Y[I] < Y[ILO] then
      ILO := I;
    if Y[I] > Y[IHI] then
    begin
      INHI := IHI;
      IHI := I;
    end
    else if Y[I] > Y[INHI] then
      if I <> IHI then
        INHI := I;
  end;

{ Compute the fractional range from highest to lowest and return if
satisfactory. }
  //DB: add ftol to denominator to prevent divisio by zero
  RTOL := 2.0 * ABS(Y[IHI] - Y[ILO]) / (ABS(Y[IHI]) + ABS(Y[ILO] + FTOL));
  if RTOL < FTOL then
    goto 10;  {normal return}
  if ITER = ITMAX then
  begin
    Writeln(' Amoeba exceeding maximum iterations.');
    ReadLn;
    goto 10;  {return}
  end;
  ITER := ITER + 1;
  for J := 1 to NDIM do
    PBAR[J] := 0.0;
  for I := 1 to MPTS do
    if I <> IHI then
      for J := 1 to NDIM do
        PBAR[J] := PBAR[J] + P[I, J];
  for J := 1 to NDIM do
  begin
    PBAR[J] := PBAR[J] / (1.0 * NDIM);
    PR[J] := (1.0 + ALPHA) * PBAR[J] - ALPHA * P[IHI, J];
  end;
  YPR := FUNC(PR);
  if YPR <= Y[ILO] then
  begin
    for J := 1 to NDIM do
      PRR[J] := GAMMA * PR[J] + (1.0 - GAMMA) * PBAR[J];
    YPRR := FUNC(PRR);
    if YPRR < Y[ILO] then
    begin
      for J := 1 to NDIM do
        P[IHI, J] := PRR[J];
      Y[IHI] := YPRR;
    end
    else
    begin
      for J := 1 to NDIM do
        P[IHI, J] := PR[J];
      Y[IHI] := YPR;
    end;
  end
  else if YPR >= Y[INHI] then
  begin
    if YPR < Y[IHI] then
    begin
      for J := 1 to NDIM do
        P[IHI, J] := PR[J];
      Y[IHI] := YPR;
    end;
    for J := 1 to NDIM do
      PRR[J] := BETA * P[IHI, J] + (1.0 - BETA) * PBAR[J];
    YPRR := FUNC(PRR);
    if YPRR < Y[IHI] then
    begin
      for J := 1 to NDIM do
        P[IHI, J] := PRR[J];
      Y[IHI] := YPRR;
    end
    else
      for I := 1 to MPTS do
        if I <> ILO then
        begin
          for J := 1 to NDIM do
          begin
            PR[J] := 0.5 * (P[I, J] + P[ILO, J]);
            P[I, J] := PR[J];
          end;
          Y[I] := FUNC(PR);
        end;
  end
  else
  begin
    for J := 1 to NDIM do
      P[IHI, J] := PR[J];
    Y[IHI] := YPR;
  end;
  goto 1;
  10: ;
end;

end.
