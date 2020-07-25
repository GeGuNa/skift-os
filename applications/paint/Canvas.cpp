#include <libgraphic/Painter.h>

#include "paint/Canvas.h"
#include "paint/PaintTool.h"

Rectangle canvas_bound(Canvas *widget)
{
    Rectangle bound = bitmap_bound(widget->document->bitmap);
    bound = bound.offset(widget_get_bound(widget).position());
    bound = bound.centered_within(widget_get_bound(widget));
    return bound;
}

void canvas_paint(Canvas *widget, Painter *painter, Rectangle rectangle)
{
    __unused(rectangle);

    Rectangle destination = canvas_bound(widget);

    painter_fill_checkboard(painter, destination, 8, COLOR_WHITE, COLOR_GAINSBORO);
    painter_draw_rectangle(painter, destination, widget_get_color(widget, THEME_BORDER));

    painter_blit_bitmap(
        painter,
        widget->document->bitmap,
        bitmap_bound(widget->document->bitmap), destination);
}

void canvas_event(Canvas *widget, Event *event)
{
    if (is_mouse_event(event))
    {
        Event event_copy = *event;

        event_copy.mouse.old_position = event_copy.mouse.old_position - canvas_bound(widget).position();
        event_copy.mouse.position = event_copy.mouse.position - canvas_bound(widget).position();

        if (widget->document->tool->on_mouse_event)
            widget->document->tool->on_mouse_event(widget->document->tool, widget->document, event_copy);

        if (widget->document->dirty)
        {
            widget_update(WIDGET(widget));
            widget->document->dirty = false;
        }
    }
}

static const WidgetClass canvas_class = {
    .name = "Canvas",

    .paint = (WidgetPaintCallback)canvas_paint,
    .event = (WidgetEventCallback)canvas_event,
};

Widget *canvas_create(Widget *parent, PaintDocument *document)
{
    Canvas *canvas = __create(Canvas);

    canvas->document = document;

    widget_initialize(WIDGET(canvas), &canvas_class, parent);

    return WIDGET(canvas);
}