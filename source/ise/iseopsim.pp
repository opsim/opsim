program iseopsim;

uses
  SysUtils,
  GL,
  DNA_listbase,
  GLPT,
  MEM_guardedalloc;

var
  window: pGLPTWindow;
  ratio: double;
  width: integer = 640;
  height: integer = 480;
  nbFrames: longint = 0;
  lastTime: double;
  rotate: double;

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
      title := Format('[FPS: %3.0f]', [nbFrames/(currentTime - lastTime)]);

      writeln(title);

      nbFrames := 0;
      lastTime := Now * 24 * 3600;
    end;
end;

procedure error_callback(const error: integer; const description: string);
begin
  writeln(stderr, description);
end;

procedure event_callback(event: pGLPT_MessageRec);
begin
  case event^.mcode of

    GLPT_MESSAGE_KEYPRESS:
    begin
      writeln(event^.params.keyboard.keycode);

      if event^.params.keyboard.keycode = GLPT_KEY_ESCAPE then
        GLPT_SetWindowShouldClose(event^.win, True);
    end;

    GLPT_MESSAGE_MOUSEDOWN:
      writeln(event^.params.mouse.buttons);
  end;
end;

begin
  //GLPT_SetErrorCallback(@error_callback);

  if not GLPT_Init then
    halt(-1);

  window := GLPT_CreateWindow(GLPT_WINDOW_POS_CENTER, GLPT_WINDOW_POS_CENTER, width, height, 'OpSim', GLPT_GetDefaultContext);
  if window = nil then
  begin
    GLPT_Terminate;
    halt(-1);
  end;

  ratio := width / height;

  window^.event_callback := @event_callback;

  writeln('GLPT version: ', GLPT_GetVersionString);
  writeln('OpenGL version: ', glGetString(GL_VERSION));

  while not GLPT_WindowShouldClose(window) do
    begin
      setWindowFPS;

      glViewport(0, 0, width, height);
      glClear(GL_COLOR_BUFFER_BIT);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity;
      glOrtho(-ratio, ratio, -1, 1, 1, -1);
      glMatrixMode(GL_MODELVIEW);

      glLoadIdentity;
      rotate := (Now * 24 * 3600 * 50);
      rotate := rotate - int(rotate / 360) * 360;
      glRotatef(rotate, 0, 0, 1);

      glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-0.6, -0.4, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0.6, -0.4, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0.6, 0);
      glEnd;

      GLPT_SwapBuffers(window);
      GLPT_PollEvents;
    end;

  GLPT_DestroyWindow(window);

  GLPT_Terminate;
end.
