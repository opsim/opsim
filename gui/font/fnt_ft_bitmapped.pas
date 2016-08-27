//a bitmapped freetype font library
unit FNT_FT_bitmapped;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, freetypeh, GL, GLu;

type
  font_data = record
    name: string;
    size: integer;

    textures: PGLuint;
    list_base: GLuint;
  end;

procedure FNT_FT_bitmapped_create(var fd: font_data; name: string; size: integer);
procedure FNT_FT_bitmapped_clean(var fd: font_data);

procedure FNT_FT_bitmapped_textout(fd: font_data; x, y: double; Text: string);
procedure FNT_FT_bitmapped_textout2(fd: font_data; x, y: double; Text: string);

implementation

const
  FNT_CHAR_NUM = 255;

//This function gets the first power of 2 >= the
//int that we pass it.
function FNT_next_p2(a: integer): integer; inline;
var
  rval: integer;
begin
  rval := 2;
  while rval < a do
    rval := rval shl 1;
  Result := rval;
end;

//Create a display list coresponding to the give character.
procedure FNT_make_dlist(face: PFT_Face; ch: char; list_base: GLuint; tex_base: pGLuint);
var
  glyph: PFT_Glyph;
  bitmap_glyph: PFT_BitmapGlyph;
  bitmap: FT_Bitmap;
  width: integer;
  height: integer;
  expanded_data: pGLubyte;
  x: double;
  y: double;
  i, j: integer;
begin
  //The first thing we do is get FreeType to render our character
  //into a bitmap.  This actually requires a couple of FreeType commands:

  //Load the Glyph for our character.
  if FT_Load_Glyph(face, FT_Get_Char_Index(face, Ord(ch)), FT_LOAD_DEFAULT) = 1 then
    raise Exception.Create('FT_Load_Glyph failed');

  //Move the face's glyph into a Glyph object.
  if FT_Get_Glyph(face^.glyph, glyph) = 1 then
    raise Exception.Create('FT_Get_Glyph failed');

  FT_Glyph_To_Bitmap(glyph, FT_RENDER_MODE_NORMAL, nil, True);

  //Convert the glyph to a bitmap.
  bitmap_glyph := PFT_BitmapGlyph(glyph);

  //This reference will make accessing the bitmap easier
  bitmap := bitmap_glyph^.bitmap;

  //Use our helper function to get the widths of
  //the bitmap data that we will need in order to create
  //our texture.
  width := FNT_next_p2(bitmap.width);
  height := FNT_next_p2(bitmap.rows);

  //Allocate memory for the texture data.
  GetMem(expanded_data, 2 * width * height);
  //writeln(2 * width * height, 'bytes for character #', Ord(ch), ' - ', ch);

  //Here we fill in the data for the expanded bitmap.
  //Notice that we are using two channel bitmap (one for
  //luminocity and one for alpha), but we assign
  //both luminocity and alpha to the value that we
  //find in the FreeType bitmap.
  //We use the ?: operator so that value which we use
  //will be 0 if we are in the padding zone, and whatever
  //is the the Freetype bitmap otherwise.
  for j := 0 to height - 1 do
    for i := 0 to width - 1 do
    begin
      expanded_data[2 * (i + j * width)] := #255;

      if (i >= bitmap.width) or (j >= bitmap.rows) then
        expanded_data[2 * (i + j * width) + 1] := #0
      else
        expanded_data[2 * (i + j * width) + 1] :=
          char((bitmap.buffer + (i + bitmap.width * j))^);
    end;

  glBindTexture(GL_TEXTURE_2D, tex_base[Ord(ch)]);

  //Now we just setup some texture paramaters.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

  //Here we actually create the texture itself, notice
  //that we are using GL_LUMINANCE_ALPHA to indicate that
  //we are using 2 channel data.

  //With the texture created, we don't need to expanded data anymore
  FreeMem(expanded_data, 2 * width * height);
  glNewList(list_base + Ord(ch), GL_COMPILE);

  //So now we can create the display list
  glBindTexture(GL_TEXTURE_2D, tex_base[Ord(ch)]);
  glPushMatrix;
  glTranslatef(bitmap_glyph^.left, 0, 0);

  //first we need to move over a little so that
  //the character has the right amount of space
  //between it and the one before it.
  glTranslatef(0, bitmap_glyph^.top - bitmap.rows, 0);

  //Now we move down a little in the case that the
  //bitmap extends past the bottom of the line
  //(this is only true for characters like 'g' or 'y'.

  //Now we need to account for the fact that many of
  //our textures are filled with empty padding space.
  //We figure what portion of the texture is used by
  //the actual character and store that information in
  //the x and y variables, then when we draw the
  //quad, we will only reference the parts of the texture
  //that we contain the character itself.
  x := bitmap.width / width;
  y := bitmap.rows / height;

  //Here we draw the texturemaped quads.
  //The bitmap that we got from FreeType was not
  //oriented quite like we would like it to be,
  //so we need to link the texture to the quad
  //so that the result will be properly aligned.
  glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2f(0, bitmap.rows);
    glTexCoord2d(0, y);
    glVertex2f(0, 0);
    glTexCoord2d(x, y);
    glVertex2f(bitmap.width, 0);
    glTexCoord2d(x, 0);
    glVertex2f(bitmap.width, bitmap.rows);
  glEnd;
  glPopMatrix;
  glTranslatef(face^.glyph^.advance.x shr 6, 0, 0);

  //increment the raster position as if we were a bitmap font.
  //(only needed if you want to calculate text length)
  glBitmap(0, 0, 0, 0, face^.glyph^.advance.x shr 6, 0, nil);

  //Finish the display list
  glEndList;

  FT_Done_Glyph(glyph);
end;

procedure FNT_FT_bitmapped_create(var fd: font_data; name: string; size: integer);
var
  library_: PFT_Library = nil;
  face: PFT_Face = nil;
  //The object in which Freetype holds information on a given font is called a "face".
  i: byte;
begin
  //Allocate some memory to store the texture ids.
  GetMem(fd.textures, FNT_CHAR_NUM * SizeOf(GLuint));

  fd.size := size;
  fd.name := name;

  //Create and initilize a freetype font library.
  if FT_Init_FreeType(library_) = 1 then
    raise Exception.Create('FT_Init_FreeType failed');

  //This is where we load in the font information from the file.
  //Of all the places where the code might die, this is the most likely,
  //as FT_New_Face will die if the font file does not exist or is somehow broken.
  if FT_New_Face(library_, PChar(name), 0, face) = 1 then
    raise Exception.Create('FT_New_Face failed (there is probably a problem with your font file)');

  //For some twisted reason, Freetype measures font size
  //in terms of 1/64ths of pixels.  Thus, to make a font
  //h pixels high, we need to request a size of h*64.
  //(h << 6 is just a prettier way of writting h*64)
  FT_Set_Char_Size(face, size shl 6, size shl 6, 96, 96);

  //Here we ask opengl to allocate resources for
  //all the textures and displays lists which we
  //are about to create.
  fd.list_base := glGenLists(FNT_CHAR_NUM);
  glGenTextures(FNT_CHAR_NUM, fd.textures);

  //This is where we actually create each of the fonts display lists.
  for i := 0 to FNT_CHAR_NUM - 1 do
    FNT_make_dlist(face, Chr(i), fd.list_base, fd.textures);

  //We don't need the face information now that the display
  //lists have been created, so we free the assosiated resources.
  FT_Done_Face(face);

  //Ditto for the library.
  FT_Done_FreeType(library_);
end;

procedure FNT_FT_bitmapped_clean(var fd: font_data);
begin
  glDeleteLists(fd.list_base, FNT_CHAR_NUM);
  fd.list_base := 0;
  glDeleteTextures(FNT_CHAR_NUM, fd.textures);
  FreeMem(fd.textures, FNT_CHAR_NUM * SizeOf(GLuint));

  //now clear all fields in the font record
  FillByte(fd, SizeOf(fd), 0);
end;

//A fairly straight forward function that pushes
//a projection matrix that will make object world
//coordinates identical to window coordinates.
procedure FNT_pushScreenCoordinateMatrix;
var
  viewport: array [0..3] of GLint;
begin
  glPushAttrib(GL_TRANSFORM_BIT);
  glGetIntegerv(GL_VIEWPORT, viewport);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix;
  glLoadIdentity;
  gluOrtho2D(viewport[0], viewport[2], viewport[1], viewport[3]);
  glPopAttrib;
end;

//Pops the projection matrix without changing the current
//MatrixMode.
procedure FNT_pop_projection_matrix;
begin
  glPushAttrib(GL_TRANSFORM_BIT);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix;
  glPopAttrib;
end;

//Much like Nehe's glPrint function, but modified to work
//with freetype fonts.
procedure FNT_FT_bitmapped_textout(fd: font_data; x, y: double; Text: string);
var
  font: GLuint;
  modelview_matrix: array [0..15] of double;
begin
  FNT_pushScreenCoordinateMatrix;

  //We want a coordinate system where things coresponding to window pixels.
  font := fd.list_base;

  //Results Are Stored In Text
  glPushAttrib(GL_LIST_BIT or GL_CURRENT_BIT or GL_ENABLE_BIT or GL_TRANSFORM_BIT);

  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glListBase(font);

  glGetFloatv(GL_MODELVIEW_MATRIX, @modelview_matrix[0]);

  glPushMatrix;
  glLoadIdentity;
  glTranslatef(x, y, 0);
  glMultMatrixf(@modelview_matrix[0]);
  glCallLists(Length(Text), GL_UNSIGNED_BYTE, PChar(Text));

  //The commented out raster position stuff can be useful if you need to
  //know the length of the text that you are creating.
  //If you decide to use it make sure to also uncomment the glBitmap command
  //in make_dlist.
  //glRasterPos2f(0,0);
  glPopMatrix;
  //float rpos[4];
  //glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
  //float len=x-rpos[0];

  glPopAttrib;
  FNT_pop_projection_matrix;
end;

procedure FNT_FT_bitmapped_textout2(fd: font_data; x, y: double; Text: string);
var
  font: GLuint;
  modelview_matrix: array [0..15] of double;
begin
  //FNT_pushScreenCoordinateMatrix;

  //We want a coordinate system where things coresponding to window pixels.
  font := fd.list_base;

  ////Results Are Stored In Text
  glPushAttrib(GL_LIST_BIT or GL_CURRENT_BIT or GL_ENABLE_BIT or GL_TRANSFORM_BIT);
  //
  //glMatrixMode(GL_MODELVIEW);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  //
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glListBase(font);

  //glGetFloatv(GL_MODELVIEW_MATRIX, @modelview_matrix[0]);

  glPushMatrix;
  glLoadIdentity;
  glTranslatef(x, y, 0);
  //glMultMatrixf(@modelview_matrix[0]);
  glCallLists(Length(Text), GL_UNSIGNED_BYTE, PChar(Text));

  //The commented out raster position stuff can be useful if you need to
  //know the length of the text that you are creating.
  //If you decide to use it make sure to also uncomment the glBitmap command
  //in make_dlist.
  //glRasterPos2f(0,0);
  glPopMatrix;
  //float rpos[4];
  //glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
  //float len=x-rpos[0];

  glPopAttrib;
  //FNT_pop_projection_matrix;
end;

end.
