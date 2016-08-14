program glfw3_test;

{$mode objfpc}{$H-}

uses
  SysUtils,
  GL, glfw3;

var
  window: pGLFWwindow;
  ratio: double;
  width, height: integer;
  nbFrames: longint = 0;
  lastTime: double;

  procedure setWindowFPS(win: PGLFWwindow);
  var
    currentTime: double;
    title: string;
    p: pchar;
  begin
    // Measure speed
    currentTime := glfwGetTime;
    inc(nbFrames);

    if currentTime - lastTime >= 1 then
    begin
      title := format('GLFW example - [FPS: %3.0f]', [nbFrames/(currentTime - lastTime)]);

      p := StrAlloc(length(title) + 1);
      strpcopy(p, title);
      glfwSetWindowTitle(win, p);
      StrDispose(p);

      nbFrames := 0;
      lastTime := glfwGetTime;
    end;
  end;

  procedure error_callback(error       : integer;
                           description : PChar); cdecl;
  begin
    writeln(stderr, description);
  end;

  procedure key_callback(window                      : pGLFWwindow;
                         key, scancode, action, mods : integer); cdecl;
  begin
    if (key = GLFW_KEY_ESCAPE) and (action = GLFW_PRESS) then
      glfwSetWindowShouldClose(window, GL_TRUE);
  end;

begin
  glfwSetErrorCallback(@error_callback);

  if glfwInit = 0 then
    halt(-1);

  window := glfwCreateWindow(640, 480, 'Simple example', nil, nil);
  if window = nil then
  begin
    glfwTerminate;
    halt(-1);
  end;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetKeyCallback(window, @key_callback);

  writeln('GLFW wrapper version ', GLFW_VERSION_MAJOR, '.', GLFW_VERSION_MINOR, '.', GLFW_VERSION_REVISION);
  writeln('GLFW library version ', glfwGetVersionString);
  writeln('OpenGL version       ', glGetString(GL_VERSION));

  while glfwWindowShouldClose(window) = 0 do
  begin
    setWindowFPS(window);

    glfwGetFramebufferSize(window, width, height);
    ratio := width / height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity;
    glOrtho(-ratio, ratio, -1, 1, 1, -1);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity;
    glRotatef(glfwGetTime * 50, 0, 0, 1);

    glBegin(GL_TRIANGLES);
      glColor3f(1, 0, 0);
      glVertex3f(-0.6, -0.4, 0);
      glColor3f(0, 1, 0);
      glVertex3f(0.6, -0.4, 0);
      glColor3f(0, 0, 1);
      glVertex3f(0, 0.6, 0);
    glEnd;

    glfwSwapBuffers(window);
    glfwPollEvents;
  end;

  glfwDestroyWindow(window);

  glfwTerminate;
end.
