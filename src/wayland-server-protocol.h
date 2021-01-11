/* 
 * Copyright © 2008-2011 Kristian Høgsberg
 * Copyright © 2010-2011 Intel Corporation
 * 
 * Permission to use, copy, modify, distribute, and sell this
 * software and its documentation for any purpose is hereby granted
 * without fee, provided that the above copyright notice appear in
 * all copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * the copyright holders not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.  The copyright holders make no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied
 * warranty.
 * 
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

#ifndef WAYLAND_SERVER_PROTOCOL_H
#define WAYLAND_SERVER_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-util.h"

struct wl_client;
struct wl_resource;

struct wl_display;
struct wl_callback;
struct wl_compositor;
struct wl_shm_pool;
struct wl_shm;
struct wl_buffer;
struct wl_data_offer;
struct wl_data_source;
struct wl_data_device;
struct wl_data_device_manager;
struct wl_shell;
struct wl_shell_surface;
struct wl_surface;
struct wl_seat;
struct wl_pointer;
struct wl_keyboard;
struct wl_touch;
struct wl_output;
struct wl_region;

extern const struct wl_interface wl_display_interface;
extern const struct wl_interface wl_callback_interface;
extern const struct wl_interface wl_compositor_interface;
extern const struct wl_interface wl_shm_pool_interface;
extern const struct wl_interface wl_shm_interface;
extern const struct wl_interface wl_buffer_interface;
extern const struct wl_interface wl_data_offer_interface;
extern const struct wl_interface wl_data_source_interface;
extern const struct wl_interface wl_data_device_interface;
extern const struct wl_interface wl_data_device_manager_interface;
extern const struct wl_interface wl_shell_interface;
extern const struct wl_interface wl_shell_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_pointer_interface;
extern const struct wl_interface wl_keyboard_interface;
extern const struct wl_interface wl_touch_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_region_interface;

#ifndef WL_DISPLAY_ERROR_ENUM
#define WL_DISPLAY_ERROR_ENUM
/**
 * wl_display_error - global error values
 * @WL_DISPLAY_ERROR_INVALID_OBJECT: server couldn't find object
 * @WL_DISPLAY_ERROR_INVALID_METHOD: method doesn't exist on the
 *	specified interface
 * @WL_DISPLAY_ERROR_NO_MEMORY: server is out of memory
 *
 * These errors are global and can be emitted in response to any server
 * request.
 */
enum wl_display_error {
	WL_DISPLAY_ERROR_INVALID_OBJECT = 0,
	WL_DISPLAY_ERROR_INVALID_METHOD = 1,
	WL_DISPLAY_ERROR_NO_MEMORY = 2,
};
#endif /* WL_DISPLAY_ERROR_ENUM */

/**
 * wl_display - core global object
 * @bind: bind an object to the display
 * @sync: asynchronous roundtrip
 *
 * The core global object. This is a special singleton object. It is used
 * for internal wayland protocol features.
 */
struct wl_display_interface {
	/**
	 * bind - bind an object to the display
	 * @name: unique number id for object
	 * @interface: (none)
	 * @version: (none)
	 * @id: (none)
	 *
	 * Binds a new, client-created object to the server using @name
	 * as the identifier.
	 */
	void (*bind)(struct wl_client *client,
		     struct wl_resource *resource,
		     uint32_t name,
		     const char *interface,
		     uint32_t version,
		     uint32_t id);
	/**
	 * sync - asynchronous roundtrip
	 * @callback: (none)
	 *
	 * The sync request asks the server to invoke the 'done' request
	 * on the provided wl_callback object. Since requests are handled
	 * in-order, this can be used as a barrier to ensure all previous
	 * requests have been handled.
	 */
	void (*sync)(struct wl_client *client,
		     struct wl_resource *resource,
		     uint32_t callback);
};

#define WL_DISPLAY_ERROR	0
#define WL_DISPLAY_GLOBAL	1
#define WL_DISPLAY_GLOBAL_REMOVE	2
#define WL_DISPLAY_DELETE_ID	3

#define WL_CALLBACK_DONE	0

static inline void
wl_callback_send_done(struct wl_resource *resource_, uint32_t serial)
{
	wl_resource_post_event(resource_, WL_CALLBACK_DONE, serial);
}

/**
 * wl_compositor - the compositor singleton
 * @create_surface: create new surface
 * @create_region: create new region
 *
 * A compositor. This object is a singleton global. The compositor is in
 * charge of combining the contents of multiple surfaces into one
 * displayable output.
 */
struct wl_compositor_interface {
	/**
	 * create_surface - create new surface
	 * @id: (none)
	 *
	 * Ask the compositor to create a new surface.
	 */
	void (*create_surface)(struct wl_client *client,
			       struct wl_resource *resource,
			       uint32_t id);
	/**
	 * create_region - create new region
	 * @id: (none)
	 *
	 * Ask the compositor to create a new region.
	 */
	void (*create_region)(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t id);
};

/**
 * wl_shm_pool - a shared memory pool
 * @create_buffer: create wl_buffer from pool
 * @destroy: destroy the pool
 * @resize: change the size of the pool mapping
 *
 * The wl_shm_pool object encapsulates a piece of memory shared between
 * the compsitor and client. Through the wl_shm_pool object, the client can
 * allocate shared memory wl_buffer objects. The objects will share the
 * same underlying mapped memory. Reusing the mapped memory avoids the
 * setup/teardown overhead and is useful when interactively resizing a
 * surface or for many small buffers.
 */
struct wl_shm_pool_interface {
	/**
	 * create_buffer - create wl_buffer from pool
	 * @id: (none)
	 * @offset: (none)
	 * @width: (none)
	 * @height: (none)
	 * @stride: (none)
	 * @format: (none)
	 *
	 * Create a wl_buffer from the pool. The buffer is created a
	 * offset bytes into the pool and has width and height as
	 * specified. The stride arguments specifies the number of bytes
	 * from beginning of one row to the beginning of the next. The
	 * format is the pixel format of the buffer and must be one of
	 * those advertised through the wl_shm.format event. A buffer will
	 * keep a reference to the pool it was created from so it is valid
	 * to destroy the pool immediatedly after creating a buffer from
	 * it.
	 */
	void (*create_buffer)(struct wl_client *client,
			      struct wl_resource *resource,
			      uint32_t id,
			      int32_t offset,
			      int32_t width,
			      int32_t height,
			      int32_t stride,
			      uint32_t format);
	/**
	 * destroy - destroy the pool
	 *
	 * Destroy the pool.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * resize - change the size of the pool mapping
	 * @size: (none)
	 *
	 * This request will cause the server to remap the backing memory
	 * for the pool from the fd passed when the pool was creating but
	 * using the new size.
	 */
	void (*resize)(struct wl_client *client,
		       struct wl_resource *resource,
		       int32_t size);
};

#ifndef WL_SHM_ERROR_ENUM
#define WL_SHM_ERROR_ENUM
enum wl_shm_error {
	WL_SHM_ERROR_INVALID_FORMAT = 0,
	WL_SHM_ERROR_INVALID_STRIDE = 1,
	WL_SHM_ERROR_INVALID_FD = 2,
};
#endif /* WL_SHM_ERROR_ENUM */

#ifndef WL_SHM_FORMAT_ENUM
#define WL_SHM_FORMAT_ENUM
enum wl_shm_format {
	WL_SHM_FORMAT_ARGB8888 = 0,
	WL_SHM_FORMAT_XRGB8888 = 1,
};
#endif /* WL_SHM_FORMAT_ENUM */

/**
 * wl_shm - shared memory support
 * @create_pool: create a shm pool
 *
 * Support for shared memory buffers.
 */
struct wl_shm_interface {
	/**
	 * create_pool - create a shm pool
	 * @id: (none)
	 * @fd: (none)
	 * @size: (none)
	 *
	 * This creates wl_shm_pool object, which can be used to create
	 * shared memory based wl_buffer objects. The server will mmap size
	 * bytes of the passed fd, to use as backing memory for then pool.
	 */
	void (*create_pool)(struct wl_client *client,
			    struct wl_resource *resource,
			    uint32_t id,
			    int32_t fd,
			    int32_t size);
};

#define WL_SHM_FORMAT	0

static inline void
wl_shm_send_format(struct wl_resource *resource_, uint32_t format)
{
	wl_resource_post_event(resource_, WL_SHM_FORMAT, format);
}

/**
 * wl_buffer - content for a wl_surface
 * @destroy: destroy a buffer
 *
 * A buffer provides the content for a wl_surface. Buffers are created
 * through factory interfaces such as wl_drm, wl_shm or similar. It has a
 * width and a height and can be attached to a wl_surface, but the
 * mechanism by which a client provides and updates the contents is defined
 * by the buffer factory interface
 */
struct wl_buffer_interface {
	/**
	 * destroy - destroy a buffer
	 *
	 * Destroy a buffer. This will invalidate the object id.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
};

#define WL_BUFFER_RELEASE	0

static inline void
wl_buffer_send_release(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_BUFFER_RELEASE);
}

struct wl_data_offer_interface {
	/**
	 * accept - accept one of the offered mime-types
	 * @serial: (none)
	 * @type: (none)
	 *
	 * Indicate that the client can accept the given mime-type, or
	 * NULL for not accepted. Use for feedback during drag and drop.
	 */
	void (*accept)(struct wl_client *client,
		       struct wl_resource *resource,
		       uint32_t serial,
		       const char *type);
	/**
	 * receive - (none)
	 * @mime_type: (none)
	 * @fd: (none)
	 */
	void (*receive)(struct wl_client *client,
			struct wl_resource *resource,
			const char *mime_type,
			int32_t fd);
	/**
	 * destroy - (none)
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
};

#define WL_DATA_OFFER_OFFER	0

static inline void
wl_data_offer_send_offer(struct wl_resource *resource_, const char *type)
{
	wl_resource_post_event(resource_, WL_DATA_OFFER_OFFER, type);
}

struct wl_data_source_interface {
	/**
	 * offer - add an offered mime type
	 * @type: (none)
	 *
	 * This request adds a mime-type to the set of mime-types
	 * advertised to targets. Can be called several times to offer
	 * multiple types.
	 */
	void (*offer)(struct wl_client *client,
		      struct wl_resource *resource,
		      const char *type);
	/**
	 * destroy - destroy the data source
	 *
	 * Destroy the data source.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
};

#define WL_DATA_SOURCE_TARGET	0
#define WL_DATA_SOURCE_SEND	1
#define WL_DATA_SOURCE_CANCELLED	2

static inline void
wl_data_source_send_target(struct wl_resource *resource_, const char *mime_type)
{
	wl_resource_post_event(resource_, WL_DATA_SOURCE_TARGET, mime_type);
}

static inline void
wl_data_source_send_send(struct wl_resource *resource_, const char *mime_type, int32_t fd)
{
	wl_resource_post_event(resource_, WL_DATA_SOURCE_SEND, mime_type, fd);
}

static inline void
wl_data_source_send_cancelled(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_DATA_SOURCE_CANCELLED);
}

struct wl_data_device_interface {
	/**
	 * start_drag - start drag and drop operation
	 * @source: (none)
	 * @origin: (none)
	 * @icon: (none)
	 * @serial: (none)
	 *
	 * This request asks the compositor to start a drag and drop
	 * operation on behalf of the client. The source argument is the
	 * data source that provides the data for the eventual data
	 * transfer. If source is NULL, enter, leave and motion events are
	 * sent only to the client that initiated the drag and the client
	 * is expected to handle the data passing internally. The origin
	 * surface is the surface where the drag originates and the client
	 * must have an active implicit grab that matches the serial. The
	 * icon surface is an optional (can be nil) surface that provides
	 * an icon to be moved around with the cursor. Initially, the
	 * top-left corner of the icon surface is placed at the cursor
	 * hotspot, but subsequent surface.attach request can move the
	 * relative position.
	 */
	void (*start_drag)(struct wl_client *client,
			   struct wl_resource *resource,
			   struct wl_resource *source,
			   struct wl_resource *origin,
			   struct wl_resource *icon,
			   uint32_t serial);
	/**
	 * set_selection - (none)
	 * @source: (none)
	 * @serial: (none)
	 */
	void (*set_selection)(struct wl_client *client,
			      struct wl_resource *resource,
			      struct wl_resource *source,
			      uint32_t serial);
};

#define WL_DATA_DEVICE_DATA_OFFER	0
#define WL_DATA_DEVICE_ENTER	1
#define WL_DATA_DEVICE_LEAVE	2
#define WL_DATA_DEVICE_MOTION	3
#define WL_DATA_DEVICE_DROP	4
#define WL_DATA_DEVICE_SELECTION	5

static inline void
wl_data_device_send_data_offer(struct wl_resource *resource_, struct wl_resource *id)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_DATA_OFFER, id);
}

static inline void
wl_data_device_send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, wl_fixed_t x, wl_fixed_t y, struct wl_resource *id)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_ENTER, serial, surface, x, y, id);
}

static inline void
wl_data_device_send_leave(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_LEAVE);
}

static inline void
wl_data_device_send_motion(struct wl_resource *resource_, uint32_t time, wl_fixed_t x, wl_fixed_t y)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_MOTION, time, x, y);
}

static inline void
wl_data_device_send_drop(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_DROP);
}

static inline void
wl_data_device_send_selection(struct wl_resource *resource_, struct wl_resource *id)
{
	wl_resource_post_event(resource_, WL_DATA_DEVICE_SELECTION, id);
}

struct wl_data_device_manager_interface {
	/**
	 * create_data_source - (none)
	 * @id: (none)
	 */
	void (*create_data_source)(struct wl_client *client,
				   struct wl_resource *resource,
				   uint32_t id);
	/**
	 * get_data_device - (none)
	 * @id: (none)
	 * @seat: (none)
	 */
	void (*get_data_device)(struct wl_client *client,
				struct wl_resource *resource,
				uint32_t id,
				struct wl_resource *seat);
};

struct wl_shell_interface {
	/**
	 * get_shell_surface - (none)
	 * @id: (none)
	 * @surface: (none)
	 */
	void (*get_shell_surface)(struct wl_client *client,
				  struct wl_resource *resource,
				  uint32_t id,
				  struct wl_resource *surface);
};

#ifndef WL_SHELL_SURFACE_RESIZE_ENUM
#define WL_SHELL_SURFACE_RESIZE_ENUM
enum wl_shell_surface_resize {
	WL_SHELL_SURFACE_RESIZE_NONE = 0,
	WL_SHELL_SURFACE_RESIZE_TOP = 1,
	WL_SHELL_SURFACE_RESIZE_BOTTOM = 2,
	WL_SHELL_SURFACE_RESIZE_LEFT = 4,
	WL_SHELL_SURFACE_RESIZE_TOP_LEFT = 5,
	WL_SHELL_SURFACE_RESIZE_BOTTOM_LEFT = 6,
	WL_SHELL_SURFACE_RESIZE_RIGHT = 8,
	WL_SHELL_SURFACE_RESIZE_TOP_RIGHT = 9,
	WL_SHELL_SURFACE_RESIZE_BOTTOM_RIGHT = 10,
};
#endif /* WL_SHELL_SURFACE_RESIZE_ENUM */

#ifndef WL_SHELL_SURFACE_TRANSIENT_ENUM
#define WL_SHELL_SURFACE_TRANSIENT_ENUM
enum wl_shell_surface_transient {
	WL_SHELL_SURFACE_TRANSIENT_INACTIVE = 0x1,
};
#endif /* WL_SHELL_SURFACE_TRANSIENT_ENUM */

#ifndef WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM
#define WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM
/**
 * wl_shell_surface_fullscreen_method - different method to set the
 *	surface fullscreen
 * @WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT: (none)
 * @WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE: (none)
 * @WL_SHELL_SURFACE_FULLSCREEN_METHOD_DRIVER: (none)
 * @WL_SHELL_SURFACE_FULLSCREEN_METHOD_FILL: (none)
 *
 * Hints to indicate compositor how to deal with a conflict between the
 * dimensions for the surface and the dimensions of the output. As a hint
 * the compositor is free to ignore this parameter. "default" The client
 * has no preference on fullscreen behavior, policies are determined by
 * compositor. "scale" The client prefers scaling by the compositor.
 * Scaling would always preserve surface's aspect ratio with surface
 * centered on the output "driver" The client wants to switch video mode to
 * the smallest mode that can fit the client buffer. If the sizes do not
 * match the compositor must add black borders. "fill" The surface is
 * centered on the output on the screen with no scaling. If the surface is
 * of insufficient size the compositor must add black borders.
 */
enum wl_shell_surface_fullscreen_method {
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT = 0,
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE = 1,
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_DRIVER = 2,
	WL_SHELL_SURFACE_FULLSCREEN_METHOD_FILL = 3,
};
#endif /* WL_SHELL_SURFACE_FULLSCREEN_METHOD_ENUM */

/**
 * wl_shell_surface - desktop style meta data interface
 * @pong: respond to a ping event
 * @move: (none)
 * @resize: (none)
 * @set_toplevel: make the surface a top level surface
 * @set_transient: make the surface a transient surface
 * @set_fullscreen: make the surface a fullscreen surface
 * @set_popup: make the surface a popup surface
 * @set_maximized: make the surface a maximized surface
 * @set_title: set surface title
 * @set_class: set surface class
 *
 * An interface implemented by a wl_surface. On server side the object is
 * automatically destroyed when the related wl_surface is destroyed. On
 * client side, wl_shell_surface_destroy() must be called before destroying
 * the wl_surface object.
 */
struct wl_shell_surface_interface {
	/**
	 * pong - respond to a ping event
	 * @serial: (none)
	 *
	 * A client must respond to a ping event with a pong request or
	 * the client may be deemed unresponsive.
	 */
	void (*pong)(struct wl_client *client,
		     struct wl_resource *resource,
		     uint32_t serial);
	/**
	 * move - (none)
	 * @seat: (none)
	 * @serial: (none)
	 */
	void (*move)(struct wl_client *client,
		     struct wl_resource *resource,
		     struct wl_resource *seat,
		     uint32_t serial);
	/**
	 * resize - (none)
	 * @seat: (none)
	 * @serial: (none)
	 * @edges: (none)
	 */
	void (*resize)(struct wl_client *client,
		       struct wl_resource *resource,
		       struct wl_resource *seat,
		       uint32_t serial,
		       uint32_t edges);
	/**
	 * set_toplevel - make the surface a top level surface
	 *
	 * Make the surface a toplevel window.
	 */
	void (*set_toplevel)(struct wl_client *client,
			     struct wl_resource *resource);
	/**
	 * set_transient - make the surface a transient surface
	 * @parent: (none)
	 * @x: (none)
	 * @y: (none)
	 * @flags: (none)
	 *
	 * Map the surface relative to an existing surface. The x and y
	 * arguments specify the locations of the upper left corner of the
	 * surface relative to the upper left corner of the parent surface.
	 * The flags argument controls overflow/clipping behaviour when the
	 * surface would intersect a screen edge, panel or such. And
	 * possibly whether the offset only determines the initial position
	 * or if the surface is locked to that relative position during
	 * moves.
	 */
	void (*set_transient)(struct wl_client *client,
			      struct wl_resource *resource,
			      struct wl_resource *parent,
			      int32_t x,
			      int32_t y,
			      uint32_t flags);
	/**
	 * set_fullscreen - make the surface a fullscreen surface
	 * @method: (none)
	 * @framerate: (none)
	 * @output: (none)
	 *
	 * Map the surface as a fullscreen surface. If an output
	 * parameter is given then the surface will be made fullscreen on
	 * that output. If the client does not specify the output then the
	 * compositor will apply its policy - usually choosing the output
	 * on which the surface has the biggest surface area. The client
	 * may specify a method to resolve a size conflict between the
	 * output size and the surface size - this is provided through the
	 * fullscreen_method parameter. The framerate parameter is used
	 * only when the fullscreen_method is set to "driver", to indicate
	 * the preferred framerate. framerate=0 indicates that the app does
	 * not care about framerate. The framerate is specified in mHz,
	 * that is framerate of 60000 is 60Hz. The compositor must reply to
	 * this request with a configure event with the dimensions for the
	 * output on which the surface will be made fullscreen.
	 */
	void (*set_fullscreen)(struct wl_client *client,
			       struct wl_resource *resource,
			       uint32_t method,
			       uint32_t framerate,
			       struct wl_resource *output);
	/**
	 * set_popup - make the surface a popup surface
	 * @seat: (none)
	 * @serial: (none)
	 * @parent: (none)
	 * @x: (none)
	 * @y: (none)
	 * @flags: (none)
	 *
	 * Popup surfaces. Will switch an implicit grab into owner-events
	 * mode, and grab will continue after the implicit grab ends
	 * (button released). Once the implicit grab is over, the popup
	 * grab continues until the window is destroyed or a mouse button
	 * is pressed in any other clients window. A click in any of the
	 * clients surfaces is reported as normal, however, clicks in other
	 * clients surfaces will be discarded and trigger the callback.
	 * TODO: Grab keyboard too, maybe just terminate on any click
	 * inside or outside the surface?
	 */
	void (*set_popup)(struct wl_client *client,
			  struct wl_resource *resource,
			  struct wl_resource *seat,
			  uint32_t serial,
			  struct wl_resource *parent,
			  int32_t x,
			  int32_t y,
			  uint32_t flags);
	/**
	 * set_maximized - make the surface a maximized surface
	 * @output: (none)
	 *
	 * A request from the client to notify the compositor the
	 * maximized operation. The compositor will reply with a configure
	 * event telling the expected new surface size. The operation is
	 * completed on the next buffer attach to this surface. A maximized
	 * client will fill the fullscreen of the output it is bound to,
	 * except the panel area. This is the main difference between a
	 * maximized shell surface and a fullscreen shell surface.
	 */
	void (*set_maximized)(struct wl_client *client,
			      struct wl_resource *resource,
			      struct wl_resource *output);
	/**
	 * set_title - set surface title
	 * @title: (none)
	 *
	 * 
	 */
	void (*set_title)(struct wl_client *client,
			  struct wl_resource *resource,
			  const char *title);
	/**
	 * set_class - set surface class
	 * @class_: (none)
	 *
	 * The surface class identifies the general class of applications
	 * to which the surface belongs. The class is the file name of the
	 * applications .desktop file (absolute path if non-standard
	 * location).
	 */
	void (*set_class)(struct wl_client *client,
			  struct wl_resource *resource,
			  const char *class_);
};

#define WL_SHELL_SURFACE_PING	0
#define WL_SHELL_SURFACE_CONFIGURE	1
#define WL_SHELL_SURFACE_POPUP_DONE	2

static inline void
wl_shell_surface_send_ping(struct wl_resource *resource_, uint32_t serial)
{
	wl_resource_post_event(resource_, WL_SHELL_SURFACE_PING, serial);
}

static inline void
wl_shell_surface_send_configure(struct wl_resource *resource_, uint32_t edges, int32_t width, int32_t height)
{
	wl_resource_post_event(resource_, WL_SHELL_SURFACE_CONFIGURE, edges, width, height);
}

static inline void
wl_shell_surface_send_popup_done(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_SHELL_SURFACE_POPUP_DONE);
}

/**
 * wl_surface - an onscreen surface
 * @destroy: delete surface
 * @attach: set the surface contents
 * @damage: mark part of the surface damaged
 * @frame: request repaint feedback
 * @set_opaque_region: set opaque region
 * @set_input_region: set input region
 *
 * A surface. This is an image that is displayed on the screen. It has a
 * location, size and pixel contents.
 */
struct wl_surface_interface {
	/**
	 * destroy - delete surface
	 *
	 * Deletes the surface and invalidates its object id.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * attach - set the surface contents
	 * @buffer: (none)
	 * @x: (none)
	 * @y: (none)
	 *
	 * Copy the contents of a buffer into this surface. The x and y
	 * arguments specify the location of the new buffers upper left
	 * corner, relative to the old buffers upper left corner.
	 */
	void (*attach)(struct wl_client *client,
		       struct wl_resource *resource,
		       struct wl_resource *buffer,
		       int32_t x,
		       int32_t y);
	/**
	 * damage - mark part of the surface damaged
	 * @x: (none)
	 * @y: (none)
	 * @width: (none)
	 * @height: (none)
	 *
	 * After attaching a new buffer, this request is used to describe
	 * the regions where the new buffer is different from the previous
	 * buffer and needs to be repainted. Coordinates are relative to
	 * the new buffer.
	 */
	void (*damage)(struct wl_client *client,
		       struct wl_resource *resource,
		       int32_t x,
		       int32_t y,
		       int32_t width,
		       int32_t height);
	/**
	 * frame - request repaint feedback
	 * @callback: (none)
	 *
	 * Request notification when the next frame is displayed. Useful
	 * for throttling redrawing operations, and driving animations. The
	 * notification will only be posted for one frame unless requested
	 * again.
	 */
	void (*frame)(struct wl_client *client,
		      struct wl_resource *resource,
		      uint32_t callback);
	/**
	 * set_opaque_region - set opaque region
	 * @region: (none)
	 *
	 * This requests sets the region of the surface that contain
	 * opaque content. The opaque region is an optimization hint for
	 * the compositor that lets it optimize out redrawing of content
	 * behind opaque regions. Setting an opaque region is not required
	 * for correct behaviour, but marking transparent content as opaque
	 * will result in repaint artifacts. The region will be clipped to
	 * the extents of the current surface size. Setting the region has
	 * copy semantics, and the region object can be destroyed
	 * immediately after setting the opaque region. If a buffer of a
	 * different size is attached or if a nil region is set, the opaque
	 * region will revert back to default. The default opaque region is
	 * empty.
	 */
	void (*set_opaque_region)(struct wl_client *client,
				  struct wl_resource *resource,
				  struct wl_resource *region);
	/**
	 * set_input_region - set input region
	 * @region: (none)
	 *
	 * This requests sets the region of the surface that can receive
	 * pointer and touch events. The region will be clipped to the
	 * extents of the current surface size. Setting the region has copy
	 * semantics, and the region object can be destroyed immediately
	 * after setting the input region. If a buffer of a different size
	 * is attached or if a nil region is passed, the input region will
	 * revert back to default. The default input region is the entire
	 * surface.
	 */
	void (*set_input_region)(struct wl_client *client,
				 struct wl_resource *resource,
				 struct wl_resource *region);
};

#define WL_SURFACE_ENTER	0
#define WL_SURFACE_LEAVE	1

static inline void
wl_surface_send_enter(struct wl_resource *resource_, struct wl_resource *output)
{
	wl_resource_post_event(resource_, WL_SURFACE_ENTER, output);
}

static inline void
wl_surface_send_leave(struct wl_resource *resource_, struct wl_resource *output)
{
	wl_resource_post_event(resource_, WL_SURFACE_LEAVE, output);
}

#ifndef WL_SEAT_CAPABILITY_ENUM
#define WL_SEAT_CAPABILITY_ENUM
/**
 * wl_seat_capability - seat capability bitmask
 * @WL_SEAT_CAPABILITY_POINTER: wl_pointer
 * @WL_SEAT_CAPABILITY_KEYBOARD: wl_keyboard
 * @WL_SEAT_CAPABILITY_TOUCH: wl_touch
 *
 * This is a bitmask of capabilities this seat has; if a member is set,
 * then it is present on the seat.
 */
enum wl_seat_capability {
	WL_SEAT_CAPABILITY_POINTER = 1,
	WL_SEAT_CAPABILITY_KEYBOARD = 2,
	WL_SEAT_CAPABILITY_TOUCH = 4,
};
#endif /* WL_SEAT_CAPABILITY_ENUM */

/**
 * wl_seat - seat
 * @get_pointer: return pointer object
 * @get_keyboard: return pointer object
 * @get_touch: return pointer object
 *
 * A group of keyboards, pointer (mice, for example) and touch devices .
 * This object is published as a global during start up, or when such a
 * device is hot plugged. A seat typically has a pointer and maintains a
 * keyboard_focus and a pointer_focus.
 */
struct wl_seat_interface {
	/**
	 * get_pointer - return pointer object
	 * @id: (none)
	 *
	 * The ID provided will be initialized to the wl_pointer
	 * interface for this seat.
	 */
	void (*get_pointer)(struct wl_client *client,
			    struct wl_resource *resource,
			    uint32_t id);
	/**
	 * get_keyboard - return pointer object
	 * @id: (none)
	 *
	 * The ID provided will be initialized to the wl_keyboard
	 * interface for this seat.
	 */
	void (*get_keyboard)(struct wl_client *client,
			     struct wl_resource *resource,
			     uint32_t id);
	/**
	 * get_touch - return pointer object
	 * @id: (none)
	 *
	 * The ID provided will be initialized to the wl_touch interface
	 * for this seat.
	 */
	void (*get_touch)(struct wl_client *client,
			  struct wl_resource *resource,
			  uint32_t id);
};

#define WL_SEAT_CAPABILITIES	0

static inline void
wl_seat_send_capabilities(struct wl_resource *resource_, uint32_t capabilities)
{
	wl_resource_post_event(resource_, WL_SEAT_CAPABILITIES, capabilities);
}

#ifndef WL_POINTER_BUTTON_STATE_ENUM
#define WL_POINTER_BUTTON_STATE_ENUM
/**
 * wl_pointer_button_state - physical button state
 * @WL_POINTER_BUTTON_STATE_RELEASED: button is not pressed
 * @WL_POINTER_BUTTON_STATE_PRESSED: button is pressed
 *
 * Describes the physical state of a button which provoked the button
 * event.
 */
enum wl_pointer_button_state {
	WL_POINTER_BUTTON_STATE_RELEASED = 0,
	WL_POINTER_BUTTON_STATE_PRESSED = 1,
};
#endif /* WL_POINTER_BUTTON_STATE_ENUM */

#ifndef WL_POINTER_AXIS_ENUM
#define WL_POINTER_AXIS_ENUM
/**
 * wl_pointer_axis - axis types
 * @WL_POINTER_AXIS_VERTICAL_SCROLL: (none)
 * @WL_POINTER_AXIS_HORIZONTAL_SCROLL: (none)
 *
 * 
 */
enum wl_pointer_axis {
	WL_POINTER_AXIS_VERTICAL_SCROLL = 0,
	WL_POINTER_AXIS_HORIZONTAL_SCROLL = 1,
};
#endif /* WL_POINTER_AXIS_ENUM */

struct wl_pointer_interface {
	/**
	 * set_cursor - set the pointer surface
	 * @serial: (none)
	 * @surface: (none)
	 * @hotspot_x: (none)
	 * @hotspot_y: (none)
	 *
	 * Set the pointer surface, i.e., the surface that contains the
	 * pointer image. This request only takes effect if the pointer
	 * focus for this device is one of the requesting client surfaces
	 * or the surface parameter is the current pointer surface. If
	 * there was a previous surface set with this request it is
	 * replaced. If surface is NULL, the pointer image is hidden. The
	 * parameters hotspot_x and hotspot_y define the position of the
	 * pointer surface relative to the pointer location. Its top-left
	 * corner is always at (x, y) - (hotspot_x, hotspot_y), where (x,
	 * y) are the coordinates of the pointer location. On
	 * surface.attach requests to the pointer surface, hotspot_x and
	 * hotspot_y are decremented by the x and y parameters passed to
	 * the request. The hotspot can also be updated by passing the
	 * current set pointer surface to this request with new values for
	 * hotspot_x and/or hotspot_y.
	 */
	void (*set_cursor)(struct wl_client *client,
			   struct wl_resource *resource,
			   uint32_t serial,
			   struct wl_resource *surface,
			   int32_t hotspot_x,
			   int32_t hotspot_y);
};

#define WL_POINTER_ENTER	0
#define WL_POINTER_LEAVE	1
#define WL_POINTER_MOTION	2
#define WL_POINTER_BUTTON	3
#define WL_POINTER_AXIS	4

static inline void
wl_pointer_send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
	wl_resource_post_event(resource_, WL_POINTER_ENTER, serial, surface, surface_x, surface_y);
}

static inline void
wl_pointer_send_leave(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface)
{
	wl_resource_post_event(resource_, WL_POINTER_LEAVE, serial, surface);
}

static inline void
wl_pointer_send_motion(struct wl_resource *resource_, uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y)
{
	wl_resource_post_event(resource_, WL_POINTER_MOTION, time, surface_x, surface_y);
}

static inline void
wl_pointer_send_button(struct wl_resource *resource_, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
{
	wl_resource_post_event(resource_, WL_POINTER_BUTTON, serial, time, button, state);
}

static inline void
wl_pointer_send_axis(struct wl_resource *resource_, uint32_t time, uint32_t axis, wl_fixed_t value)
{
	wl_resource_post_event(resource_, WL_POINTER_AXIS, time, axis, value);
}

#ifndef WL_KEYBOARD_KEYMAP_FORMAT_ENUM
#define WL_KEYBOARD_KEYMAP_FORMAT_ENUM
/**
 * wl_keyboard_keymap_format - keyboard mapping format
 * @WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1: (none)
 *
 * This enum specifies the format of the keymap provided to the client
 * with the wl_keyboard::keymap event.
 */
enum wl_keyboard_keymap_format {
	WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1 = 1,
};
#endif /* WL_KEYBOARD_KEYMAP_FORMAT_ENUM */

#ifndef WL_KEYBOARD_KEY_STATE_ENUM
#define WL_KEYBOARD_KEY_STATE_ENUM
/**
 * wl_keyboard_key_state - physical key state
 * @WL_KEYBOARD_KEY_STATE_RELEASED: key is not pressed
 * @WL_KEYBOARD_KEY_STATE_PRESSED: key is pressed
 * @WL_KEYBOARD_KEY_STATE_REPEAT: key is pressed and repeating
 *
 * Describes the physical state of a key which provoked the key event.
 * When a key repeats, the sequence is down -> repeat -> repeat -> up.
 */
enum wl_keyboard_key_state {
	WL_KEYBOARD_KEY_STATE_RELEASED = 0,
	WL_KEYBOARD_KEY_STATE_PRESSED = 1,
	WL_KEYBOARD_KEY_STATE_REPEAT = 2,
};
#endif /* WL_KEYBOARD_KEY_STATE_ENUM */

#define WL_KEYBOARD_KEYMAP	0
#define WL_KEYBOARD_ENTER	1
#define WL_KEYBOARD_LEAVE	2
#define WL_KEYBOARD_KEY	3
#define WL_KEYBOARD_MODIFIERS	4

static inline void
wl_keyboard_send_keymap(struct wl_resource *resource_, uint32_t format, int32_t fd, uint32_t size)
{
	wl_resource_post_event(resource_, WL_KEYBOARD_KEYMAP, format, fd, size);
}

static inline void
wl_keyboard_send_enter(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface, struct wl_array *keys)
{
	wl_resource_post_event(resource_, WL_KEYBOARD_ENTER, serial, surface, keys);
}

static inline void
wl_keyboard_send_leave(struct wl_resource *resource_, uint32_t serial, struct wl_resource *surface)
{
	wl_resource_post_event(resource_, WL_KEYBOARD_LEAVE, serial, surface);
}

static inline void
wl_keyboard_send_key(struct wl_resource *resource_, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
	wl_resource_post_event(resource_, WL_KEYBOARD_KEY, serial, time, key, state);
}

static inline void
wl_keyboard_send_modifiers(struct wl_resource *resource_, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{
	wl_resource_post_event(resource_, WL_KEYBOARD_MODIFIERS, serial, mods_depressed, mods_latched, mods_locked, group);
}

#define WL_TOUCH_DOWN	0
#define WL_TOUCH_UP	1
#define WL_TOUCH_MOTION	2
#define WL_TOUCH_FRAME	3
#define WL_TOUCH_CANCEL	4

static inline void
wl_touch_send_down(struct wl_resource *resource_, uint32_t serial, uint32_t time, struct wl_resource *surface, int32_t id, wl_fixed_t x, wl_fixed_t y)
{
	wl_resource_post_event(resource_, WL_TOUCH_DOWN, serial, time, surface, id, x, y);
}

static inline void
wl_touch_send_up(struct wl_resource *resource_, uint32_t serial, uint32_t time, int32_t id)
{
	wl_resource_post_event(resource_, WL_TOUCH_UP, serial, time, id);
}

static inline void
wl_touch_send_motion(struct wl_resource *resource_, uint32_t time, int32_t id, wl_fixed_t x, wl_fixed_t y)
{
	wl_resource_post_event(resource_, WL_TOUCH_MOTION, time, id, x, y);
}

static inline void
wl_touch_send_frame(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_TOUCH_FRAME);
}

static inline void
wl_touch_send_cancel(struct wl_resource *resource_)
{
	wl_resource_post_event(resource_, WL_TOUCH_CANCEL);
}

#ifndef WL_OUTPUT_SUBPIXEL_ENUM
#define WL_OUTPUT_SUBPIXEL_ENUM
enum wl_output_subpixel {
	WL_OUTPUT_SUBPIXEL_UNKNOWN = 0,
	WL_OUTPUT_SUBPIXEL_NONE = 1,
	WL_OUTPUT_SUBPIXEL_HORIZONTAL_RGB = 2,
	WL_OUTPUT_SUBPIXEL_HORIZONTAL_BGR = 3,
	WL_OUTPUT_SUBPIXEL_VERTICAL_RGB = 4,
	WL_OUTPUT_SUBPIXEL_VERTICAL_BGR = 5,
};
#endif /* WL_OUTPUT_SUBPIXEL_ENUM */

#ifndef WL_OUTPUT_TRANSFORM_ENUM
#define WL_OUTPUT_TRANSFORM_ENUM
/**
 * wl_output_transform - transform from framebuffer to output
 * @WL_OUTPUT_TRANSFORM_NORMAL: (none)
 * @WL_OUTPUT_TRANSFORM_90: (none)
 * @WL_OUTPUT_TRANSFORM_180: (none)
 * @WL_OUTPUT_TRANSFORM_270: (none)
 * @WL_OUTPUT_TRANSFORM_FLIPPED: (none)
 * @WL_OUTPUT_TRANSFORM_FLIPPED_90: (none)
 * @WL_OUTPUT_TRANSFORM_FLIPPED_180: (none)
 * @WL_OUTPUT_TRANSFORM_FLIPPED_270: (none)
 *
 * This describes the transform that a compositor will apply to a surface
 * to compensate for the rotation or mirroring of an output device. The
 * flipped values correspond to an initial flip around a vertical axis
 * followed by rotaion. The purpose is mainly to allow clients render
 * accordingly and tell the compositor, so that for fullscreen surfaces,
 * the compositor will still be able to scan out directly from client
 * surfaces.
 */
enum wl_output_transform {
	WL_OUTPUT_TRANSFORM_NORMAL = 0,
	WL_OUTPUT_TRANSFORM_90 = 1,
	WL_OUTPUT_TRANSFORM_180 = 2,
	WL_OUTPUT_TRANSFORM_270 = 3,
	WL_OUTPUT_TRANSFORM_FLIPPED = 4,
	WL_OUTPUT_TRANSFORM_FLIPPED_90 = 5,
	WL_OUTPUT_TRANSFORM_FLIPPED_180 = 6,
	WL_OUTPUT_TRANSFORM_FLIPPED_270 = 7,
};
#endif /* WL_OUTPUT_TRANSFORM_ENUM */

#ifndef WL_OUTPUT_MODE_ENUM
#define WL_OUTPUT_MODE_ENUM
/**
 * wl_output_mode - values for the flags bitfield in the mode event
 * @WL_OUTPUT_MODE_CURRENT: indicates this is the current mode
 * @WL_OUTPUT_MODE_PREFERRED: indicates this is the preferred mode
 *
 * 
 */
enum wl_output_mode {
	WL_OUTPUT_MODE_CURRENT = 0x1,
	WL_OUTPUT_MODE_PREFERRED = 0x2,
};
#endif /* WL_OUTPUT_MODE_ENUM */

#define WL_OUTPUT_GEOMETRY	0
#define WL_OUTPUT_MODE	1

static inline void
wl_output_send_geometry(struct wl_resource *resource_, int32_t x, int32_t y, int32_t physical_width, int32_t physical_height, int32_t subpixel, const char *make, const char *model, int32_t transform)
{
	wl_resource_post_event(resource_, WL_OUTPUT_GEOMETRY, x, y, physical_width, physical_height, subpixel, make, model, transform);
}

static inline void
wl_output_send_mode(struct wl_resource *resource_, uint32_t flags, int32_t width, int32_t height, int32_t refresh)
{
	wl_resource_post_event(resource_, WL_OUTPUT_MODE, flags, width, height, refresh);
}

/**
 * wl_region - region interface
 * @destroy: destroy region
 * @add: add rectangle to region
 * @subtract: subtract rectangle from region
 *
 * Region.
 */
struct wl_region_interface {
	/**
	 * destroy - destroy region
	 *
	 * Destroy the region. This will invalidate the object id.
	 */
	void (*destroy)(struct wl_client *client,
			struct wl_resource *resource);
	/**
	 * add - add rectangle to region
	 * @x: (none)
	 * @y: (none)
	 * @width: (none)
	 * @height: (none)
	 *
	 * Add the specified rectangle to the region
	 */
	void (*add)(struct wl_client *client,
		    struct wl_resource *resource,
		    int32_t x,
		    int32_t y,
		    int32_t width,
		    int32_t height);
	/**
	 * subtract - subtract rectangle from region
	 * @x: (none)
	 * @y: (none)
	 * @width: (none)
	 * @height: (none)
	 *
	 * Subtract the specified rectangle from the region
	 */
	void (*subtract)(struct wl_client *client,
			 struct wl_resource *resource,
			 int32_t x,
			 int32_t y,
			 int32_t width,
			 int32_t height);
};

#ifdef  __cplusplus
}
#endif

#endif
