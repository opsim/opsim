program graph;

uses
  GL,
  glut;

type
  plotfunc = function(x: double): double;

  // Sample func itself
  function func(x: double): double;
  begin
    exit(x * x);
  end;

  //function plotting func
  procedure draw(func: plotfunc; x1, x2, y1, y2: double; N: integer);
  var
    x: double;
    dx: double;
  begin
    dx := 1.0 / N;

    glPushMatrix(); // GL_MODELVIEW is default

    glScalef(1.0 / (x2 - x1), 1.0 / (y2 - y1), 1.0);
    glTranslatef(-x1, -y1, 0.0);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_STRIP);

    x := x1;
    while x < x2 do
    begin
      glVertex2f(x, func(x));
      x += dx;
    end;

    glEnd();

    glPopMatrix();
  end;

  // Redrawing func
  procedure redraw; cdecl;
  begin
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    draw(@func, -3, 3, 0, 10, 10);

    glutSwapBuffers();
  end;

  // Idle proc. Redisplays, if called.
  procedure idle; cdecl;
  begin
    glutPostRedisplay();
  end;

  // Key press processing
  procedure key(c: byte; x, y: integer); cdecl;
  begin
    if c = 27 then
      halt(0);
  end;

  // Window reashape
  procedure reshape(w, h: integer); cdecl;
  begin
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 0, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
  end;

begin
  glutInit(@argc, argv);
  glutInitDisplayMode(GLUT_RGB or GLUT_DOUBLE);
  glutCreateWindow('Graph plotter');

  // Register GLUT callbacks.
  glutDisplayFunc(@redraw);
  glutKeyboardFunc(@key);
  glutReshapeFunc(@reshape);
  glutIdleFunc(@idle);

  // Init the GL state
  glLineWidth(1.0);

  // Main loop
  glutMainLoop();
end.
