unit Flash;
{3 Phase Flash Routine Based on Alogorithms presented in
"Rapid Phase Determination in Multiple-Phase Flash Calculations"
by P. A. Nelson.  Comput. chem. Engng, Vol 11, No 6, pp. 581-591,1987}
{Copyright 2000 by Matt Henley Released under the GNU General License (GPL) V2 }
interface
uses Math, SimBaseTypes, SRK;

type
  TFlash = class (TObject)
    CompList: CompIntArray;
    EOS: TEOS;
    NumComp: Integer;
    WhichImmisc: Integer;
    function DoesPhaseExist(var K1, K2, Z:comparray; var phi1,phi2: Double): 
            ExistArray;
    function dTwoPhaseSum(var K,FeedMolFractionofComponent:comparray;L:Double;
            NComp:Integer): Double;
    function GetdQ1dphi1(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
    function GetdQ1dphi2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
    function GetdQ2dphi2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
    function GetQ1(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
    function GetQ2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
    function GetTwoPhaseL(var K,Z:comparray): Double;
    procedure normalize(var MyArray: comparray);
    procedure SetEOS(Choice:integer);
    procedure Solvephi1phi2(var phi1,phi2: Double; var K1,K2,Z:CompArray);
    function SolveQ1(var K1,K2,Z:Comparray; phi2:Double): Double;
    function SolveQ1minusQ2(var K1,K2,Z:Comparray): Double;
    function SolveQ2(var K1,K2,Z:Comparray; phi1:Double): Double;
    procedure TP(var Feed: TStrm; t,p:Double);
    function TwoPhaseSum(var K,Z:comparray;L:Double;NComp:Integer): Double;
  end;
  

implementation

{
************************************ TFlash ************************************
}
function TFlash.DoesPhaseExist(var K1, K2, Z:comparray; var phi1,phi2: Double): 
        ExistArray;
var
  i: Integer;
  T1, T2, T3, T4, T5, T6, T7, T8, T9: Double;
  Otherphi1: Double;
  Exist: ExistArray;
  
  {Given K1,K2 and Z, this function detemines the existence of the different phases
   based on rules on page 585 of nelson paper.  While it is a Function that returns an array showing
   the existing phases, it also returns phi values for phases that exists, which appear to be subsequently used}
  
begin
  Exist[1]:=False; // Initialize Phase Existence array to no phases
  Exist[2]:=False; //
  Exist[3]:=False; //
  T1:=0.0;    // Equation 9a from Nelson
  T2:=0.0;    // Equation 9b from Nelson
  T3:=0.0;    // Equation 9c from Nelson
  T4:=0.0;    // Equation 9d from Nelson
  T5:=0.0;    // Equation 9e from Nelson
  T6:=0.0;    // Equation 9f from Nelson
  {Sorry I know these have crappy names but they are unnamed in article}
  for i:= 1 to NumComp do  {Sum Each}
  begin
    T1:=T1+Z[i]/K1[i];
    T2:=T2+Z[i]*K1[i];
    T3:=T3+Z[i]/K2[i];
    T4:=T4+Z[i]*K2[i];
    T5:=T5+Z[i]*K1[i]/K2[i];
    T6:=T6+Z[i]*K2[i]/K1[i];
  end;
  { First Check for only Single Phases, Do the easy one first         }
  if  ( (T1<1.0) and (T3<1.0) ) then       { Vapor Phase Only         }
      Exist[1] := True
  else if ( (T2<1.0) and (T5<1.0) ) then   { Liquid 1 Phase Only      }
      Exist[2] := True
  else if ( (T4<1.0) and (T6<1.0)) then    { Liquid 2 Phase Only      }
      Exist[3] := True
  else
  {Check Two Phase Scenarios, See Fig 3 in paper for graphical explanation }
      begin
        {Check boundary conditions}
        {Find Q2(phi1,0) where phi1 is the root of Q1(phi1,0)=0}
        phi1 := SolveQ1(K1,K2,Z,0.0);
        T7 := GetQ2(K1,K2,Z,phi1,0.0);
        {Find Q1(0,phi2) where phi2 is the root of Q2(0,phi2)=0)}
        phi2 := SolveQ2(K1,K2,Z,0.0);
        T8 := GetQ1(K1,K2,Z,0.0,phi2);
        {Find Q1(phi1,1-phi1) at the root of Q1(phi1,1-phi1)-Q2(phi1,1-phi1)=0}
        OtherPhi1:=SolveQ1minusQ2(K1,K2,Z);
        T9 := GetQ1(K1,K2,Z,Otherphi1,1.0-Otherphi1);
        if ( (T7 < 0.0) and (T1>1.0) and (T2>1.0) ) then
          begin                            { Vap and Liq1 Only        }
            Exist[1]:=True;
            Exist[2]:=True;
            phi2:=0.0
          end
        else if ( (T8 < 0.0) and (T1>1.0) and (T2>1.0) ) then
          begin                            { Vap and Liq2 Only        }
            Exist[1]:=True;
            Exist[3]:=True;
            phi1:=0.0
          end
        else if ( (T9 > 0.0) and (T5>1.0) and (T6>1.0) ) then
          begin                            { Liq1 and Liq2 Only       }
            Exist[2]:=True;
            Exist[3]:=True;
          end
        else
          begin
            Exist[1]:=True;                { Three Phase System       }
            Exist[2]:=True;
            Exist[3]:=True;
          end;
      end;
  DoesPhaseExist:=Exist;
  
end;

function TFlash.dTwoPhaseSum(var K,FeedMolFractionofComponent:comparray;
        L:Double;NComp:Integer): Double;
var
  i: Integer;
  thesum: Double;
  
  {Still thinking about these two.. two phase flash equations for solution with Newton-Raphson}
  
begin
  thesum := 0.0;
  for i := 1 to Ncomp do
    thesum:=thesum-(Sqr(1.0-K[i])*Z[i]/Sqr((L+(1-L)*K[i])));
  dTwoPhaseSum:=thesum;
end;

function TFlash.GetdQ1dphi1(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
var
  i: Integer;
  dQ: Double;
  
  {Partial derivative function of Q1 with respect to phi1 are below. Defined in EQ 10a
  
  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}
  
begin
  dQ:=0.0;
  for i:=1 to NumComp do
    begin
      dQ:= dQ  - Z[i]*Sqr(K2[i])*Sqr((1.0-K1[i]))/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetdQ1dphi1:=dQ;
end;

function TFlash.GetdQ1dphi2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
var
  i: Integer;
  dQ: Double;
  
  {Partial derivative function of Q1 with respect to phi2 are below. Defined in EQ 10b
  Same as dQ2/dphi1
  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}
  
begin
  dQ:=0.0;
  for i:=1 to NumComp do
    begin
      dQ:= dQ  - Z[i]*K1[i]*K2[i]*(1.0-K1[i])*(1.0-K2[i])/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetdQ1dphi2:=dQ;
end;

function TFlash.GetdQ2dphi2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
var
  i: Integer;
  dQ: Double;
  
  {Partial derivative function of Q2 with respect to phi2 are below. Defined in EQ 10c
  
  Needed for the Newton-Raphson solutions for both 2 and 3 phase cases}
  
begin
  dQ:=0.0;
  for i:=1 to NumComp do
    begin
      dQ:= dQ  - Z[i]*Sqr(K1[i])*Sqr((1.0-K2[i]))/
           Sqr(K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetdQ2dphi2:=dQ;
end;

function TFlash.GetQ1(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
var
  i: Integer;
  Q1: Double;
  
  {An implemenation of EQ 7a.  Partial derivative functions are below.
  
  Essentially this is a mass balance inserted into the component balance and then
  played around with to produce a form that solves better under Newton-Raphson
  No variables in the call list are changed.. only a number is returned}
  
  {EQ 7a}
  
begin
  Q1:=0.0;
  for i:=1 to NumComp do
    begin
      Q1:= Q1  + Z[i]*K2[i]*(1.0-K1[i])/
                 (K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetQ1:=Q1;
end;

function TFlash.GetQ2(var K1,K2,Z:Comparray; phi1,phi2:Double): Double;
var
  i: Integer;
  Q2: Double;
  
  {An implemenation of EQ 7b.  Partial derivative functions are below.
  
  Essentially this is a mass balance inserted into the component balance and then
  played around with to produce a form that solves better under Newton-Raphson
  No variables in the call list are changed.. only a number is returned}
  {EQ 7b}
  
begin
  Q2:=0.0;
  for i:=1 to NumComp do
    begin
      Q2:= Q2  + Z[i]*K1[i]*(1.0-K2[i])/
                 (K1[i]*K2[i]+phi1*K2[i]*(1-K1[i])+phi2*K1[i]*(1-K2[i]));
    end;
  GetQ2:=Q2;
end;

function TFlash.GetTwoPhaseL(var K,Z:comparray): Double;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;
  
  {Assuming two phases (Vapor/Liquid) and given feed composition
   and K's, solve for the Liquid Fraction using using Newton-Raphson
   Does not update K or Z.. just L                                   }
  {Function and Derivative return values}
  
begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= TwoPhaseSum(K,Z,OldL,NumComp);
    dFdL := dTwoPhaseSum(K,Z,OldL,NumComp);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  GetTwoPhaseL:=L;
end;

procedure TFlash.normalize(var MyArray: comparray);
var
  i: Integer;
  thesum: Double;
  
  // Normalize an array of mol fractions so that it adds up to 1.0
  
begin
  thesum:=0.0;
  for i:=1 to numcomp do thesum:=thesum+MyArray[i];
  thesum:=1.0/thesum;
  for i:=1 to numcomp do MyArray[i]:=thesum*MyArray[i];
end;

procedure TFlash.SetEOS(Choice:integer);
var
  SRK: TSRK;
  
  {This is code left over from how i did things..i assume it goes away.. }
  
begin
  SRK := TSRK.Create;
  SRK.InitComponents(CompList,NumComp);
  EOS := SRK;
  WhichImmisc := 3;
end;

procedure TFlash.Solvephi1phi2(var phi1,phi2: Double; var K1,K2,Z:CompArray);
var
  i: Integer;
  Q1, Q2: Double;
  Oldphi1, Oldphi2: Double;
  D, M11, M12, M21, M22: Double;
  Converged: Boolean;
  
  { Solve for phi1 and phi2 using Newton-Raphson method.  Eq 11 in Nelson Paper
    This is the solution method when we have determined that three phases exist.
    It returns the liquid fractions of each of the two liquid phases}
  
begin
  Converged:=False;
  If (phi1=0.0) then phi1:=0.5;    {Dont start with 0.0}
  If (phi2=0.0) then phi2:=0.5;
  Q1:= GetQ1(K1,K2,Z,phi1,phi2);
  Q2:= GetQ2(K1,K2,Z,phi1,phi2);
  While (not Converged) do
  begin
    Oldphi1:=phi1;                 {Need previous run for derivative}
    Oldphi2:=phi2;                 {}
    M11:= GetdQ2dphi2(K1,K2,Z,Oldphi1,Oldphi2); {Get partial derivative of Q2 with repect to phi2}
    M12:= -1.0 * GetdQ1dphi2(K1,K2,Z,Oldphi1,Oldphi2); {Take the negative of the partial derivative of Q1 with respect to phi2 }
    M21:= M12;                                   {As per EQ 10b is equivalent to M12}
    M22:= GetdQ1dphi1(K1,K2,Z,Oldphi1,Oldphi2);  { Get partial derivative of Q1 with respect to phi1}
    D := M22*M11 - M12*M21;                      {Calculate the Determinent.  Negatives Cancel on M12*M21}
    phi1:=Oldphi1-(1.0/D)*(M11*Q1+M12*Q2);       {Calculate the next iternation of phi1}
    phi2:=Oldphi2-(1.0/D)*(M21*Q1+M22*Q2);       {Calculate the next iteration of phi2}
    Q1:= GetQ1(K1,K2,Z,phi1,phi2);               {Do it all again}
    Q2:= GetQ2(K1,K2,Z,phi1,phi2);
    If ((Abs(Q1)<0.001) and (Abs(Q2)<0.001)) then Converged := True; // Repeat until Q1 and Q2 are 0.0
  end;
  
  
end;

function TFlash.SolveQ1(var K1,K2,Z:Comparray; phi2:Double): Double;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;
  
  {Given a set of K values for both Liquid Phases and the Feed mol Fractions of the Feed
  Solve Eq 7a for phi1 at fixed phi2 using Newton-Raphson
  -- so despite its name, it returns a Liquid fraction of Liquid phase 1 at a given
  liquid phase 2 fraction
  
  }
  {Function and derivative of Function, in this case Q1()}
  
begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;           {Limit the number of iterations}
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= GetQ1(K1,K2,Z,L,phi2);
    dFdL := GetdQ1dphi1(K1,K2,Z,L,phi2);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  SolveQ1:=L;
end;

function TFlash.SolveQ1minusQ2(var K1,K2,Z:Comparray): Double;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;
  
  {This is the third boundary condition.  Shown graphically as the diagonal line
  on Fig 2.  Here phi2=1-phi1 .  We are solving for the root of Q1(phi1,1-phi1)-
  Q2(phi1,1-phi1)=0 which returns phi1 along the diagonal edge}
  
begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= GetQ1(K1,K2,Z,L,1.0-L)-GetQ2(K1,K2,Z,L,1.0-L);
    dFdL := GetdQ1dphi1(K1,K2,Z,L,1.0-L)-GetdQ1dphi2(K1,K2,Z,L,1.0-L);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  SolveQ1minusQ2:=L;
end;

function TFlash.SolveQ2(var K1,K2,Z:Comparray; phi1:Double): Double;
var
  Err: Double;
  L, OldL: Double;
  iter: Integer;
  F, dFdL: Double;
  
  {Given a set of K values for both Liquid Phases and the Feed mol Fractions of the Feed
  Solve Eq 7b for phi1 at fixed phi1 using Newton-Raphson
  -- so despite its name, it returns a Liquid fraction of Liquid phase 2 at a given
  liquid phase 1 fraction
  
  }
  
begin
  L:=0.5;
  Err:=10000.0;
  iter:=0;
  While ((Abs(Err) > 0.00005) and (iter<100)) do
   begin
    OldL:= L;
    F:= GetQ2(K1,K2,Z,phi1,L);
    dFdL := GetdQ2dphi2(K1,K2,Z,phi1,L);
    L := OldL - F/dFdL;
    Err := L/OldL -1;
    iter:=iter+1;
   end;
  SolveQ2:=L;
end;

procedure TFlash.TP(var Feed: TStrm; t,p:Double);
var
  i, j, numphase: Integer;
  Z, Y, X1, X2: CompArray;
  temp, L: Double;
  phi1, phi2: Double;
  FlowV, FlowL1, FlowL2: Double;
  K1, K2: CompArray;
  ZVap, ZLiq1, ZLiq2: Double;
  OldK1, OldK2: CompArray;
  OldY, OldX1, OldX2: CompArray;
  PhaseExist: ExistArray;
  Converged: Boolean;
  iter: Integer;
  Err: Double;
  
  { This is the actual coded flash routine.  You give it a stream and t and p and it returns a flashed stream
  }
  //  Absolute error
  
begin
  Converged := False;              // We are not converged
  Z:=Feed.Comp;                    // Extract the overall composition mol fractions to a working variable
  K1:=EOS.EstK1Values(t,p);        // Estimate K1 Values Via wilson equation
  L:= GetTwoPhaseL(K1,Z);          // From the K values and the overall mol fractions Calculate a Liquid Fraction L
  for i:= 1 to NumComp do X1[i]:= Z[i]/(L+(1-L)*K1[i]); // From L,K1 and Z Calculate mol fractions in liquid phase1
  for i:= 1 to NumComp do Y[i]:= K1[i]*Z[i]/(L+(1-L)*K1[i]); // from L,K1 and Z Calculate mol fractions Gas phase
  OldK1:=K1;
  K1:=EOS.GetK(OldK1,Y,X1,ZVap,ZLiq1,t,p); // Use this Info to do a real flash calculation and get a better K1
  { Create a preliminary guess for liquid phase 2 by taking the predominate Liquid 2 Component (usually water)
     and putting it in a phase.  Add a tiny bit of the other components }
  temp := 0.0;
  for i:= 1 to NumComp do
    begin
      If (i <> WhichImmisc) then   {If not the main immicible component, just add a token amount so they are not 0}
        begin                      {This is arbitrary.. it could also use the Kerosene solubility thing that people do to cheat}
          X2[i]:= Z[i]*0.001;
          temp:=temp+X2[i];
        end;
    end;
  X2[WhichImmisc]:=100.0 - temp;   {Make the rest of that phase the immiscible component}
  for i:= 1 to NumComp do OldK2[i]:=Y[i]/X2[i]; {Back calculate some K values from what we just did}
  K2:=EOS.GetK(OldK2,Y,X2,ZVap,Zliq2,t,p);     {Now actually run a flash on this composition}
  iter:=1;
  Err:=10000;        // Set high error for first iteration
  
  {This is the main solution Loop of this flash}
  While ( (abs(Err) > 0.01) and (Iter < 100) ) do {Don't let it keep going even if it fails}
  begin
   OldY:=Y;
   OldX1:=X1;
   OldX2:=X2;
   {Determine which phases exist based on the current K values, also calculate phi's }
   PhaseExist:=DoesPhaseExist(K1,K2,Z,phi1,phi2);
   numphase:=0;
   for i:= 1 to 3 do if PhaseExist[i] then numphase:=numphase+1; {Add up the number of phases.. dont rememeber why}
  
   if (numphase=1) then {If we only have one phase, stick it all in the appropriate phase,
                         because we should know if this phase is Gas, Liq1 or Liq2}
     begin
       if PhaseExist[1] then Feed.Vap.Comp:=Z
       else if PhaseExist[2] then Feed.Liq1.Comp:=Z
       else Feed.Liq2.Comp:=Z;
     end
   else if (numphase=2) then   {If 2 phases we still need to keep around info on the other one..
                                just in case another phase shows up during convergence... but
                                we do need to make sure things add up so we normalize}
     begin
       FlowL1:=phi1*Feed.Flow;
       FlowL2:=phi2*Feed.Flow;
       FlowV:= Feed.Flow - FlowL1 - FlowL2;
       for i:= 1 to numcomp do
         begin
           Y[i] :=Z[i]*Feed.Flow*K1[i]*K2[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
           X1[i]:=Z[i]*Feed.Flow*K2[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
           X2[i]:=Z[i]*Feed.Flow*K1[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
         end;
         normalize(Y);  {Normalize vapor fractions}
         normalize(X1); {Normalize Liq1 fractions}
         normalize(X2); {Normalize Liq2 fractions}
  
     end
   else  {Three Phases present}
     begin
       Solvephi1phi2(phi1,phi2,K1,K2,Z);  {Calculate Fraction of total Flow in each Liquid phase}
       FlowL1:=phi1*Feed.Flow;
       FlowL2:=phi2*Feed.Flow;
       FlowV:= Feed.Flow - FlowL1 - FlowL2;
       for i:= 1 to numcomp do
         begin
           Y[i] :=Z[i]*Feed.Flow*K1[i]*K2[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
           X1[i]:=Z[i]*Feed.Flow*K2[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
           X2[i]:=Z[i]*Feed.Flow*K1[i]/(FlowV*K1[i]*K2[i]+FlowL1*K2[i]+FlowL2*K1[i]);
         end;
       normalize(Y);  {Normalize each of the component fractions so they add up to 1.0}
       normalize(X1);
       normalize(X2);
  
     end;
   Err := 0.0;
   OldK1 := K1; {Save the last Kvalue to use as a starting point and speed the solution}
   OldK2 := K2;
   K1:=EOS.GetK(OldK1,Y,X1,ZVap,ZLiq1,t,p);  {Generate new K1 values for the next iteration}
   K2:=EOS.GetK(OldK2,Y,X2,ZVap,ZLiq2,t,p);  {Generate new K2 values for the next iteration}
   {Sum the overall errors as defined as change from the previous iteration}
   for i:=1 to NumComp  do Err:=Err + abs(Y[i]/OldY[i]-1.0);
   for i:=1 to NumComp  do Err:=Err + abs(X1[i]/OldX1[i]-1.0);
   for i:=1 to NumComp  do Err:=Err + abs(X2[i]/OldX2[i]-1.0);
   iter := iter + 1;
  end;
  {Convereged!}
  Feed.t:= t;  {Set the t and p to the specs in the Flash.TP call}
  Feed.p:= p;
                                {Put stuff into the appropriate place}
  If PhaseExist[1] then         {Vapor Phase         }
  begin
    Feed.Vap.Comp := Y;
    Feed.Vap.Flow := FlowV;
    Feed.Vap.Exist:= True;
    Feed.Vap.Z:= ZVap;
  end
  else Feed.Vap.clearphase;     {If nothing there.. zero it out}
  
  If PhaseExist[2] then         {Liquid 1 (HC) Phase }
  begin
    Feed.Liq1.Comp := X1;
    Feed.Liq1.Flow := FlowL1;
    Feed.Liq1.Exist:= True;
    Feed.Liq1.Z:=ZLiq1;
  end
  else Feed.Liq1.clearphase;
  
  If PhaseExist[3] then         {Liquid 2 (AQ) Phase }
  begin
    Feed.Liq2.Comp := X2;
    Feed.Liq2.Flow := FlowL2;
    Feed.Liq2.Exist:= True;
    Feed.Liq2.Z:= ZLiq2;
  end
  else Feed.Liq2.clearphase;
  
  
end;

function TFlash.TwoPhaseSum(var K,Z:comparray;L:Double;NComp:Integer): Double;
var
  i: Integer;
  thesum: Double;
  
  {Still thinking about these two.. part of two phase flash equations for solution with Newton-Raphson}
  
begin
  thesum := 0.0;
  for i := 1 to Ncomp do
    thesum:=thesum+(1.0-K[i])*Z[i]/(L+(1-L)*K[i]);
  TwoPhaseSum:=thesum;
  
end;




end.




