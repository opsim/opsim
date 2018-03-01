program drawgrid;

uses
  GL,
  glut;

const
  WIDTH = 1000;
  HEIGHT = 400;
  OFFSET = 200;

var
  x: double = 0;
  y: double = 0;

  procedure init;
  begin
    //glClearColor (1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WIDTH, 0.0, HEIGHT, -2.0, 2.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  end;

  procedure drawline(x1, y1, x2, y2: double);
  begin
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
  end;

  procedure drawgrid();
  var
    i: integer;
  begin
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3ub(240, 57, 53);

    i := 0;
    while i < HEIGHT - 1 do
    begin
      if (i mod 100 = 0) then
        glLineWidth(3.0)
      else if (i mod 50 = 0) then
        glLineWidth(2.0)
      else
        glLineWidth(1.0);

      drawline(0, i, WIDTH, i);
      i += 10;
    end;

    i := 0;
    while i < WIDTH do
    begin
      if (i mod 100 = 0) then
        glLineWidth(3.0)
      else if (i mod 50 = 0) then
        glLineWidth(2.0)
      else
        glLineWidth(1.0);

      drawline(i, 0, i, HEIGHT);
      i += 10;
    end;
  end;

  procedure display; cdecl;
  begin
    drawgrid();

    glFlush();
    glutSwapBuffers();
  end;

begin
  glutInit(@argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE or GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow('ECG');

  init();

  glutDisplayFunc(@display);
  glDisable(GL_TEXTURE_2D);

  glutMainLoop();
end.
