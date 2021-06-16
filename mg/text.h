#pragma once

#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "csc/csc_xlog.h"
#include "csc/csc_math.h"




struct gchar
{
	float ax;	// advance.x
	float ay;	// advance.y
	float bw;	// bitmap.width;
	float bh;	// bitmap.height;
	float bl;	// bitmap_left;
	float bt;	// bitmap_top;
	float tx;	// x offset of glyph in texture coordinates
	float ty;	// y offset of glyph in texture coordinates
};


struct gatlas
{
	unsigned int w;
	unsigned int h;
};



// Maximum texture width
#define MAXWIDTH 1024



static int gtext_init
(char const * fontname, struct gchar c[], GLint uniform_tex, int height, struct gatlas * atlas)
{
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		XLOG (XLOG_ERR, XLOG_GENERAL, "Could not init FreeType Library");
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontname, 0, &face))
	{
		XLOG (XLOG_ERR, XLOG_GENERAL, "Failed to load font");
		return -1;
	}

	FT_Set_Pixel_Sizes (face, 0, height);
	FT_GlyphSlot g = face->glyph;
	unsigned int w = 0;
	unsigned int h = 0;
	unsigned int roww = 0;
	unsigned int rowh = 0;


	/* Find minimum size for a texture holding all visible ASCII characters */
	for (int i = 32; i < 128; i++)
	{
		if (FT_Load_Char (face, i, FT_LOAD_RENDER))
		{
			XLOG (XLOG_ERR, XLOG_GENERAL, "Loading character %c failed!\n", i);
			continue;
		}

		if (roww + g->bitmap.width + 1 >= MAXWIDTH)
		{
			w = MAX(w, roww);
			h += rowh;
			roww = 0;
			rowh = 0;
		}
		roww += g->bitmap.width + 1;
		rowh = MAX(rowh, g->bitmap.rows);
	}
	w = MAX(w, roww);
	h += rowh;


	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Paste all glyph bitmaps into the texture, remembering the offset */
	int ox = 0;
	int oy = 0;

	rowh = 0;

	for (int i = 32; i < 128; i++)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			XLOG (XLOG_ERR, XLOG_GENERAL, "Loading character %c failed!\n", i);
			continue;
		}

		if (ox + g->bitmap.width + 1 >= MAXWIDTH)
		{
			oy += rowh;
			rowh = 0;
			ox = 0;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		c[i].ax = g->advance.x >> 6;
		c[i].ay = g->advance.y >> 6;

		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;

		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;

		c[i].tx = ox / (float)w;
		c[i].ty = oy / (float)h;

		rowh = MAX(rowh, g->bitmap.rows);
		ox += g->bitmap.width + 1;
	}

	atlas->w = w;
	atlas->h = h;
	return 0;
}



static void gtext_gen_pos (v4f32 pos[6], float x, float y, float w, float h)
{
	pos[0] = (v4f32){{x + 0, y + 0, 0, 0}};
	pos[1] = (v4f32){{x + w, y + 0, 0, 0}};
	pos[2] = (v4f32){{x + 0, y + h, 0, 0}};
	pos[3] = (v4f32){{x + w, y + 0, 0, 0}};
	pos[4] = (v4f32){{x + 0, y + h, 0, 0}};
	pos[5] = (v4f32){{x + w, y + h, 0, 0}};
}

static void gtext_gen_uv (v2f32 uv[6], float x, float y, float w, float h)
{
	uv[0] = (v2f32){{x + 0, y + 0}};
	uv[1] = (v2f32){{x + w, y + 0}};
	uv[2] = (v2f32){{x + 0, y + h}};
	uv[3] = (v2f32){{x + w, y + 0}};
	uv[4] = (v2f32){{x + 0, y + h}};
	uv[5] = (v2f32){{x + w, y + h}};
}



/**
 * Render text using the currently loaded font and currently set font size.
 * Rendering starts at coordinates (x, y), z is always 0.
 * The pixel coordinates that the FreeType2 library uses are scaled by (sx, sy).
 */
static void render_text
(
		const char *text,
		struct gchar c[],
		unsigned int aw, //Atlas width
		unsigned int ah, //Atlas height
		GLuint vbo_pos,
		GLuint vbo_uv,
		float x, //Coordinate x
		float y, //Coordinate y
		float sx,
		float sy
)
{

	v4f32 pos[6 * 100];
	v2f32 uv[6 * 100];
	int i = 0;
	for (const uint8_t *p = (const uint8_t *)text; *p; p++)
	{
		/* Calculate the vertex and texture coordinates */
		float x2 = x + c[*p].bl * sx;
		float y2 = y + c[*p].bt * sy;
		float w = c[*p].bw * sx;
		float h = c[*p].bh * -sy;
		float tx = c[*p].tx;
		float ty = c[*p].ty;
		float tw = c[*p].bw / aw;
		float th = c[*p].bh / ah;

		/* Advance the cursor to the start of the next character */
		x += c[*p].ax * sx;
		y += c[*p].ay * sy;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		gtext_gen_pos (pos + i, x2, y2, w, h);
		gtext_gen_uv (uv + i, tx, ty, tw, th);
		i += 6;
	}



	/*
	pos[0] = (v4f32){{0.0f, 0.0f, 0.0f, 0.0f}};
	pos[1] = (v4f32){{0.0f, 1.0f, 0.0f, 0.0f}};
	pos[2] = (v4f32){{1.0f, 1.0f, 0.0f, 0.0f}};
	*/



	/* Draw all the character on the screen in one go */
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferData (GL_ARRAY_BUFFER, sizeof(v4f32)*i, pos, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv);
	glBufferData (GL_ARRAY_BUFFER, sizeof(v2f32)*i, uv, GL_DYNAMIC_DRAW);
	glDrawArrays (GL_TRIANGLES, 0, i);
}














struct gtext_context
{
	GLuint tex;
	GLuint vao;
	GLuint vbo_pos;
	GLuint vbo_uv;
	GLint uniform_tex;
	GLint uniform_mvp;
	GLuint program;
	struct gchar c[128];
	struct gatlas atlas;
};


static void gtext_context_init(struct gtext_context * ctx, char const * fontname, GLuint program)
{
	glUseProgram (program);
	ctx->uniform_tex = glGetUniformLocation (program, "tex0");
	ctx->uniform_mvp = glGetUniformLocation (program, "mvp");
	ctx->program = program;
	// Create a texture that will be used to hold all ASCII glyphs
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &ctx->tex);
	glBindTexture(GL_TEXTURE_2D, ctx->tex);
	glUniform1i(ctx->uniform_tex, 0);
	gtext_init (fontname, ctx->c, ctx->uniform_tex, 48, &ctx->atlas);
	// Set up the VBO for our vertex data

	glGenBuffers(1, &ctx->vbo_pos);
	glGenBuffers(1, &ctx->vbo_uv);
	glGenVertexArrays (1, &ctx->vao);
	glBindVertexArray (ctx->vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo_pos);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo_uv);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	/**/
}

static void gtext_context_draw(struct gtext_context * ctx, char const * text, float x, float y, float sx, float sy, float mvp[4*4])
{
	glBindVertexArray (ctx->vao);
	glUseProgram(ctx->program);
	/* Use the texture containing the atlas */
	glBindTexture (GL_TEXTURE_2D, ctx->tex);
	glUniform1i (ctx->uniform_tex, 0);
	glUniformMatrix4fv (ctx->uniform_mvp, 1, GL_FALSE, (const GLfloat *) mvp);
	render_text (text, ctx->c, ctx->atlas.w, ctx->atlas.h, ctx->vbo_pos, ctx->vbo_uv, x, y, sx, sy);
}


