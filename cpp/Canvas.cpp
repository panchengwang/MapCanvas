#include "Canvas.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <iostream>

using namespace geos::io;
using namespace geos::geom;

MapCanvas::MapCanvas()
{

    _rotate = 0;
    _dotsPerMM = 72.0 / 25.4;

    _width = 100;
    _height = 100;
    _format = "png";
    _scale = 1.0;

    _centerX = 0.0;
    _centerY = 0.0;

    _surface = NULL;
    _cairo = NULL;


    _factory = GeometryFactory::create();
    _wktReader = geos::io::WKTReader(*_factory);

    _defaultPointSymbol.fromJsonString("{ \"size\": 20.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [  { \"type\": \"path\", \"stroke\": { \"cap\": \"butt\", \"join\": \"miter\", \"miter\": 10.0, \"color\": { \"alpha\": 255, \"red\": 100, \"green\": 100, \"blue\": 100 }, \"width\": 0.5, \"dashoffset\": 0.0, \"dashes\": [ 1.0, 0.0 ] }, \"fill\": { \"type\": \"solid\", \"color\": { \"alpha\": 255, \"red\": 255, \"green\": 255, \"blue\": 0 } }, \"subpaths\": [ { \"type\": \"circle\", \"center\": { \"x\": 0.0, \"y\": 0.0 }, \"radius\": 0.80000000000000004 } ] } ] }");
    // _defaultPointSymbol.fromJsonString("{ \"size\": 20.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"path\", \"stroke\": { \"cap\": \"butt\", \"join\": \"miter\", \"miter\": 10.0, \"color\": { \"alpha\": 255, \"red\": 255, \"green\": 0, \"blue\": 0 }, \"width\": 0.20000000000000001, \"dashoffset\": 0.0, \"dashes\": [ 1.0, 0.0 ] }, \"fill\": { \"type\": \"solid\", \"color\": { \"alpha\": 127, \"red\": 255, \"green\": 255, \"blue\": 0 } }, \"subpaths\": [ { \"type\": \"regularpolygon\", \"center\": { \"x\": 0.0, \"y\": 0.0 }, \"radius\": 0.79000000000000004, \"rotate\": 0.0, \"numedges\": 16 } ] } ] }");
    _defaultLineSymbol.fromJsonString("{ \"size\": 50.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"systemline\", \"stroke\": { \"cap\": \"butt\", \"join\": \"miter\", \"miter\": 10.0, \"color\": { \"alpha\": 255, \"red\": 0, \"green\": 0, \"blue\": 0 }, \"width\": 0.5, \"dashoffset\": 0.0, \"dashes\": [ 3.0, 1.0, 1.0, 1.0 ] } } ] }");
    _defaultFillSymbol.fromJsonString("{ \"size\": 5.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"systemfill\", \"fill\": { \"type\": \"solid\", \"color\": { \"alpha\": 255, \"red\": 255, \"green\": 0, \"blue\": 0 } } } ] }");
    _defaultPointSymbolSurface = NULL;
    _defaultPointSymbolImage = NULL;
}

MapCanvas::MapCanvas(double width, double height, const char* format)
{
    _rotate = 0;
    _dotsPerMM = 72.0 / 25.4;

    _width = width;
    _height = height;
    _format = format;
    _scale = 1.0;

    _centerX = 0.0;
    _centerY = 0.0;

    _surface = NULL;
    _cairo = NULL;


    _factory = GeometryFactory::create();
    _wktReader = geos::io::WKTReader(*_factory);

    _defaultPointSymbol.fromJsonString("{ \"size\": 5.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"path\", \"stroke\": { \"cap\": \"butt\", \"join\": \"miter\", \"miter\": 10.0, \"color\": { \"alpha\": 255, \"red\": 0, \"green\": 0, \"blue\": 0 }, \"width\": 0.29999999999999999, \"dashoffset\": 0.0, \"dashes\": [ 1.0, 0.0 ] }, \"fill\": { \"type\": \"solid\", \"color\": { \"alpha\": 127, \"red\": 255, \"green\": 255, \"blue\": 0 } }, \"subpaths\": [ { \"type\": \"circle\", \"center\": { \"x\": 0.0, \"y\": 0.0 }, \"radius\": 0.80000000000000004 } ] } ] }");
    _defaultLineSymbol.fromJsonString("{ \"size\": 50.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"systemline\", \"stroke\": { \"cap\": \"butt\", \"join\": \"miter\", \"miter\": 10.0, \"color\": { \"alpha\": 255, \"red\": 0, \"green\": 0, \"blue\": 0 }, \"width\": 0.5, \"dashoffset\": 0.0, \"dashes\": [ 3.0, 1.0, 1.0, 1.0 ] } } ] }");
    _defaultFillSymbol.fromJsonString("{ \"size\": 5.0, \"offset\": { \"x\": 0.0, \"y\": 0.0 }, \"shapes\": [ { \"type\": \"systemfill\", \"fill\": { \"type\": \"solid\", \"color\": { \"alpha\": 255, \"red\": 255, \"green\": 0, \"blue\": 0 } } } ] }");

    _defaultPointSymbolSurface = NULL;
    _defaultPointSymbolImage = NULL;
}

MapCanvas::~MapCanvas()
{
    clear();
}

void MapCanvas::begin()
{
    clear();

    if (_format == "png" || _format == "jpg" || _format == "jpeg") {
        _surface = cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32,
            ceil(_width * _dotsPerMM),
            ceil(_height * _dotsPerMM)
        );
    }
    else {

    }

    if (_surface) {
        _cairo = cairo_create(_surface);
        cairo_set_source_rgba(_cairo,1,1,1,0);
        cairo_paint(_cairo);

        cairo_translate(_cairo, ceil(_width*_dotsPerMM) * 0.5, ceil(_height*_dotsPerMM)*0.5);
        cairo_scale(_cairo, _dotsPerMM, _dotsPerMM);
        cairo_scale(_cairo, _scale, _scale);
        cairo_scale(_cairo, 1, -1);
        cairo_translate(_cairo, -_centerX, -_centerY);

    }
}

void MapCanvas::end()
{
    cairo_surface_flush(_surface);
}


void MapCanvas::clear()
{
    if(_defaultPointSymbolSurface ){
        cairo_surface_destroy(_defaultPointSymbolSurface);
        _defaultPointSymbolSurface = NULL;
    }

    if(_defaultPointSymbolImage){
        delete _defaultPointSymbolImage;
        _defaultPointSymbolImage = NULL;
    }

    if (_cairo) {
        cairo_destroy(_cairo);
        _cairo = NULL;
    }

    if (_surface) {
        cairo_surface_destroy(_surface);
        _surface = NULL;
    }
}



typedef struct
{
    unsigned char* data;
    size_t len;
} DataBuffer;

static cairo_status_t
_write_image(void* closure,
    const unsigned char* data,
    unsigned int length) {
    DataBuffer* buffer = (DataBuffer*)closure;
    buffer->data = (unsigned char*)realloc(buffer->data, buffer->len + length);
    memcpy(buffer->data + buffer->len, data, length);
    buffer->len += length;
    return CAIRO_STATUS_SUCCESS;
}

unsigned char* MapCanvas::data(size_t& len)
{
    DataBuffer buffer;
    buffer.data = NULL;
    buffer.len = 0;

    cairo_surface_write_to_png_stream(_surface, _write_image, &buffer);

    len = buffer.len;
    return buffer.data;
}



bool MapCanvas::saveToFile(const char* filename)
{
    size_t len;
    unsigned char* buf = data(len);
    if (!buf) {
        _errorMessage = "Error: image is empty.";
        return false;
    }
    FILE* fd = fopen(filename, "wb");
    if (!fd) {
        _errorMessage = std::string("Can not create file : ") + filename;
        return false;
    }
    fwrite(buf, 1, len, fd);
    fclose(fd);
    free(buf);
    return true;
}

std::string MapCanvas::errorMessage() const
{
    return _errorMessage;
}

double MapCanvas::width() const
{
    return _width;
}

void MapCanvas::setWidth(double newWidth)
{
    _width = newWidth;
}

double MapCanvas::height() const
{
    return _height;
}

void MapCanvas::setHeight(double newHeight)
{
    _height = newHeight;
}

double MapCanvas::rotate() const
{
    return _rotate;
}

void MapCanvas::setRotate(double newRotate)
{
    _rotate = newRotate;
}

double MapCanvas::dotsPerMM() const
{
    return _dotsPerMM;
}

void MapCanvas::setDotsPerMM(double dotsPerMM)
{
    _dotsPerMM = dotsPerMM;
}

std::string MapCanvas::format() const
{
    return _format;
}

void MapCanvas::setFormat(const std::string& format)
{
    _format = format;
}

double MapCanvas::scale() const
{
    return _scale;
}

void MapCanvas::setScale(double newScale)
{
    _scale = newScale;
}

double MapCanvas::centerX() const
{
    return _centerX;
}

void MapCanvas::setCenterX(double newCenterX)
{
    _centerX = newCenterX;
}

double MapCanvas::centerY() const
{
    return _centerY;
}

void MapCanvas::setCenterY(double newCenterY)
{
    _centerY = newCenterY;
}

void MapCanvas::setCenter(double cx, double cy)
{
    _centerX = cx;
    _centerY = cy;
}

void MapCanvas::draw(const char* wkt)
{
    std::unique_ptr<Geometry> g =  _wktReader.read(wkt);

    draw(*g);
}

void MapCanvas::draw(Geometry& geo)
{

    if(geo.getGeometryTypeId() == GEOS_POINT){
        draw((Point&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_LINESTRING){
        draw((LineString&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_POLYGON){
        draw((Polygon&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_MULTIPOINT){
        draw((MultiPoint&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_MULTILINESTRING){
        draw((MultiLineString&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_MULTIPOLYGON){
        draw((MultiPolygon&)geo);
    }else if(geo.getGeometryTypeId() == GEOS_GEOMETRYCOLLECTION){
        draw((GeometryCollection&)geo);
    }
}

void MapCanvas::draw(Point& pt)
{

    if(!_defaultPointSymbolImage){
        _defaultPointSymbolImage = _defaultPointSymbol.toImage("png", _dotsPerMM);
        _defaultPointSymbolSurface = _defaultPointSymbolImage->createSurface();
    }

    SImage *img = _defaultPointSymbolImage;
    cairo_save(_cairo);
    cairo_move_to(_cairo,pt.getX(),pt.getY());
    cairo_translate(_cairo,-img->width()*0.5, -img->height()*0.5);
    cairo_scale(_cairo, 1.0/img->dotsPerMM(),1.0/img->dotsPerMM());
    cairo_set_source_surface(_cairo,_defaultPointSymbolSurface,0,0);
    cairo_paint(_cairo);
    cairo_restore(_cairo);

    // cairo_surface_destroy(_defaultPointSymbolSurface);
    // _defaultPointSymbolSurface = NULL;
    // delete _defaultPointSymbolImage;
    // _defaultPointSymbolImage = NULL;


}

void MapCanvas::draw(LineString& ls)
{
    cairo_save(_cairo);
    std::unique_ptr<Point> pt = ls.getPointN(0);
    cairo_move_to(_cairo,pt->getX(), pt->getY());
    for(size_t i=0; i<ls.getNumPoints(); i++){
        pt = ls.getPointN(i);
        cairo_line_to(_cairo, pt->getX(),pt->getY());
    }
    cairo_set_source_rgba(_cairo,0,0,0,1);
    cairo_stroke(_cairo);
    cairo_restore(_cairo);
}

void MapCanvas::draw(Polygon& pg)
{

}

void MapCanvas::draw(MultiPoint& mpt)
{

}

void MapCanvas::draw(MultiLineString& mls)
{

}

void MapCanvas::draw(MultiPolygon& mpg)
{

}

void MapCanvas::draw(GeometryCollection& cl)
{

}

void MapCanvas::drawGrid()
{

}


