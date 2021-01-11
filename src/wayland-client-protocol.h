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

#ifndef WAYLAND_CLIENT_PROTOCOL_H
#define WAYLAND_CLIENT_PROTOCOL_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "wayland-client.h"

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
 * @error: fatal error event
 * @global: announce global object
 * @global_remove: announce removal of global object
 * @delete_id: acknowledge object id deletion
 *
 * The core global object. This is a special singleton object. It is used
 * for internal wayland protocol features.
 */
struct wl_display_listener {
	/**
	 * error - fatal error event
	 * @object_id: (none)
	 * @code: (none)
	 * @message: (none)
	 *
	 * The error event is sent out when a fatal (non-recoverable)
	 * error has occurred.
	 */
	void (*error)(void *data,
		      struct wl_display *wl_display,
		      struct wl_object *object_id,
		      uint32_t code,
		      const char *message);
	/**
	 * global - announce global object
	 * @name: (none)
	 * @interface: (none)
	 * @version: (none)
	 *
	 * Notify the client of global objects. These are objects that
	 * are created by the server. Globals are published on the initial
	 * client connection sequence, upon device hotplugs, device
	 * disconnects, reconfiguration or other events. A client can
	 * 'bind' to a global object by using the bind request. This
	 * creates a client side handle that lets the object emit events to
	 * the client and lets the client invoke requests on the object.
	 */
	void (*global)(void *data,
		       struct wl_display *wl_display,
		       uint32_t name,
		       const char *interface,
		       uint32_t version);
	/**
	 * global_remove - announce removal of global object
	 * @name: (none)
	 *
	 * Notify the client of removed global objects.
	 */
	void (*global_remove)(void *data,
			      struct wl_display *wl_display,
			      uint32_t name);
	/**
	 * delete_id - acknowledge object id deletion
	 * @id: (none)
	 *
	 * Server has deleted the id and client can now reuse it.
	 */
	void (*delete_id)(void *data,
			  struct wl_display *wl_display,
			  uint32_t id);
};

static inline int
wl_display_add_listener(struct wl_display *wl_display,
			const struct wl_display_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_display,
				     (void (**)(void)) listener, data);
}

#define WL_DISPLAY_BIND	0
#define WL_DISPLAY_SYNC	1

struct wl_callback_listener {
	/**
	 * done - (none)
	 * @serial: (none)
	 */
	void (*done)(void *data,
		     struct wl_callback *wl_callback,
		     uint32_t serial);
};

static inline int
wl_callback_add_listener(struct wl_callback *wl_callback,
			 const struct wl_callback_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_callback,
				     (void (**)(void)) listener, data);
}

static inline void
wl_callback_set_user_data(struct wl_callback *wl_callback, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_callback, user_data);
}

static inline void *
wl_callback_get_user_data(struct wl_callback *wl_callback)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_callback);
}

static inline void
wl_callback_destroy(struct wl_callback *wl_callback)
{
	wl_proxy_destroy((struct wl_proxy *) wl_callback);
}

#define WL_COMPOSITOR_CREATE_SURFACE	0
#define WL_COMPOSITOR_CREATE_REGION	1

static inline void
wl_compositor_set_user_data(struct wl_compositor *wl_compositor, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_compositor, user_data);
}

static inline void *
wl_compositor_get_user_data(struct wl_compositor *wl_compositor)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_compositor);
}

static inline void
wl_compositor_destroy(struct wl_compositor *wl_compositor)
{
	wl_proxy_destroy((struct wl_proxy *) wl_compositor);
}

static inline struct wl_surface *
wl_compositor_create_surface(struct wl_compositor *wl_compositor)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_compositor,
			     &wl_surface_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_compositor,
			 WL_COMPOSITOR_CREATE_SURFACE, id);

	return (struct wl_surface *) id;
}

static inline struct wl_region *
wl_compositor_create_region(struct wl_compositor *wl_compositor)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_compositor,
			     &wl_region_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_compositor,
			 WL_COMPOSITOR_CREATE_REGION, id);

	return (struct wl_region *) id;
}

#define WL_SHM_POOL_CREATE_BUFFER	0
#define WL_SHM_POOL_DESTROY	1
#define WL_SHM_POOL_RESIZE	2

static inline void
wl_shm_pool_set_user_data(struct wl_shm_pool *wl_shm_pool, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shm_pool, user_data);
}

static inline void *
wl_shm_pool_get_user_data(struct wl_shm_pool *wl_shm_pool)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shm_pool);
}

static inline struct wl_buffer *
wl_shm_pool_create_buffer(struct wl_shm_pool *wl_shm_pool, int32_t offset, int32_t width, int32_t height, int32_t stride, uint32_t format)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_shm_pool,
			     &wl_buffer_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_CREATE_BUFFER, id, offset, width, height, stride, format);

	return (struct wl_buffer *) id;
}

static inline void
wl_shm_pool_destroy(struct wl_shm_pool *wl_shm_pool)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_shm_pool);
}

static inline void
wl_shm_pool_resize(struct wl_shm_pool *wl_shm_pool, int32_t size)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shm_pool,
			 WL_SHM_POOL_RESIZE, size);
}

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
 * @format: (none)
 *
 * Support for shared memory buffers.
 */
struct wl_shm_listener {
	/**
	 * format - (none)
	 * @format: (none)
	 */
	void (*format)(void *data,
		       struct wl_shm *wl_shm,
		       uint32_t format);
};

static inline int
wl_shm_add_listener(struct wl_shm *wl_shm,
		    const struct wl_shm_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_shm,
				     (void (**)(void)) listener, data);
}

#define WL_SHM_CREATE_POOL	0

static inline void
wl_shm_set_user_data(struct wl_shm *wl_shm, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shm, user_data);
}

static inline void *
wl_shm_get_user_data(struct wl_shm *wl_shm)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shm);
}

static inline void
wl_shm_destroy(struct wl_shm *wl_shm)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shm);
}

static inline struct wl_shm_pool *
wl_shm_create_pool(struct wl_shm *wl_shm, int32_t fd, int32_t size)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_shm,
			     &wl_shm_pool_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_shm,
			 WL_SHM_CREATE_POOL, id, fd, size);

	return (struct wl_shm_pool *) id;
}

/**
 * wl_buffer - content for a wl_surface
 * @release: compositor releases buffer
 *
 * A buffer provides the content for a wl_surface. Buffers are created
 * through factory interfaces such as wl_drm, wl_shm or similar. It has a
 * width and a height and can be attached to a wl_surface, but the
 * mechanism by which a client provides and updates the contents is defined
 * by the buffer factory interface
 */
struct wl_buffer_listener {
	/**
	 * release - compositor releases buffer
	 *
	 * Sent when an attached buffer is no longer used by the
	 * compositor.
	 */
	void (*release)(void *data,
			struct wl_buffer *wl_buffer);
};

static inline int
wl_buffer_add_listener(struct wl_buffer *wl_buffer,
		       const struct wl_buffer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_buffer,
				     (void (**)(void)) listener, data);
}

#define WL_BUFFER_DESTROY	0

static inline void
wl_buffer_set_user_data(struct wl_buffer *wl_buffer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_buffer, user_data);
}

static inline void *
wl_buffer_get_user_data(struct wl_buffer *wl_buffer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_buffer);
}

static inline void
wl_buffer_destroy(struct wl_buffer *wl_buffer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_buffer,
			 WL_BUFFER_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_buffer);
}

struct wl_data_offer_listener {
	/**
	 * offer - advertise offered mime-type
	 * @type: (none)
	 *
	 * Sent immediately after creating the wl_data_offer object. One
	 * event per offered mime type.
	 */
	void (*offer)(void *data,
		      struct wl_data_offer *wl_data_offer,
		      const char *type);
};

static inline int
wl_data_offer_add_listener(struct wl_data_offer *wl_data_offer,
			   const struct wl_data_offer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_offer,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_OFFER_ACCEPT	0
#define WL_DATA_OFFER_RECEIVE	1
#define WL_DATA_OFFER_DESTROY	2

static inline void
wl_data_offer_set_user_data(struct wl_data_offer *wl_data_offer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_offer, user_data);
}

static inline void *
wl_data_offer_get_user_data(struct wl_data_offer *wl_data_offer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_offer);
}

static inline void
wl_data_offer_accept(struct wl_data_offer *wl_data_offer, uint32_t serial, const char *type)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_ACCEPT, serial, type);
}

static inline void
wl_data_offer_receive(struct wl_data_offer *wl_data_offer, const char *mime_type, int32_t fd)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_RECEIVE, mime_type, fd);
}

static inline void
wl_data_offer_destroy(struct wl_data_offer *wl_data_offer)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_offer,
			 WL_DATA_OFFER_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_data_offer);
}

struct wl_data_source_listener {
	/**
	 * target - a target accepts an offered mime-type
	 * @mime_type: (none)
	 *
	 * Sent when a target accepts pointer_focus or motion events. If
	 * a target does not accept any of the offered types, type is NULL.
	 */
	void (*target)(void *data,
		       struct wl_data_source *wl_data_source,
		       const char *mime_type);
	/**
	 * send - send the data
	 * @mime_type: (none)
	 * @fd: (none)
	 *
	 * Request for data from another client. Send the data as the
	 * specified mime-type over the passed fd, then close the fd.
	 */
	void (*send)(void *data,
		     struct wl_data_source *wl_data_source,
		     const char *mime_type,
		     int32_t fd);
	/**
	 * cancelled - selection was cancelled
	 *
	 * Another selection became active.
	 */
	void (*cancelled)(void *data,
			  struct wl_data_source *wl_data_source);
};

static inline int
wl_data_source_add_listener(struct wl_data_source *wl_data_source,
			    const struct wl_data_source_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_source,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_SOURCE_OFFER	0
#define WL_DATA_SOURCE_DESTROY	1

static inline void
wl_data_source_set_user_data(struct wl_data_source *wl_data_source, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_source, user_data);
}

static inline void *
wl_data_source_get_user_data(struct wl_data_source *wl_data_source)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_source);
}

static inline void
wl_data_source_offer(struct wl_data_source *wl_data_source, const char *type)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_source,
			 WL_DATA_SOURCE_OFFER, type);
}

static inline void
wl_data_source_destroy(struct wl_data_source *wl_data_source)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_source,
			 WL_DATA_SOURCE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_data_source);
}

struct wl_data_device_listener {
	/**
	 * data_offer - introduce a new wl_data_offer
	 * @id: (none)
	 *
	 * The data_offer event introduces a new wl_data_offer object,
	 * which will subsequently be used in either the data_device.enter
	 * event (for drag and drop) or the data_device.selection event
	 * (for selections). Immediately following the
	 * data_device_data_offer event, the new data_offer object will
	 * send out data_offer.offer events to describe the mime-types it
	 * offers.
	 */
	void (*data_offer)(void *data,
			   struct wl_data_device *wl_data_device,
			   struct wl_data_offer *id);
	/**
	 * enter - (none)
	 * @serial: (none)
	 * @surface: (none)
	 * @x: (none)
	 * @y: (none)
	 * @id: (none)
	 */
	void (*enter)(void *data,
		      struct wl_data_device *wl_data_device,
		      uint32_t serial,
		      struct wl_surface *surface,
		      wl_fixed_t x,
		      wl_fixed_t y,
		      struct wl_data_offer *id);
	/**
	 * leave - (none)
	 */
	void (*leave)(void *data,
		      struct wl_data_device *wl_data_device);
	/**
	 * motion - (none)
	 * @time: (none)
	 * @x: (none)
	 * @y: (none)
	 */
	void (*motion)(void *data,
		       struct wl_data_device *wl_data_device,
		       uint32_t time,
		       wl_fixed_t x,
		       wl_fixed_t y);
	/**
	 * drop - (none)
	 */
	void (*drop)(void *data,
		     struct wl_data_device *wl_data_device);
	/**
	 * selection - advertise new selection
	 * @id: (none)
	 *
	 * The selection event is sent out to notify the client of a new
	 * wl_data_offer for the selection for this device. The
	 * data_device.data_offer and the data_offer.offer events are sent
	 * out immediately before this event to introduce the data offer
	 * object. The selection event is sent to a client immediately
	 * before receiving keyboard focus and when a new selection is set
	 * while the client has keyboard focus. The data_offer is valid
	 * until a new data_offer or NULL is received or until the client
	 * loses keyboard focus.
	 */
	void (*selection)(void *data,
			  struct wl_data_device *wl_data_device,
			  struct wl_data_offer *id);
};

static inline int
wl_data_device_add_listener(struct wl_data_device *wl_data_device,
			    const struct wl_data_device_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_data_device,
				     (void (**)(void)) listener, data);
}

#define WL_DATA_DEVICE_START_DRAG	0
#define WL_DATA_DEVICE_SET_SELECTION	1

static inline void
wl_data_device_set_user_data(struct wl_data_device *wl_data_device, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_device, user_data);
}

static inline void *
wl_data_device_get_user_data(struct wl_data_device *wl_data_device)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_device);
}

static inline void
wl_data_device_destroy(struct wl_data_device *wl_data_device)
{
	wl_proxy_destroy((struct wl_proxy *) wl_data_device);
}

static inline void
wl_data_device_start_drag(struct wl_data_device *wl_data_device, struct wl_data_source *source, struct wl_surface *origin, struct wl_surface *icon, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_device,
			 WL_DATA_DEVICE_START_DRAG, source, origin, icon, serial);
}

static inline void
wl_data_device_set_selection(struct wl_data_device *wl_data_device, struct wl_data_source *source, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_data_device,
			 WL_DATA_DEVICE_SET_SELECTION, source, serial);
}

#define WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE	0
#define WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE	1

static inline void
wl_data_device_manager_set_user_data(struct wl_data_device_manager *wl_data_device_manager, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_data_device_manager, user_data);
}

static inline void *
wl_data_device_manager_get_user_data(struct wl_data_device_manager *wl_data_device_manager)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_data_device_manager);
}

static inline void
wl_data_device_manager_destroy(struct wl_data_device_manager *wl_data_device_manager)
{
	wl_proxy_destroy((struct wl_proxy *) wl_data_device_manager);
}

static inline struct wl_data_source *
wl_data_device_manager_create_data_source(struct wl_data_device_manager *wl_data_device_manager)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_data_device_manager,
			     &wl_data_source_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_data_device_manager,
			 WL_DATA_DEVICE_MANAGER_CREATE_DATA_SOURCE, id);

	return (struct wl_data_source *) id;
}

static inline struct wl_data_device *
wl_data_device_manager_get_data_device(struct wl_data_device_manager *wl_data_device_manager, struct wl_seat *seat)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_data_device_manager,
			     &wl_data_device_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_data_device_manager,
			 WL_DATA_DEVICE_MANAGER_GET_DATA_DEVICE, id, seat);

	return (struct wl_data_device *) id;
}

#define WL_SHELL_GET_SHELL_SURFACE	0

static inline void
wl_shell_set_user_data(struct wl_shell *wl_shell, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shell, user_data);
}

static inline void *
wl_shell_get_user_data(struct wl_shell *wl_shell)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shell);
}

static inline void
wl_shell_destroy(struct wl_shell *wl_shell)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shell);
}

static inline struct wl_shell_surface *
wl_shell_get_shell_surface(struct wl_shell *wl_shell, struct wl_surface *surface)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_shell,
			     &wl_shell_surface_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_shell,
			 WL_SHELL_GET_SHELL_SURFACE, id, surface);

	return (struct wl_shell_surface *) id;
}

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
 * @ping: ping client
 * @configure: suggest resize
 * @popup_done: popup interaction is done
 *
 * An interface implemented by a wl_surface. On server side the object is
 * automatically destroyed when the related wl_surface is destroyed. On
 * client side, wl_shell_surface_destroy() must be called before destroying
 * the wl_surface object.
 */
struct wl_shell_surface_listener {
	/**
	 * ping - ping client
	 * @serial: (none)
	 *
	 * Ping a client to check if it is receiving events and sending
	 * requests. A client is expected to reply with a pong request.
	 */
	void (*ping)(void *data,
		     struct wl_shell_surface *wl_shell_surface,
		     uint32_t serial);
	/**
	 * configure - suggest resize
	 * @edges: (none)
	 * @width: (none)
	 * @height: (none)
	 *
	 * The configure event asks the client to resize its surface. The
	 * size is a hint, in the sense that the client is free to ignore
	 * it if it doesn't resize, pick a smaller size (to satisfy aspect
	 * ration or resize in steps of NxM pixels). The client is free to
	 * dismiss all but the last configure event it received.
	 */
	void (*configure)(void *data,
			  struct wl_shell_surface *wl_shell_surface,
			  uint32_t edges,
			  int32_t width,
			  int32_t height);
	/**
	 * popup_done - popup interaction is done
	 *
	 * The popup_done event is sent out when a popup grab is broken,
	 * that is, when the users clicks a surface that doesn't belong to
	 * the client owning the popup surface.
	 */
	void (*popup_done)(void *data,
			   struct wl_shell_surface *wl_shell_surface);
};

static inline int
wl_shell_surface_add_listener(struct wl_shell_surface *wl_shell_surface,
			      const struct wl_shell_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_shell_surface,
				     (void (**)(void)) listener, data);
}

#define WL_SHELL_SURFACE_PONG	0
#define WL_SHELL_SURFACE_MOVE	1
#define WL_SHELL_SURFACE_RESIZE	2
#define WL_SHELL_SURFACE_SET_TOPLEVEL	3
#define WL_SHELL_SURFACE_SET_TRANSIENT	4
#define WL_SHELL_SURFACE_SET_FULLSCREEN	5
#define WL_SHELL_SURFACE_SET_POPUP	6
#define WL_SHELL_SURFACE_SET_MAXIMIZED	7
#define WL_SHELL_SURFACE_SET_TITLE	8
#define WL_SHELL_SURFACE_SET_CLASS	9

static inline void
wl_shell_surface_set_user_data(struct wl_shell_surface *wl_shell_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_shell_surface, user_data);
}

static inline void *
wl_shell_surface_get_user_data(struct wl_shell_surface *wl_shell_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_shell_surface);
}

static inline void
wl_shell_surface_destroy(struct wl_shell_surface *wl_shell_surface)
{
	wl_proxy_destroy((struct wl_proxy *) wl_shell_surface);
}

static inline void
wl_shell_surface_pong(struct wl_shell_surface *wl_shell_surface, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_PONG, serial);
}

static inline void
wl_shell_surface_move(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_MOVE, seat, serial);
}

static inline void
wl_shell_surface_resize(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial, uint32_t edges)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_RESIZE, seat, serial, edges);
}

static inline void
wl_shell_surface_set_toplevel(struct wl_shell_surface *wl_shell_surface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TOPLEVEL);
}

static inline void
wl_shell_surface_set_transient(struct wl_shell_surface *wl_shell_surface, struct wl_surface *parent, int32_t x, int32_t y, uint32_t flags)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TRANSIENT, parent, x, y, flags);
}

static inline void
wl_shell_surface_set_fullscreen(struct wl_shell_surface *wl_shell_surface, uint32_t method, uint32_t framerate, struct wl_output *output)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_FULLSCREEN, method, framerate, output);
}

static inline void
wl_shell_surface_set_popup(struct wl_shell_surface *wl_shell_surface, struct wl_seat *seat, uint32_t serial, struct wl_surface *parent, int32_t x, int32_t y, uint32_t flags)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_POPUP, seat, serial, parent, x, y, flags);
}

static inline void
wl_shell_surface_set_maximized(struct wl_shell_surface *wl_shell_surface, struct wl_output *output)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_MAXIMIZED, output);
}

static inline void
wl_shell_surface_set_title(struct wl_shell_surface *wl_shell_surface, const char *title)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_TITLE, title);
}

static inline void
wl_shell_surface_set_class(struct wl_shell_surface *wl_shell_surface, const char *class_)
{
	wl_proxy_marshal((struct wl_proxy *) wl_shell_surface,
			 WL_SHELL_SURFACE_SET_CLASS, class_);
}

/**
 * wl_surface - an onscreen surface
 * @enter: surface enters an output
 * @leave: surface leaves an output
 *
 * A surface. This is an image that is displayed on the screen. It has a
 * location, size and pixel contents.
 */
struct wl_surface_listener {
	/**
	 * enter - surface enters an output
	 * @output: (none)
	 *
	 * This is emitted whenever a surface's creation, movement, or
	 * resizing results in some part of it being within the scanout
	 * region of an output.
	 */
	void (*enter)(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);
	/**
	 * leave - surface leaves an output
	 * @output: (none)
	 *
	 * This is emitted whenever a surface's creation, movement, or
	 * resizing results in it no longer having any part of it within
	 * the scanout region of an output.
	 */
	void (*leave)(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);
};

static inline int
wl_surface_add_listener(struct wl_surface *wl_surface,
			const struct wl_surface_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_surface,
				     (void (**)(void)) listener, data);
}

#define WL_SURFACE_DESTROY	0
#define WL_SURFACE_ATTACH	1
#define WL_SURFACE_DAMAGE	2
#define WL_SURFACE_FRAME	3
#define WL_SURFACE_SET_OPAQUE_REGION	4
#define WL_SURFACE_SET_INPUT_REGION	5

static inline void
wl_surface_set_user_data(struct wl_surface *wl_surface, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_surface, user_data);
}

static inline void *
wl_surface_get_user_data(struct wl_surface *wl_surface)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_surface);
}

static inline void
wl_surface_destroy(struct wl_surface *wl_surface)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_surface);
}

static inline void
wl_surface_attach(struct wl_surface *wl_surface, struct wl_buffer *buffer, int32_t x, int32_t y)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_ATTACH, buffer, x, y);
}

static inline void
wl_surface_damage(struct wl_surface *wl_surface, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_DAMAGE, x, y, width, height);
}

static inline struct wl_callback *
wl_surface_frame(struct wl_surface *wl_surface)
{
	struct wl_proxy *callback;

	callback = wl_proxy_create((struct wl_proxy *) wl_surface,
			     &wl_callback_interface);
	if (!callback)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_FRAME, callback);

	return (struct wl_callback *) callback;
}

static inline void
wl_surface_set_opaque_region(struct wl_surface *wl_surface, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_OPAQUE_REGION, region);
}

static inline void
wl_surface_set_input_region(struct wl_surface *wl_surface, struct wl_region *region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_surface,
			 WL_SURFACE_SET_INPUT_REGION, region);
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
 * @capabilities: seat capabilities changed
 *
 * A group of keyboards, pointer (mice, for example) and touch devices .
 * This object is published as a global during start up, or when such a
 * device is hot plugged. A seat typically has a pointer and maintains a
 * keyboard_focus and a pointer_focus.
 */
struct wl_seat_listener {
	/**
	 * capabilities - seat capabilities changed
	 * @capabilities: (none)
	 *
	 * This is emitted whenever a seat gains or loses the pointer,
	 * keyboard or touch capabilities. The argument is a
	 * wl_seat_caps_mask enum containing the complete set of
	 * capabilities this seat has.
	 */
	void (*capabilities)(void *data,
			     struct wl_seat *wl_seat,
			     uint32_t capabilities);
};

static inline int
wl_seat_add_listener(struct wl_seat *wl_seat,
		     const struct wl_seat_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_seat,
				     (void (**)(void)) listener, data);
}

#define WL_SEAT_GET_POINTER	0
#define WL_SEAT_GET_KEYBOARD	1
#define WL_SEAT_GET_TOUCH	2

static inline void
wl_seat_set_user_data(struct wl_seat *wl_seat, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_seat, user_data);
}

static inline void *
wl_seat_get_user_data(struct wl_seat *wl_seat)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_seat);
}

static inline void
wl_seat_destroy(struct wl_seat *wl_seat)
{
	wl_proxy_destroy((struct wl_proxy *) wl_seat);
}

static inline struct wl_pointer *
wl_seat_get_pointer(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_seat,
			     &wl_pointer_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_POINTER, id);

	return (struct wl_pointer *) id;
}

static inline struct wl_keyboard *
wl_seat_get_keyboard(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_seat,
			     &wl_keyboard_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_KEYBOARD, id);

	return (struct wl_keyboard *) id;
}

static inline struct wl_touch *
wl_seat_get_touch(struct wl_seat *wl_seat)
{
	struct wl_proxy *id;

	id = wl_proxy_create((struct wl_proxy *) wl_seat,
			     &wl_touch_interface);
	if (!id)
		return NULL;

	wl_proxy_marshal((struct wl_proxy *) wl_seat,
			 WL_SEAT_GET_TOUCH, id);

	return (struct wl_touch *) id;
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

struct wl_pointer_listener {
	/**
	 * enter - enter event
	 * @serial: (none)
	 * @surface: (none)
	 * @surface_x: (none)
	 * @surface_y: (none)
	 *
	 * Notification that this seat's pointer is focused on a certain
	 * surface. When an seat's focus enters a surface, the pointer
	 * image is undefined and a client should respond to this event by
	 * setting an appropriate pointer image.
	 */
	void (*enter)(void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface,
		      wl_fixed_t surface_x,
		      wl_fixed_t surface_y);
	/**
	 * leave - leave event
	 * @serial: (none)
	 * @surface: (none)
	 *
	 * 
	 */
	void (*leave)(void *data,
		      struct wl_pointer *wl_pointer,
		      uint32_t serial,
		      struct wl_surface *surface);
	/**
	 * motion - pointer motion event
	 * @time: (none)
	 * @surface_x: (none)
	 * @surface_y: (none)
	 *
	 * Notification of pointer location change. The arguments
	 * surface_[xy] are the location relative to the focused surface.
	 */
	void (*motion)(void *data,
		       struct wl_pointer *wl_pointer,
		       uint32_t time,
		       wl_fixed_t surface_x,
		       wl_fixed_t surface_y);
	/**
	 * button - pointer button event
	 * @serial: (none)
	 * @time: (none)
	 * @button: (none)
	 * @state: (none)
	 *
	 * Mouse button click and release notifications. The location of
	 * the click is given by the last motion or pointer_focus event.
	 */
	void (*button)(void *data,
		       struct wl_pointer *wl_pointer,
		       uint32_t serial,
		       uint32_t time,
		       uint32_t button,
		       uint32_t state);
	/**
	 * axis - axis event
	 * @time: (none)
	 * @axis: (none)
	 * @value: (none)
	 *
	 * Scroll and other axis notifications.
	 */
	void (*axis)(void *data,
		     struct wl_pointer *wl_pointer,
		     uint32_t time,
		     uint32_t axis,
		     wl_fixed_t value);
};

static inline int
wl_pointer_add_listener(struct wl_pointer *wl_pointer,
			const struct wl_pointer_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_pointer,
				     (void (**)(void)) listener, data);
}

#define WL_POINTER_SET_CURSOR	0

static inline void
wl_pointer_set_user_data(struct wl_pointer *wl_pointer, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_pointer, user_data);
}

static inline void *
wl_pointer_get_user_data(struct wl_pointer *wl_pointer)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_pointer);
}

static inline void
wl_pointer_destroy(struct wl_pointer *wl_pointer)
{
	wl_proxy_destroy((struct wl_proxy *) wl_pointer);
}

static inline void
wl_pointer_set_cursor(struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, int32_t hotspot_x, int32_t hotspot_y)
{
	wl_proxy_marshal((struct wl_proxy *) wl_pointer,
			 WL_POINTER_SET_CURSOR, serial, surface, hotspot_x, hotspot_y);
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

/**
 * wl_keyboard - keyboard input device
 * @keymap: keyboard mapping
 * @enter: (none)
 * @leave: (none)
 * @key: key event
 * @modifiers: modifier and group state
 *
 * 
 */
struct wl_keyboard_listener {
	/**
	 * keymap - keyboard mapping
	 * @format: (none)
	 * @fd: (none)
	 * @size: (none)
	 *
	 * This event provides a file descriptor to the client which can
	 * be memory-mapped to provide a keyboard mapping description.
	 */
	void (*keymap)(void *data,
		       struct wl_keyboard *wl_keyboard,
		       uint32_t format,
		       int32_t fd,
		       uint32_t size);
	/**
	 * enter - (none)
	 * @serial: (none)
	 * @surface: (none)
	 * @keys: (none)
	 */
	void (*enter)(void *data,
		      struct wl_keyboard *wl_keyboard,
		      uint32_t serial,
		      struct wl_surface *surface,
		      struct wl_array *keys);
	/**
	 * leave - (none)
	 * @serial: (none)
	 * @surface: (none)
	 */
	void (*leave)(void *data,
		      struct wl_keyboard *wl_keyboard,
		      uint32_t serial,
		      struct wl_surface *surface);
	/**
	 * key - key event
	 * @serial: (none)
	 * @time: (none)
	 * @key: (none)
	 * @state: (none)
	 *
	 * A key was pressed or released.
	 */
	void (*key)(void *data,
		    struct wl_keyboard *wl_keyboard,
		    uint32_t serial,
		    uint32_t time,
		    uint32_t key,
		    uint32_t state);
	/**
	 * modifiers - modifier and group state
	 * @serial: (none)
	 * @mods_depressed: (none)
	 * @mods_latched: (none)
	 * @mods_locked: (none)
	 * @group: (none)
	 *
	 * Notifies clients that the modifier and/or group state has
	 * changed, and it should update its local state.
	 */
	void (*modifiers)(void *data,
			  struct wl_keyboard *wl_keyboard,
			  uint32_t serial,
			  uint32_t mods_depressed,
			  uint32_t mods_latched,
			  uint32_t mods_locked,
			  uint32_t group);
};

static inline int
wl_keyboard_add_listener(struct wl_keyboard *wl_keyboard,
			 const struct wl_keyboard_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_keyboard,
				     (void (**)(void)) listener, data);
}

static inline void
wl_keyboard_set_user_data(struct wl_keyboard *wl_keyboard, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_keyboard, user_data);
}

static inline void *
wl_keyboard_get_user_data(struct wl_keyboard *wl_keyboard)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_keyboard);
}

static inline void
wl_keyboard_destroy(struct wl_keyboard *wl_keyboard)
{
	wl_proxy_destroy((struct wl_proxy *) wl_keyboard);
}

/**
 * wl_touch - touch screen input device
 * @down: (none)
 * @up: (none)
 * @motion: (none)
 * @frame: end of touch frame event
 * @cancel: touch session cancelled
 *
 * 
 */
struct wl_touch_listener {
	/**
	 * down - (none)
	 * @serial: (none)
	 * @time: (none)
	 * @surface: (none)
	 * @id: (none)
	 * @x: (none)
	 * @y: (none)
	 */
	void (*down)(void *data,
		     struct wl_touch *wl_touch,
		     uint32_t serial,
		     uint32_t time,
		     struct wl_surface *surface,
		     int32_t id,
		     wl_fixed_t x,
		     wl_fixed_t y);
	/**
	 * up - (none)
	 * @serial: (none)
	 * @time: (none)
	 * @id: (none)
	 */
	void (*up)(void *data,
		   struct wl_touch *wl_touch,
		   uint32_t serial,
		   uint32_t time,
		   int32_t id);
	/**
	 * motion - (none)
	 * @time: (none)
	 * @id: (none)
	 * @x: (none)
	 * @y: (none)
	 */
	void (*motion)(void *data,
		       struct wl_touch *wl_touch,
		       uint32_t time,
		       int32_t id,
		       wl_fixed_t x,
		       wl_fixed_t y);
	/**
	 * frame - end of touch frame event
	 *
	 * Indicates the end of a contact point list.
	 */
	void (*frame)(void *data,
		      struct wl_touch *wl_touch);
	/**
	 * cancel - touch session cancelled
	 *
	 * Sent if the compositor decides the touch stream is a global
	 * gesture. No further events are sent to the clients from that
	 * particular gesture.
	 */
	void (*cancel)(void *data,
		       struct wl_touch *wl_touch);
};

static inline int
wl_touch_add_listener(struct wl_touch *wl_touch,
		      const struct wl_touch_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_touch,
				     (void (**)(void)) listener, data);
}

static inline void
wl_touch_set_user_data(struct wl_touch *wl_touch, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_touch, user_data);
}

static inline void *
wl_touch_get_user_data(struct wl_touch *wl_touch)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_touch);
}

static inline void
wl_touch_destroy(struct wl_touch *wl_touch)
{
	wl_proxy_destroy((struct wl_proxy *) wl_touch);
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

/**
 * wl_output - compositor output region
 * @geometry: properties of the output
 * @mode: advertise available modes for the output
 *
 * An output describes part of the compositor geometry. The compositor
 * work in the 'compositor coordinate system' and an output corresponds to
 * rectangular area in that space that is actually visible. This typically
 * corresponds to a monitor that displays part of the compositor space.
 * This object is published as global during start up, or when a screen is
 * hot plugged.
 */
struct wl_output_listener {
	/**
	 * geometry - properties of the output
	 * @x: x position within the global compositor space
	 * @y: y position within the global compositor space
	 * @physical_width: width in millimeters of the output
	 * @physical_height: height in millimeters of the output
	 * @subpixel: subpixel orientation of the output
	 * @make: textual description of the manufacturer
	 * @model: textual description of the model
	 * @transform: transform that maps framebuffer to output
	 *
	 * 
	 */
	void (*geometry)(void *data,
			 struct wl_output *wl_output,
			 int32_t x,
			 int32_t y,
			 int32_t physical_width,
			 int32_t physical_height,
			 int32_t subpixel,
			 const char *make,
			 const char *model,
			 int32_t transform);
	/**
	 * mode - advertise available modes for the output
	 * @flags: mask of wl_output_mode flags
	 * @width: width of the mode in pixels
	 * @height: height of the mode in pixels
	 * @refresh: vertical refresh rate in mHz
	 *
	 * The mode event describes an available mode for the output. The
	 * event is sent when binding to the output object and there will
	 * always be one mode, the current mode. The event is sent again if
	 * an output changes mode, for the mode that is now current. In
	 * other words, the current mode is always the last mode that was
	 * received with the current flag set.
	 */
	void (*mode)(void *data,
		     struct wl_output *wl_output,
		     uint32_t flags,
		     int32_t width,
		     int32_t height,
		     int32_t refresh);
};

static inline int
wl_output_add_listener(struct wl_output *wl_output,
		       const struct wl_output_listener *listener, void *data)
{
	return wl_proxy_add_listener((struct wl_proxy *) wl_output,
				     (void (**)(void)) listener, data);
}

static inline void
wl_output_set_user_data(struct wl_output *wl_output, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_output, user_data);
}

static inline void *
wl_output_get_user_data(struct wl_output *wl_output)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_output);
}

static inline void
wl_output_destroy(struct wl_output *wl_output)
{
	wl_proxy_destroy((struct wl_proxy *) wl_output);
}

#define WL_REGION_DESTROY	0
#define WL_REGION_ADD	1
#define WL_REGION_SUBTRACT	2

static inline void
wl_region_set_user_data(struct wl_region *wl_region, void *user_data)
{
	wl_proxy_set_user_data((struct wl_proxy *) wl_region, user_data);
}

static inline void *
wl_region_get_user_data(struct wl_region *wl_region)
{
	return wl_proxy_get_user_data((struct wl_proxy *) wl_region);
}

static inline void
wl_region_destroy(struct wl_region *wl_region)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_DESTROY);

	wl_proxy_destroy((struct wl_proxy *) wl_region);
}

static inline void
wl_region_add(struct wl_region *wl_region, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_ADD, x, y, width, height);
}

static inline void
wl_region_subtract(struct wl_region *wl_region, int32_t x, int32_t y, int32_t width, int32_t height)
{
	wl_proxy_marshal((struct wl_proxy *) wl_region,
			 WL_REGION_SUBTRACT, x, y, width, height);
}

#ifdef  __cplusplus
}
#endif

#endif
