
program simple;

uses
  SysUtils,
  GL,
  DNA_listbase,
  ANT_main, ANT_types, ANT_messages;

var 
  window: pANTWindow;
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
      title := format('[FPS: %3.0f]', [nbFrames/(currentTime - lastTime)]);

      writeln(title);

      nbFrames := 0;
      lastTime := Now * 24 * 3600;
    end;
end;

procedure error_callback(error             : integer;
                         const description : string);
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
                            writeln(event^.params.keyboard.keychar);

                            if event^.params.keyboard.keychar = ANT_KEY_ESCAPE then
                              ANT_SetWindowShouldClose(event^.win, True);
                          end;
    ANT_MESSAGE_MOUSEDOWN:
                           begin
                             writeln(event^.params.mouse.buttons);
                           end;
  end;
end;

begin
  ANT_SetErrorCallback(@error_callback);

  if not ANT_Init then
    halt(-1);

  window := ANT_CreateWindow(width, height, 'Simple example');
  if window = nil then
    begin
      ANT_Terminate;
      halt(-1);
    end;

  ratio := width / height;

  ANT_SetEventCallback(@event_callback);

  writeln('OpenGL version: ', glGetString(GL_VERSION));

  while not ANT_WindowShouldClose(window) do
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

      ANT_SwapBuffers(window);
      ANT_PollEvents;
    end;

  ANT_DestroyWindow(window);

  ANT_Terminate;
end.
