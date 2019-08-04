#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#include <pango/pangocairo.h>
#include <cairo.h>
#include <cairo-xlib.h>

int event_loop(cairo_surface_t *sfc, int block)
{
   XEvent e;

   while (1)
   {
      if (block || XPending(cairo_xlib_surface_get_display(sfc)))
         XNextEvent(cairo_xlib_surface_get_display(sfc), &e);
      else 
         return 0;
   }
}

cairo_surface_t *cairo_create_surface()
{
   Display *dsp;
   Drawable da;
   int screen;
   cairo_surface_t *sfc;

   if ((dsp = XOpenDisplay(NULL)) == NULL)
      exit(1);
   screen = DefaultScreen(dsp);


   XVisualInfo vinfo;
   XMatchVisualInfo(dsp,
                    screen,
                    32, TrueColor, &vinfo);
   
   XSetWindowAttributes attr;
   attr.colormap =
     XCreateColormap(dsp,
                     DefaultRootWindow(dsp),
                     vinfo.visual,
                     AllocNone);
   
   attr.border_pixel = 0;
   attr.background_pixel = 0;
   attr.override_redirect = 1;

   int screen_num = DefaultScreen(dsp);
   Screen* pScreen = ScreenOfDisplay(dsp, screen_num);

   int width  = pScreen->width;
   int height = pScreen->height;
   
   da = 
     XCreateWindow (dsp,
                    DefaultRootWindow(dsp),
                    0, 0,
                    width, height,
                    0,
                    vinfo.depth,
                    InputOutput,
                    vinfo.visual,
                    CWBorderPixel | CWBackPixel | CWColormap | CWOverrideRedirect,
                    &attr);

   XSelectInput(dsp, da, ExposureMask);
   XMapWindow(dsp, da);

   XClassHint *classhint;

   classhint = XAllocClassHint();
   if (classhint)
     {
       classhint->res_name = "xtext";
       classhint->res_class = "xtext";
       XSetClassHint(dsp, da, classhint);
       XFree(classhint);
     }

   sfc = cairo_xlib_surface_create(dsp, da, vinfo.visual, width, height);
   cairo_xlib_surface_set_size(sfc, width, height);

   XRectangle rect;
   XserverRegion region = XFixesCreateRegion(dsp, &rect, 1);

   XFixesSetWindowShapeRegion(dsp, da, ShapeInput, 0, 0, region);
   XFixesDestroyRegion(dsp, region);

   return sfc;
}

void cairo_draw_text(cairo_surface_t *sfc, cairo_t *cr, char *string, int x, int y, int alignment)
{
  PangoLayout* layout;
  PangoFontDescription* font_desc;

  cairo_set_source_rgba(cr, 0, 0, 0, 0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);
  
  layout = pango_cairo_create_layout(cr);
  
  pango_layout_set_alignment(layout, alignment);
  pango_layout_set_markup(layout, string, -1);

  int w = 0;
  int h = 0;

  pango_layout_get_pixel_size(layout, &w, &h);

  if (alignment == 1) {
    cairo_move_to(cr, x - w, y);
  } else if (alignment == 2) {
    cairo_move_to(cr, x - w / 2, y);
  } else {
    cairo_move_to(cr, x, y);
  }

  cairo_set_source_rgb(cr, 1, 1, 1);
  pango_cairo_show_layout(cr, layout);
  cairo_surface_flush(sfc);
}

void cairo_close(cairo_surface_t *sfc)
{
   Display *dsp = cairo_xlib_surface_get_display(sfc);

   cairo_surface_destroy(sfc);
   XCloseDisplay(dsp);
}

int main(int argc, char **argv)
{
   cairo_surface_t *sfc;
   cairo_t *cr;

   int x = atoi(argv[1]);
   int y = atoi(argv[2]);
   int a = (argc > 3) ? atoi(argv[3]) : 0;

   sfc = cairo_create_surface();
   cr = cairo_create(sfc);

   char string[255];

   while (fgets(string, sizeof(string), stdin))
   {
     cairo_draw_text(sfc, cr, string, x, y, a);
     event_loop(sfc, 0);
   }

   cairo_destroy(cr);
   cairo_close(sfc);

   return 0;
}
