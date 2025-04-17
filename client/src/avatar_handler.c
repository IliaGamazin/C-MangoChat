#include "../inc/avatar_handler.h"

static GdkPixbuf* swap_red_blue_channels(GdkPixbuf *src_pixbuf) {
    int width = gdk_pixbuf_get_width(src_pixbuf);
    int height = gdk_pixbuf_get_height(src_pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(src_pixbuf);
    int n_channels = gdk_pixbuf_get_n_channels(src_pixbuf);

    if (n_channels != 3 && n_channels != 4) {
        g_warning("Pixbuf has an unsupported number of channels (%d).", n_channels);
        return src_pixbuf;
    }

    guint8 *pixels = gdk_pixbuf_get_pixels(src_pixbuf);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            guint8 *pixel = pixels + y * rowstride + x * n_channels; 

            if (n_channels == 4 && pixel[3] == 0) {
                continue;
            }

            guint8 temp = pixel[0];  
            pixel[0] = pixel[2];     
            pixel[2] = temp;         
        }
    }
    return src_pixbuf;
}

GdkPixbuf* create_rounded_pixbuf_from_memory(const guchar *file_data, gsize data_length, int dest_width, int dest_height) {
    GError *error = NULL;

    GInputStream *memory_stream = g_memory_input_stream_new_from_data(file_data, data_length, NULL);

    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_stream(memory_stream, NULL, &error);
    g_object_unref(memory_stream);

    if (!src_pixbuf) {
        g_warning("Failed to load image from memory: %s", error->message);
        g_clear_error(&error);
        return NULL;
    }

    int src_width = gdk_pixbuf_get_width(src_pixbuf);
    int src_height = gdk_pixbuf_get_height(src_pixbuf);
    int side_length = src_width < src_height ? src_width : src_height;
    int x_offset = (src_width - side_length) / 2;
    int y_offset = (src_height - side_length) / 2;

    GdkPixbuf *square_pixbuf = gdk_pixbuf_new_subpixbuf(src_pixbuf, x_offset, y_offset, side_length, side_length);
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(square_pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);

    g_object_unref(square_pixbuf);
    g_object_unref(src_pixbuf);

    if (!scaled_pixbuf) {
        g_warning("Failed to scale the image.");
        return NULL;
    }

    scaled_pixbuf = swap_red_blue_channels(scaled_pixbuf);

    GdkPixbuf *dest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, dest_width, dest_height);
    gdk_pixbuf_fill(dest_pixbuf, 0x00000000);
    cairo_surface_t *surface = cairo_image_surface_create_for_data(
        gdk_pixbuf_get_pixels(dest_pixbuf),
        CAIRO_FORMAT_ARGB32,
        dest_width,
        dest_height,
        gdk_pixbuf_get_rowstride(dest_pixbuf)
    );

    cairo_t *cr = cairo_create(surface);

    cairo_arc(cr, dest_width / 2, dest_height / 2, dest_width / 2, 0, 2 * M_PI);
    cairo_clip(cr);

    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    g_object_unref(scaled_pixbuf);

    return dest_pixbuf;
}

GdkPixbuf* create_rounded_pixbuf(gchar *selected_file, int dest_width, int dest_height) {
    GdkPixbuf *src_pixbuf = gdk_pixbuf_new_from_file(selected_file, NULL);

    if (!src_pixbuf) {
        g_warning("Failed to load image file.");
        return NULL;
    }

    int src_width = gdk_pixbuf_get_width(src_pixbuf);
    int src_height = gdk_pixbuf_get_height(src_pixbuf);
    int side_length = src_width < src_height ? src_width : src_height;
    int x_offset = (src_width - side_length) / 2;
    int y_offset = (src_height - side_length) / 2;

    GdkPixbuf *square_pixbuf = gdk_pixbuf_new_subpixbuf(src_pixbuf, x_offset, y_offset, side_length, side_length);
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(square_pixbuf, dest_width, dest_height, GDK_INTERP_BILINEAR);

    g_object_unref(square_pixbuf);
    g_object_unref(src_pixbuf);
    
    if (!scaled_pixbuf) {
        g_warning("Failed to scale the image.");
        return NULL;
    }

    scaled_pixbuf = swap_red_blue_channels(scaled_pixbuf);

    GdkPixbuf *dest_pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, dest_width, dest_height);
    gdk_pixbuf_fill(dest_pixbuf, 0x00000000); 
    cairo_surface_t *surface = cairo_image_surface_create_for_data(
        gdk_pixbuf_get_pixels(dest_pixbuf),
        CAIRO_FORMAT_ARGB32,
        dest_width,
        dest_height,
        gdk_pixbuf_get_rowstride(dest_pixbuf)
    );

    cairo_t *cr = cairo_create(surface);

    cairo_arc(cr, dest_width / 2, dest_height / 2, dest_width / 2, 0, 2 * M_PI);
    cairo_clip(cr);

    gdk_cairo_set_source_pixbuf(cr, scaled_pixbuf, 0, 0);
    cairo_paint(cr);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);

    g_object_unref(scaled_pixbuf);

    return dest_pixbuf;
}
