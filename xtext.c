#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cairo.h>
#include <cairo-xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

cairo_surface_t *cairo_create_surface(int x, int y)
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
   
   int width = 1920;
   int height = 1080;
   
   da = 
     XCreateWindow (dsp,
                    DefaultRootWindow(dsp),
                    x, y,
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

   return sfc;
}

void cairo_close(cairo_surface_t *sfc)
{
   Display *dsp = cairo_xlib_surface_get_display(sfc);

   cairo_surface_destroy(sfc);
   XCloseDisplay(dsp);
}

void input(char *string,int length)
{
  int x = 0;
  
  fgets(string,length,stdin);
  while(*string)
    {
      if(*string == '\n')
        {
          *string = '\0';
          return;
        }
      x++;
      if(x == length)
        {
          *string = '\0';
          return;
        }
      string++;
    }
}

int main(int argc, char **argv)
{
   cairo_surface_t *sfc;
   cairo_t *ctx;

   sfc = cairo_create_surface(0, 0);
   ctx = cairo_create(sfc);

   char line[255];

   while (1)
   {

     // READ
     input(line, sizeof(line));

     // CLEAR
     cairo_set_source_rgba (ctx, 0, 0, 0, 0);
     cairo_set_operator (ctx, CAIRO_OPERATOR_SOURCE);
     cairo_paint (ctx);

     // DRAW
     cairo_set_source_rgb(ctx, 1, 1, 1);
     cairo_select_font_face(ctx, "Hasklug Nerd Font",
                            CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_BOLD);
     cairo_set_font_size(ctx, 14);
     cairo_move_to(ctx, 48, 28);
     cairo_show_text(ctx, line);
     cairo_stroke(ctx);
     cairo_surface_flush(sfc);

     event_loop(sfc, 0);

   }

   cairo_destroy(ctx);
   cairo_close(sfc);

   return 0;
}
