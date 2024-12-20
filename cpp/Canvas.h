#ifndef __MAP_CANVAS_H
#define __MAP_CANVAS_H


#include <mapsymbol/DLLExport.h>
#include <cairo.h>
#include <stdint.h>
#include <string>
#include <geos.h>
#include <mapsymbol/SSymbol.h>

using namespace geos::io;
using namespace  geos::geom;


class DLL_EXPORT MapCanvas {
public:
    MapCanvas();
    MapCanvas(double width, double height, const char* format);
    virtual ~MapCanvas();

    void begin();
    void end();
    void clear();

    unsigned char* data(size_t& len);
    bool saveToFile(const char* filename);

    std::string errorMessage() const;

    double width() const;
    void setWidth(double newWidth);

    double height() const;
    void setHeight(double newHeight);

    double rotate() const;
    void setRotate(double newRotate);

    double dotsPerMM() const;
    void setDotsPerMM(double newDotsPerMM);

    std::string format() const;
    void setFormat(const std::string& newFormat);

    double scale() const;
    void setScale(double newScale);

    double centerX() const;
    void setCenterX(double centerX);

    double centerY() const;
    void setCenterY(double centerY);

    void setCenter(double cx, double cy);



    void draw(const char* wkt);
    void draw(Geometry& geo);
    void draw(Point& pt);
    void draw(LineString& ls);
    void draw(Polygon& pg);
    void draw(MultiPoint& mpt);
    void draw(MultiLineString& mls);
    void draw(MultiPolygon& mpg);
    void draw(GeometryCollection& cl);

    unsigned char* imageData(size_t& len) ;

protected:
    void drawGrid();

protected:
    double _width;
    double _height;
    double _rotate;
    double _dotsPerMM;
    double _scale;
    std::string _format;
    double _centerX;
    double _centerY;

    cairo_surface_t *_surface;
    cairo_t *_cairo;
    std::string _errorMessage;

    GeometryFactory::Ptr _factory;
    geos::io::WKTReader _wktReader;

    SSymbol _defaultPointSymbol;
    SSymbol _defaultLineSymbol;
    SSymbol _defaultFillSymbol;

    cairo_surface_t* _defaultPointSymbolSurface;
    SImage* _defaultPointSymbolImage;

};


#endif
