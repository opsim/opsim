unit DNA_listbase;

interface

type
  pLink = ^Link;

  { generic - all records which are put into linked lists begin with this }
  Link = record
    next: ^Link;
    prev: ^Link;
  end;

  { never change the size of this! genfile.c detects pointerlen with it  }
  pListBase = ^ListBase;

  ListBase = record
    first: pointer;
    last: pointer;
  end;

implementation

end.