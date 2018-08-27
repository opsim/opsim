(*
 * 3-D gear wheels.  This program is in the public domain.
 *
 * Brian Paul
 *
 * Darius Blaszyk
 *   - Conversion to ANT (24 September 2017)
 *)

program gears;

uses
  StrUtils,
  GL,
  DNA_listbase,
  ANT_main,
  ANT_types,
  ANT_messages;

(**

  Draw a gear wheel.  You'll probably want to call this function when
  building a display list since we do a lot of trig here.

  Input:  inner_radius - radius of hole at center
          outer_radius - radius at center of teeth
          width - width of gear
          teeth - number of teeth
          tooth_depth - depth of tooth

 **)

  procedure gear(inner_radius, outer_radius, width: GLfloat;
    teeth: GLint; tooth_depth: GLfloat);
  var
    i: GLint;
    r0, r1, r2: GLfloat;
    angle, da: GLfloat;
    u, v, len: GLfloat;
  begin

    r0 := inner_radius;
    r1 := outer_radius - tooth_depth / 2.0;
    r2 := outer_radius + tooth_depth / 2.0;

    da := 2.0 * PI / teeth / 4.0;

    glShadeModel(GL_FLAT);

    glNormal3f(0.0, 0.0, 1.0);

    (* draw front face *)
    glBegin(GL_QUAD_STRIP);
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    end;
    glEnd;

    (* draw front sides of teeth *)
    glBegin(GL_QUADS);
    da := 2.0 * PI / teeth / 4.0;
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
    end;
    glEnd;

    glNormal3f(0.0, 0.0, -1.0);

    (* draw back face *)
    glBegin(GL_QUAD_STRIP);
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
    end;
    glEnd;

    (* draw back sides of teeth *)
    glBegin(GL_QUADS);
    da := 2.0 * PI / teeth / 4.0;
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;

      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
    end;
    glEnd;

    (* draw outward faces of teeth *)
    glBegin(GL_QUAD_STRIP);
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;

      glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      u := r2 * cos(angle + da) - r1 * cos(angle);
      v := r2 * sin(angle + da) - r1 * sin(angle);
      len := sqrt(u * u + v * v);
      u := u / len;
      v := v / len;
      glNormal3f(v, -u, 0.0);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
      glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
      u := r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
      v := r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
      glNormal3f(v, -u, 0.0);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
      glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
      glNormal3f(cos(angle), sin(angle), 0.0);
    end;

    glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
    glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

    glEnd;

    glShadeModel(GL_SMOOTH);

    (* draw inside radius cylinder *)
    glBegin(GL_QUAD_STRIP);
    for i := 0 to teeth - 1 do
    begin
      angle := i * 2.0 * PI / teeth;

      glNormal3f(-cos(angle), -sin(angle), 0.0);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
    end;
    glEnd;
  end;

var
  win: pANTWindow;
  view_rotx: GLfloat = 20.0;
  view_roty: GLfloat = 30.0;
  view_rotz: GLfloat = 0.0;
  gear1, gear2, gear3: GLint;
  angle: GLfloat = 0.0;

  limit: GLuint = 0;
  count: GLuint = 1;

  procedure draw;
  begin
    glClear(GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT);

    glPushMatrix;
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);

    glPushMatrix;
    glTranslatef(-3.0, -2.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glCallList(gear1);
    glPopMatrix;

    glPushMatrix;
    glTranslatef(3.1, -2.0, 0.0);
    glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
    glCallList(gear2);
    glPopMatrix;

    glPushMatrix;
    glTranslatef(-3.1, 4.2, 0.0);
    glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
    glCallList(gear3);
    glPopMatrix;

    glPopMatrix;

    inc(count);
    if (count = limit) then
      ANT_SetWindowShouldClose(win, True);
  end;

  procedure idle;
  begin
    angle += 2.0;
    //glutPostRedisplay;
  end;

  procedure error_callback(error: integer;
  const description: string);
  begin
    writeln(stderr, description);
  end;

  procedure event_callback(msg: pointer);
  var
    event: pANT_MessageRec;
    h: GLfloat;
  begin
    event := msg;

    case event^.mcode of
      ANT_MESSAGE_KEYPRESS:
      begin
        (* change view angle, exit upon ESC *)
        case event^.params.keyboard.keychar of
          ANT_KEY_Z: view_rotz += 5.0;
          ANT_KEY_X: view_rotz -= 5.0;
          ANT_KEY_ESCAPE: ANT_SetWindowShouldClose(event^.win, True);
          ANT_KEY_UP: view_rotx += 5.0;
          ANT_KEY_DOWN: view_rotx -= 5.0;
          ANT_KEY_LEFT: view_roty += 5.0;
          ANT_KEY_RIGHT: view_roty -= 5.0;
        end;
      end;
      ANT_MESSAGE_RESIZE:
      begin
        (* new window size or exposure *)
        h := event^.params.rect.height / event^.params.rect.width;

        glViewport(0, 0, event^.params.rect.width, event^.params.rect.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity;
        glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity;
        glTranslatef(0.0, 0.0, -40.0);
      end;
      ANT_MESSAGE_MOUSEDOWN:
      begin
        writeln(event^.params.mouse.buttons);
      end;
    end;
  end;

var
  pos: array [0..3] of GLfloat = (5.0, 5.0, 10.0, 0.0);
  red: array [0..3] of GLfloat = (0.8, 0.1, 0.0, 1.0);
  green: array [0..3] of GLfloat = (0.0, 0.8, 0.2, 1.0);
  blue: array [0..3] of GLfloat = (0.2, 0.2, 1.0, 1.0);

  procedure init;
  begin
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    (* make the gears *)
    gear1 := glGenLists(1);
    glNewList(gear1, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    gear(1.0, 4.0, 1.0, 20, 0.7);
    glEndList;

    gear2 := glGenLists(1);
    glNewList(gear2, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    gear(0.5, 2.0, 2.0, 10, 0.7);
    glEndList;

    gear3 := glGenLists(1);
    glNewList(gear3, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    gear(1.3, 2.0, 0.5, 10, 0.7);
    glEndList;

    glEnable(GL_NORMALIZE);
  end;

var
  code: integer;

begin
  if ParamCount > 0 then
  begin
    (* do 'n' frames then exit *)
    val(ParamStr(1), limit, code);
    if code <> 0 then
    begin
      writeln('error: ', Paramstr(1));
      writeln(' ': (code + 6), '^');
      halt(1);
    end;
  end;

  ANT_SetErrorCallback(@error_callback);

  if not ANT_Init then
    halt(-1);

  win := ANT_CreateWindow(0, 0, 640, 480, 'Gears');

  win^.event_callback := @event_callback;

  init;

  while not ANT_WindowShouldClose(win) do
  begin
    draw;

    angle += 0.02;

    ANT_SwapBuffers(win);
    ANT_PollEvents;
  end;

  ANT_DestroyWindow(win);

  ANT_Terminate;
end.
