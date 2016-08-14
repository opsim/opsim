//========================================================================
// Simple multi-window test
// Copyright (c) Camilla Berglund <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//
// This test creates four windows and clears each in a different color
//
//========================================================================

program glfw_windows;

uses
  GL,
  glfw3,
  getopts;

const
  titles: array [0..3] of string = (
    'Red',
    'Green',
    'Blue',
    'Yellow');

type
  color = record
    r, g, b: double;
  end;

var
  colors: array [0..3] of color = (
    (r: 0.95; g: 0.32; b: 0.11),
    (r: 0.50; g: 0.80; b: 0.16),
    (r: 0.00; g: 0.68; b: 0.94),
    (r: 0.98; g: 0.74; b: 0.04));

  procedure usage;
  begin
    writeln('Usage: windows [-h] [-b]');
    writeln('Options:');
    writeln('  -b create decorated windows');
    writeln('  -h show this help');
  end;

  procedure error_callback(error: integer; const description: PChar); cdecl;
  begin
    writeln(stderr, 'Error: ', description);
  end;

  procedure key_callback(window: pGLFWwindow; key, scancode, action, mods: integer); cdecl;
  var
    xpos, ypos: integer;
  begin
    if action <> GLFW_PRESS then
      exit;

    case key of
      GLFW_KEY_SPACE:
      begin
        glfwGetWindowPos(window, xpos, ypos);
        glfwSetWindowPos(window, xpos, ypos);
      end;

      GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    end;
  end;

var
  i: integer;
  ch: char;
  decorated: integer = GLFW_FALSE;
  running: integer = GLFW_TRUE;
  windows: array [0..3] of pGLFWwindow;
  left, top, right, bottom: integer;

begin
  repeat
    ch := getopt('bh');
    case ch of
      'b':
        decorated := GLFW_TRUE;
      'h':
      begin
        usage();
        halt(1);
      end;
    end;
  until ch = EndOfOptions;

  glfwSetErrorCallback(@error_callback);

  if glfwInit = 0 then
    halt(0);

  glfwWindowHint(GLFW_DECORATED, decorated);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  for i := 0 to 3 do
  begin
    windows[i] := glfwCreateWindow(200, 200, PChar(titles[i]), nil, nil);
    if windows[i] = nil then
    begin
      glfwTerminate();
      halt(0);
    end;

    glfwSetKeyCallback(windows[i], @key_callback);

    glfwMakeContextCurrent(windows[i]);
    glClearColor(colors[i].r, colors[i].g, colors[i].b, 1);

    glfwGetWindowFrameSize(windows[i], left, top, right, bottom);
    glfwSetWindowPos(windows[i],
      100 + (i and 1) * (200 + left + right),
      100 + (i shr 1) * (200 + top + bottom));
  end;

  for i := 0 to 3 do
    glfwShowWindow(windows[i]);

  while running <> GLFW_FALSE do
  begin
    for i := 0 to 3 do
    begin
      glfwMakeContextCurrent(windows[i]);
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(windows[i]);

      if glfwWindowShouldClose(windows[i]) <> 0 then
        running := GLFW_FALSE;
    end;

    glfwWaitEvents();
  end;

  glfwTerminate();
end.
