unit pars7glb;
{$O+,F+}
interface
uses realtype;
type rpointer=^float;
     onestep=procedure;
     operationpointer=^operation;
     operation=record
             arg1,arg2:rpointer;
             dest:rpointer;
             next:operationpointer;
             op:onestep;
             opnum:word;
            end;
var randomsize,randomresult:word;
    randomiterates,contrand:boolean;
    contrandresult:float;
implementation

end.