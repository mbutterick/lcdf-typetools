// -*- related-file-name: "../../libefont/t1bounds.cc" -*-
#ifndef EFONT_T1BOUNDS_HH
#define EFONT_T1BOUNDS_HH
#include <efont/t1interp.hh>
#include <lcdf/transform.hh>
namespace Efont {

class CharstringBounds : public CharstringInterp { public:

    CharstringBounds(const EfontProgram *, Vector<double> *weight = 0);
    ~CharstringBounds()				{ }

    const Transform &transform() const		{ return _xf; }
    void set_transform(const Transform &xf)	{ _xf = xf; }
    void transform(const Transform &);
    void translate(double, double);
    void extend(double);
    void shear(double);
    
    void init();
    void init(const Transform &);

    const Point &width() const			{ return _width; }
    double x_width() const			{ return _width.x; }
    double bb_left() const			{ return _lb.x; }
    double bb_top() const			{ return _rt.y; }
    double bb_right() const			{ return _rt.x; }
    double bb_bottom() const			{ return _lb.y; }

    void act_width(int, const Point &);
    void act_line(int, const Point &, const Point &);
    void act_curve(int, const Point &, const Point &, const Point &, const Point &);
    void mark(const Point &);

    bool bounds(int bb[4], int &width, bool use_cur_width = false) const;
    bool run(const Charstring &, int bounds[4], int &width);
    bool run_incr(const Charstring &);
    
  private:

    Point _lb;
    Point _rt;
    Point _width;
    Transform _xf;

    void xf_mark(const Point &);
    void xf_mark(const Bezier &);

    bool xf_inside(const Point &) const;
    bool xf_controls_inside(const Bezier &) const;
    
};

inline void
CharstringBounds::xf_mark(const Point &p)
{
    if (!KNOWN(_lb.x))
	_lb = _rt = p;
    if (p.x < _lb.x)
	_lb.x = p.x;
    else if (p.x > _rt.x)
	_rt.x = p.x;
    if (p.y < _lb.y)
	_lb.y = p.y;
    else if (p.y > _rt.y)
	_rt.y = p.y;
}

inline void
CharstringBounds::mark(const Point &p)
{
    xf_mark(p * _xf);
}

inline bool
CharstringBounds::xf_inside(const Point &p) const
{
    return p.x >= _lb.x && p.x <= _rt.x && p.y >= _lb.y && p.y <= _rt.y;
}

inline bool
CharstringBounds::xf_controls_inside(const Bezier &b) const
{
    return xf_inside(b.point(1)) && xf_inside(b.point(2));
}

}
#endif
