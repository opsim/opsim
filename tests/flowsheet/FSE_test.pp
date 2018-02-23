program FSE_test;

uses
  SysUtils,
  GL,
  MEM_guardedalloc,
  ANT_main,
  ANT_types,
  ANT_messages,
  FSE_main;

type
  MainType = record
    fseobs: ListBase;
  end;

var
  main: MainType;

  window: pANTwindow;
  width: integer = 640;
  height: integer = 480;
  nbFrames: longint = 0;
  lastTime: double;

  procedure setWindowFPS;
  var
    currentTime: double;
    title: string;
  begin
    // measure FPS
    currentTime := Now * 24 * 3600;
    inc(nbFrames);

    if currentTime - lastTime >= 1 then
    begin
      title := format('[FPS: %3.0f]', [nbFrames / (currentTime - lastTime)]);

      writeln(title);

      nbFrames := 0;
      lastTime := Now * 24 * 3600;
    end;
  end;

  procedure error_callback(error: integer; const description: string);
  begin
    writeln(stderr, description);
  end;

  procedure event_callback(msg: pointer);

  var
    event: pANT_MessageRec;
  begin
    event := msg;

    case event^.mcode of
      ANT_MESSAGE_KEYPRESS:
      begin
        if event^.params.keyboard.keychar = ANT_KEY_ESCAPE then
          ANT_SetWindowShouldClose(event^.win, True);
      end;
      ANT_MESSAGE_MOUSEDOWN:
      begin
        writeln(event^.params.mouse.buttons);
      end;
      ANT_MESSAGE_RESIZE:
      begin
        glViewport(0, 0, event^.params.rect.width, event^.params.rect.height);
      end;
    end;
  end;

  procedure setup_FSE_objects(lb: pListBase);
  var
    ob: pFSEobj;
  begin
    //create object to be drawn
    ob := FSE_object_new(lb);

    //add primitives and ports
    FSE_primitive_line_new(ob, 25, 0, 50, 25);
    FSE_primitive_line_new(ob, 25, 50, 50, 25);
    FSE_primitive_arc_new(ob, 0, 0, 25, 0, 360);
    //FSE_port_new(ob, 'port', 0, 0, FSE_PORT_INPUT);

    //position of FSE object
    ob^.x := 100;
    ob^.y := 200;
  end;

  procedure set_ortho_projection(width, height: cardinal);
  begin
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, width, 0.0, height, -1000.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
  end;

procedure InitGL;
begin
  glShadeModel(GL_SMOOTH);
  glClearColor(0, 0, 0, 0.5);
  glClearDepth(1);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
end;

begin
  ANT_SetErrorCallback(@error_callback);

  if not ANT_Init then
    halt(-1);

  window := ANT_CreateWindow(width, height, 'FlowSheet Editor example application');
  if window = nil then
  begin
    ANT_Terminate;
    halt(-1);
  end;

  InitGL;

  ANT_SetEventCallback(@event_callback);

  writeln('OpenGL version: ', glGetString(GL_VERSION));

  setup_FSE_objects(@Main.fseobs);

  while not ANT_WindowShouldClose(window) do
  begin
    setWindowFPS;

    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    set_ortho_projection(width, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity;

    FSE_draw(Main.fseobs);

    ANT_SwapBuffers(window);
    ANT_PollEvents;
  end;

  ANT_DestroyWindow(window);

  ANT_Terminate;
end.
