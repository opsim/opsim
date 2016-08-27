program main;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX} {$IFDEF UseCThreads}
    cthreads,
  {$ENDIF} {$ENDIF}
  SysUtils,
  GL, GLu, glfw3,
  FNT_FT_bitmapped;

var
  window: pGLFWwindow;
  ratio: double;
  nbFrames: longint = 0;
  lastTime: double;
  font: font_data;
  width: integer = 1024;
  height: integer = 768;
  cnt1: GLfloat;                         //1st Counter Used To Move Text & For Coloring
  cnt2: GLfloat;                         //2nd Counter Used To Move Text & For Coloring

  procedure glInit;
  begin
    glShadeModel(GL_SMOOTH);                           //Enable smooth shading
    glClearColor(0, 0, 0, 0.5);                        //Black background
    glClearDepth(1);                                   //Depth buffer setup
    glEnable(GL_DEPTH_TEST);                           //Enables depth testing
    glDepthFunc(GL_LEQUAL);                            //The type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Really nice perspective calculations
    FNT_FT_bitmapped_create(font, 'Ubuntu-R.ttf', 10); //Build the freetype font
  end;

  procedure DisplayWindow; cdecl;
  begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);
    //Clear screen and depth buffer

    //some blue text
    glColor3ub(0, 0, $ff);
    FNT_FT_bitmapped_textout(font, 10, 100,
      'Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut ');

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(10, 10);
    glVertex2f(100, 100);
    glEnd;


    //some red rotating text
    glColor3ub($ff, 0, 0);
    glPushMatrix;
    glLoadIdentity;
    glRotatef(cnt1, 0, 0, 1);
    glScalef(1, 0.8 + 0.3 * cos(cnt1 / 5), 1);
    glTranslatef(-180, 0, 0);
    FNT_FT_bitmapped_textout(font, 320, 240, Format('Active FreeType Text - %7.2f', [cnt1]));
    glPopMatrix;
    cnt1 := cnt1 + 0.29;  //Increase The First Counter
    cnt2 := cnt2 + 0.5;   //Increase The second Counter

    //some green text
    glColor3ub(0, $ff, 0);
    FNT_FT_bitmapped_textout(font, 10, 200, 'The quick brown fox jumps over the lazy dog');

    glfwSwapBuffers(window);
    glfwPollEvents;
  end;

  procedure OnReshape(Width, Height: integer); cdecl;
  begin
    if Height = 0 then                  // prevent divide by zero exception
      Height := 1;

    glViewport(0, 0, Width, Height);    // Set the viewport for the OpenGL window
    glMatrixMode(GL_PROJECTION);        // Change Matrix Mode to Projection
    glLoadIdentity();                   // Reset View
    gluPerspective(45.0, Width / glFloat(Height), 2.0, 200.0);
    // Do the perspective calculations. Last value = max clipping depth

    glMatrixMode(GL_MODELVIEW);         // Return to the modelview matrix
    glLoadIdentity();                   // Reset View
  end;

  procedure KeyPressed(k: byte; x, y: longint); cdecl;
  begin
    case k of
      27:
      begin
        FNT_FT_bitmapped_clean(font);
        halt(0);
      end;
    end;
  end;

begin
  if glfwInit = 0 then
    halt(-1);

  window := glfwCreateWindow(width, height, 'Font example', nil, nil);
  if window = nil then
  begin
    glfwTerminate;
    halt(-1);
  end;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glInit;

  while glfwWindowShouldClose(window) = 0 do
  begin
    DisplayWindow;
  end;

  glfwDestroyWindow(window);

  glfwTerminate;
end.
