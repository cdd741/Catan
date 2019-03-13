#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <cmath>
#include "window.h"

using namespace std;

struct XwindowImpl
{
	GC gc;
	Window w;
	Display *d;
};

const double pi = std::acos(-1);

Xwindow::Xwindow(int width, int height): width{width}, height{height} {

	impl = new XwindowImpl;
	impl->d = XOpenDisplay(NULL);
	if (impl->d == NULL) {
		cerr << "Cannot open display" << endl;
		exit(1);
	}
	s = DefaultScreen(impl->d);
	impl->w = XCreateSimpleWindow(impl->d, RootWindow(impl->d, s), 10, 10, width, height, 1,
			BlackPixel(impl->d, s), WhitePixel(impl->d, s));
	XSelectInput(impl->d, impl->w, ExposureMask | KeyPressMask);
	XMapRaised(impl->d, impl->w);

	Pixmap pix = XCreatePixmap(impl->d,impl->w,width,
			height,DefaultDepth(impl->d,DefaultScreen(impl->d)));
	impl->gc = XCreateGC(impl->d, pix, 0,(XGCValues *)0);

	Font thefont = XLoadFont(impl->d, "*-*-*-*-*-*-*-*-*-*-*-*-*-*");
	XFlush(impl->d);
	XFlush(impl->d);


	// Set up colours.
	XColor xcolour;
	Colormap cmap;
	char color_vals[11][11] = {"white",  "black", "red",      "green",
		"blue",   "cyan",  "yellow",   "magenta",
		"orange", "brown", "darkgreen"};

	cmap=DefaultColormap(impl->d,DefaultScreen(impl->d));
	for(int i=0; i < 11; ++i) {
		XParseColor(impl->d,cmap,color_vals[i],&xcolour);
		XAllocColor(impl->d,cmap,&xcolour);
		colours[i]=xcolour.pixel;
	}

	XSetForeground(impl->d,impl->gc,colours[Black]);

	// Make window non-resizeable.
	XSizeHints hints;
	hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
	hints.height = hints.base_height = hints.min_height = hints.max_height = height;
	hints.width = hints.base_width = hints.min_width = hints.max_width = width;
	XSetNormalHints(impl->d, impl->w, &hints);

	XSynchronize(impl->d, True);

	XSelectInput(impl->d, impl->w, ExposureMask);
	XFlush(impl->d);
	XEvent event;
	XNextEvent(impl->d, &event); // Hang until the window is ready.
	XSelectInput(impl->d, impl->w, 0);

}

Xwindow::~Xwindow() {
	XFreeGC(impl->d, impl->gc);
	XCloseDisplay(impl->d);
	delete impl;
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
	XSetForeground(impl->d, impl->gc, colours[colour]);
	XFillRectangle(impl->d, impl->w, impl->gc, x, y, width, height);
	XSetForeground(impl->d, impl->gc, colours[Black]);
}

void Xwindow::fillPolygon(int x, int y, int vertices, int side, int rotate, int colour){
	XPoint* point = new XPoint[vertices];

	point[0] = XPoint{(short int)x, (short int)y};

	double angle = rotate + pi/2;

	for ( int i = 1; i < vertices; ++i ){
		short int X = side * sin(angle);
		short int Y = side * cos(angle);
		point[i] = XPoint{X,Y};
		angle += 2*pi - pi* 2/vertices;
	}

	XSetForeground(impl->d, impl->gc, colours[colour]);
	XFillPolygon(impl->d, impl->w, impl->gc, point, vertices, Convex, CoordModePrevious);
	XSetForeground(impl->d, impl->gc, colours[Black]);

	delete[] point;
}

void Xwindow::drawLine(int x1, int y1, int x2, int y2) {
	    XDrawLine(impl->d, impl->w, impl->gc, x1, y1, x2, y2);
}

void Xwindow::drawArc(int x1, int y1, int width, int height, int sAngle,
                      int eAngle) {
	    XDrawArc(impl->d, impl->w, impl->gc, x1, y1, width/2, height/2, sAngle * 64, eAngle * 64);
}

void Xwindow::fillArc(int x, int y, int width, int height, int angle1,
                      int angle2, int colour) {
	    XSetForeground(impl->d, impl->gc, colours[colour]);
			    XFillArc(impl->d, impl->w, impl->gc, x-width/2, y-height/2, width, height, angle1 * 64, angle2 * 64);
					    XSetForeground(impl->d, impl->gc, colours[Black]);
}

void Xwindow::fillCircle(int x, int y, int di, int colour) {
	    fillArc(x, y, di, di, 0, 360, colour);
}

void Xwindow::drawString(int x, int y, string msg, int colour) {
  XFontStruct * f = XLoadQueryFont(impl->d, "6x13");
	
	XSetForeground(impl->d, impl->gc, colours[colour]);
	XTextItem ti;
	ti.chars = const_cast<char*>(msg.c_str());
	ti.nchars = msg.length();
	ti.delta = 0;
	ti.font = f->fid;
	XDrawText(impl->d, impl->w, impl->gc, x, y, &ti, 1);
	XSetForeground(impl->d, impl->gc, colours[Black]);
	XFlush(impl->d);

	delete f;
}


void Xwindow::drawStringFont(int x, int y, string msg, string font, int colour) {
  XFontStruct * f = XLoadQueryFont(impl->d, font.c_str());
  
	if ( f == nullptr ){
		f = XLoadQueryFont(impl->d, "6x13");
	}

	XSetForeground(impl->d, impl->gc, colours[colour]);
	XTextItem ti;
	ti.chars = const_cast<char*>(msg.c_str());
	ti.nchars = msg.length();
	ti.delta = 0;
	ti.font = f->fid;
	XDrawText(impl->d, impl->w, impl->gc, x, y, &ti, 1);
	XSetForeground(impl->d, impl->gc, colours[Black]);
	XFlush(impl->d);

	delete f;
}

void Xwindow::drawBigString(int x, int y, string msg, int colour) {
  // Font f = XLoadFont(impl->d, "-*-helvetica-bold-r-normal--*-240-*-*-*-*-*");
  ostringstream name;
  name << "-*-helvetica-bold-r-*-*-*-240-" << width/5 << "-" << height/5 << "-*-*-*-*";

	drawStringFont(x, y, msg, name.str(), colour);
}

void Xwindow::showAvailableFonts() {
  int count;
  char** fnts = XListFonts(impl->d, "*", 10000, &count);

  for (int i = 0; i < count; ++i) cout << fnts[i] << endl;
}
