#ifndef MUPDF_HTML_H
#define MUPDF_HTML_H

#include "mupdf/fitz.h"

typedef struct fz_html_font_set_s fz_html_font_set;
typedef struct fz_html_s fz_html;
typedef struct fz_html_flow_s fz_html_flow;

typedef struct fz_css_rule_s fz_css_rule;
typedef struct fz_css_match_prop_s fz_css_match_prop;
typedef struct fz_css_match_s fz_css_match;
typedef struct fz_css_style_s fz_css_style;

typedef struct fz_css_selector_s fz_css_selector;
typedef struct fz_css_condition_s fz_css_condition;
typedef struct fz_css_property_s fz_css_property;
typedef struct fz_css_value_s fz_css_value;
typedef struct fz_css_number_s fz_css_number;
typedef struct fz_css_color_s fz_css_color;

struct fz_html_font_set_s
{
	fz_font *fonts[16];
	fz_font *fallback;
};

enum
{
	CSS_KEYWORD = 256,
	CSS_HASH,
	CSS_STRING,
	CSS_NUMBER,
	CSS_LENGTH,
	CSS_PERCENT,
	CSS_URI,
};

struct fz_css_rule_s
{
	fz_css_selector *selector;
	fz_css_property *declaration;
	fz_css_property *garbage; /* for freeing inline style attributes at the end */
	fz_css_rule *next;
};

struct fz_css_selector_s
{
	char *name;
	int combine;
	fz_css_condition *cond;
	fz_css_selector *left;
	fz_css_selector *right;
	fz_css_selector *next;
};

struct fz_css_condition_s
{
	int type;
	char *key;
	char *val;
	fz_css_condition *next;
};

struct fz_css_property_s
{
	char *name;
	fz_css_value *value;
	short spec;
	short important;
	fz_css_property *next;
};

struct fz_css_value_s
{
	int type;
	char *data;
	fz_css_value *args; /* function arguments */
	fz_css_value *next;
};

struct fz_css_match_prop_s
{
	const char *name; /* not owned */
	fz_css_value *value; /* not owned */
	int spec;
};

struct fz_css_match_s
{
	fz_css_match *up;
	int count;
	fz_css_match_prop prop[64];
};

enum { DIS_NONE, DIS_BLOCK, DIS_INLINE, DIS_LIST_ITEM, DIS_INLINE_BLOCK };
enum { POS_STATIC, POS_RELATIVE, POS_ABSOLUTE, POS_FIXED };
enum { TA_LEFT, TA_RIGHT, TA_CENTER, TA_JUSTIFY };
enum { VA_BASELINE, VA_SUB, VA_SUPER, VA_TOP, VA_BOTTOM };
enum { BS_NONE, BS_SOLID };
enum { V_VISIBLE, V_HIDDEN, V_COLLAPSE };

enum {
	WS_COLLAPSE = 1,
	WS_ALLOW_BREAK_SPACE = 2,
	WS_FORCE_BREAK_NEWLINE = 4,
	WS_NORMAL = WS_COLLAPSE | WS_ALLOW_BREAK_SPACE,
	WS_PRE = WS_FORCE_BREAK_NEWLINE,
	WS_NOWRAP = WS_COLLAPSE,
	WS_PRE_WRAP = WS_ALLOW_BREAK_SPACE | WS_FORCE_BREAK_NEWLINE,
	WS_PRE_LINE = WS_COLLAPSE | WS_ALLOW_BREAK_SPACE | WS_FORCE_BREAK_NEWLINE
};

enum {
	LST_NONE,
	LST_DISC, LST_CIRCLE, LST_SQUARE,
	LST_DECIMAL, LST_DECIMAL_ZERO,
	LST_LC_ROMAN, LST_UC_ROMAN,
	LST_LC_GREEK, LST_UC_GREEK,
	LST_LC_LATIN, LST_UC_LATIN,
	LST_LC_ALPHA, LST_UC_ALPHA,
	LST_ARMENIAN, LST_GEORGIAN,
};

enum { N_NUMBER='p', N_SCALE='m', N_PERCENT='%', N_AUTO='a' };

struct fz_css_number_s
{
	float value;
	int unit;
};

struct fz_css_color_s
{
	unsigned char r, g, b, a;
};

struct fz_css_style_s
{
	fz_css_number font_size;
	fz_css_number width, height;
	fz_css_number margin[4];
	fz_css_number padding[4];
	fz_css_number border_width[4];
	fz_css_number text_indent;
	char visibility;
	char white_space;
	char text_align;
	char vertical_align;
	char list_style_type;
	char border_style[4];
	fz_css_number line_height;
	fz_css_color background_color;
	fz_css_color border_color[4];
	fz_css_color color;
	fz_font *font, *fallback;
};

enum
{
	BOX_BLOCK,	/* block-level: contains block, break, and flow boxes */
	BOX_BREAK,	/* block-level: empty <br> tag boxes */
	BOX_FLOW,	/* block-level: contains only inline boxes */
	BOX_INLINE,	/* inline-level: contains only inline boxes */
};

struct fz_html_s
{
	int type;
	float x, y, w, h; /* content */
	float padding[4];
	float margin[4];
	float border[4];
	float em;
	fz_html *up, *down, *last, *next;
	fz_html_flow *flow_head, **flow_tail;
	fz_bidi_direction flow_dir;
	fz_css_style style;
	int list_item;
	int is_first_flow; /* for text-indent */
	fz_pool *pool; /* pool allocator for this html tree (only set for root block) */
};

enum
{
	FLOW_WORD = 0,
	FLOW_GLUE = 1,
	FLOW_BREAK = 2,
	FLOW_IMAGE = 3
};

/* We have to recognise the distinction between render direction
 * and layout direction. For most strings render direction and
 * logical direction are the same.
 *
 * Char direction determines whether a string 'ABC' appears as
 * ABC or CBA.
 *
 * Block direction determines how fragments are attached together.
 * 'ABC' and 'DEF' with r2l char and block directions will
 * appear as 'FEDCBA'. With l2r char and block it will appear
 * as 'ABCDEF'.
 *
 * The reason for the distinction is that we can have logical
 * strings like 'ABC0123DEF', where 'ABC' and 'DEF' are in r2l
 * scripts. The bidirectional code breaks this down into 3 fragments
 * 'ABC' '0123' 'DEF', where all three are r2l, but digits need to
 * be rendered left to right. i.e. the desired result is:
 * FED0123CBA, rather than FED3210CBA.
 */
struct fz_html_flow_s
{
	unsigned int type : 2;
	unsigned int expand : 1;
	unsigned int char_r2l : 1;
	unsigned int block_r2l : 1;
	unsigned int mirror : 1;
	float x, y, w, h, em;
	fz_css_style *style;
	union {
		char *text;
		fz_image *image;
	} content;
	fz_html_flow *next;
};

fz_css_rule *fz_parse_css(fz_context *ctx, fz_css_rule *chain, const char *source, const char *file);
fz_css_property *fz_parse_css_properties(fz_context *ctx, const char *source);
void fz_drop_css(fz_context *ctx, fz_css_rule *rule);

void fz_match_css(fz_context *ctx, fz_css_match *match, fz_css_rule *rule, fz_xml *node);
void fz_match_css_at_page(fz_context *ctx, fz_css_match *match, fz_css_rule *css);

int fz_get_css_match_display(fz_css_match *node);
void fz_default_css_style(fz_context *ctx, fz_css_style *style);
void fz_apply_css_style(fz_context *ctx, fz_html_font_set *set, fz_css_style *style, fz_css_match *match);

float fz_from_css_number(fz_css_number, float em, float width);
float fz_from_css_number_scale(fz_css_number number, float scale, float em, float width);

fz_html_font_set *fz_new_html_font_set(fz_context *ctx);
fz_font *fz_load_html_font(fz_context *ctx, fz_html_font_set *set,
	const char *family, const char *variant, const char *style, const char *weight);
fz_font *fz_load_html_fallback_font(fz_context *ctx, fz_html_font_set *set);
void fz_drop_html_font_set(fz_context *ctx, fz_html_font_set *htx);

fz_html *fz_parse_html(fz_context *ctx, fz_html_font_set *htx, fz_archive *zip, const char *base_uri, fz_buffer *buf, const char *user_css);
void fz_layout_html(fz_context *ctx, fz_html *box, float w, float h, float em);
void fz_draw_html(fz_context *ctx, fz_html *box, float page_top, float page_bot, fz_device *dev, const fz_matrix *ctm);
void fz_drop_html(fz_context *ctx, fz_html *box);

#endif
