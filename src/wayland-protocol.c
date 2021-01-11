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

#include <stdlib.h>
#include <stdint.h>
#include "wayland-util.h"

extern const struct wl_interface wl_object_interface;
extern const struct wl_interface wl_callback_interface;
extern const struct wl_interface wl_object_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_region_interface;
extern const struct wl_interface wl_buffer_interface;
extern const struct wl_interface wl_shm_pool_interface;
extern const struct wl_interface wl_data_source_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_data_source_interface;
extern const struct wl_interface wl_data_offer_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_data_offer_interface;
extern const struct wl_interface wl_data_offer_interface;
extern const struct wl_interface wl_data_source_interface;
extern const struct wl_interface wl_data_device_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_shell_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_buffer_interface;
extern const struct wl_interface wl_callback_interface;
extern const struct wl_interface wl_region_interface;
extern const struct wl_interface wl_region_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_pointer_interface;
extern const struct wl_interface wl_keyboard_interface;
extern const struct wl_interface wl_touch_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface wl_surface_interface;

static const struct wl_interface *types[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&wl_callback_interface,
	NULL,
	NULL,
	NULL,
	&wl_surface_interface,
	&wl_region_interface,
	&wl_buffer_interface,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&wl_shm_pool_interface,
	NULL,
	NULL,
	&wl_data_source_interface,
	&wl_surface_interface,
	&wl_surface_interface,
	NULL,
	&wl_data_source_interface,
	NULL,
	&wl_data_offer_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	&wl_data_offer_interface,
	&wl_data_offer_interface,
	&wl_data_source_interface,
	&wl_data_device_interface,
	&wl_seat_interface,
	&wl_shell_surface_interface,
	&wl_surface_interface,
	&wl_seat_interface,
	NULL,
	&wl_seat_interface,
	NULL,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	&wl_output_interface,
	&wl_seat_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&wl_output_interface,
	&wl_buffer_interface,
	NULL,
	NULL,
	&wl_callback_interface,
	&wl_region_interface,
	&wl_region_interface,
	&wl_output_interface,
	&wl_output_interface,
	&wl_pointer_interface,
	&wl_keyboard_interface,
	&wl_touch_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
	&wl_surface_interface,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	&wl_surface_interface,
	NULL,
	NULL,
	NULL,
};

static const struct wl_message wl_display_requests[] = {
	{ "bind", "usun", types + 8 },
	{ "sync", "n", types + 12 },
};

static const struct wl_message wl_display_events[] = {
	{ "error", "ous", types + 13 },
	{ "global", "usu", types + 0 },
	{ "global_remove", "u", types + 0 },
	{ "delete_id", "u", types + 0 },
};

WL_EXPORT const struct wl_interface wl_display_interface = {
	"wl_display", 1,
	ARRAY_LENGTH(wl_display_requests), wl_display_requests,
	ARRAY_LENGTH(wl_display_events), wl_display_events,
};

static const struct wl_message wl_callback_events[] = {
	{ "done", "u", types + 0 },
};

WL_EXPORT const struct wl_interface wl_callback_interface = {
	"wl_callback", 1,
	0, NULL,
	ARRAY_LENGTH(wl_callback_events), wl_callback_events,
};

static const struct wl_message wl_compositor_requests[] = {
	{ "create_surface", "n", types + 16 },
	{ "create_region", "n", types + 17 },
};

WL_EXPORT const struct wl_interface wl_compositor_interface = {
	"wl_compositor", 1,
	ARRAY_LENGTH(wl_compositor_requests), wl_compositor_requests,
	0, NULL,
};

static const struct wl_message wl_shm_pool_requests[] = {
	{ "create_buffer", "niiiiu", types + 18 },
	{ "destroy", "", types + 0 },
	{ "resize", "i", types + 0 },
};

WL_EXPORT const struct wl_interface wl_shm_pool_interface = {
	"wl_shm_pool", 1,
	ARRAY_LENGTH(wl_shm_pool_requests), wl_shm_pool_requests,
	0, NULL,
};

static const struct wl_message wl_shm_requests[] = {
	{ "create_pool", "nhi", types + 24 },
};

static const struct wl_message wl_shm_events[] = {
	{ "format", "u", types + 0 },
};

WL_EXPORT const struct wl_interface wl_shm_interface = {
	"wl_shm", 1,
	ARRAY_LENGTH(wl_shm_requests), wl_shm_requests,
	ARRAY_LENGTH(wl_shm_events), wl_shm_events,
};

static const struct wl_message wl_buffer_requests[] = {
	{ "destroy", "", types + 0 },
};

static const struct wl_message wl_buffer_events[] = {
	{ "release", "", types + 0 },
};

WL_EXPORT const struct wl_interface wl_buffer_interface = {
	"wl_buffer", 1,
	ARRAY_LENGTH(wl_buffer_requests), wl_buffer_requests,
	ARRAY_LENGTH(wl_buffer_events), wl_buffer_events,
};

static const struct wl_message wl_data_offer_requests[] = {
	{ "accept", "u?s", types + 0 },
	{ "receive", "sh", types + 0 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message wl_data_offer_events[] = {
	{ "offer", "s", types + 0 },
};

WL_EXPORT const struct wl_interface wl_data_offer_interface = {
	"wl_data_offer", 1,
	ARRAY_LENGTH(wl_data_offer_requests), wl_data_offer_requests,
	ARRAY_LENGTH(wl_data_offer_events), wl_data_offer_events,
};

static const struct wl_message wl_data_source_requests[] = {
	{ "offer", "s", types + 0 },
	{ "destroy", "", types + 0 },
};

static const struct wl_message wl_data_source_events[] = {
	{ "target", "?s", types + 0 },
	{ "send", "sh", types + 0 },
	{ "cancelled", "", types + 0 },
};

WL_EXPORT const struct wl_interface wl_data_source_interface = {
	"wl_data_source", 1,
	ARRAY_LENGTH(wl_data_source_requests), wl_data_source_requests,
	ARRAY_LENGTH(wl_data_source_events), wl_data_source_events,
};

static const struct wl_message wl_data_device_requests[] = {
	{ "start_drag", "?oo?ou", types + 27 },
	{ "set_selection", "?ou", types + 31 },
};

static const struct wl_message wl_data_device_events[] = {
	{ "data_offer", "n", types + 33 },
	{ "enter", "uoff?o", types + 34 },
	{ "leave", "", types + 0 },
	{ "motion", "uff", types + 0 },
	{ "drop", "", types + 0 },
	{ "selection", "?o", types + 39 },
};

WL_EXPORT const struct wl_interface wl_data_device_interface = {
	"wl_data_device", 1,
	ARRAY_LENGTH(wl_data_device_requests), wl_data_device_requests,
	ARRAY_LENGTH(wl_data_device_events), wl_data_device_events,
};

static const struct wl_message wl_data_device_manager_requests[] = {
	{ "create_data_source", "n", types + 40 },
	{ "get_data_device", "no", types + 41 },
};

WL_EXPORT const struct wl_interface wl_data_device_manager_interface = {
	"wl_data_device_manager", 1,
	ARRAY_LENGTH(wl_data_device_manager_requests), wl_data_device_manager_requests,
	0, NULL,
};

static const struct wl_message wl_shell_requests[] = {
	{ "get_shell_surface", "no", types + 43 },
};

WL_EXPORT const struct wl_interface wl_shell_interface = {
	"wl_shell", 1,
	ARRAY_LENGTH(wl_shell_requests), wl_shell_requests,
	0, NULL,
};

static const struct wl_message wl_shell_surface_requests[] = {
	{ "pong", "u", types + 0 },
	{ "move", "ou", types + 45 },
	{ "resize", "ouu", types + 47 },
	{ "set_toplevel", "", types + 0 },
	{ "set_transient", "oiiu", types + 50 },
	{ "set_fullscreen", "uu?o", types + 54 },
	{ "set_popup", "ouoiiu", types + 57 },
	{ "set_maximized", "?o", types + 63 },
	{ "set_title", "s", types + 0 },
	{ "set_class", "s", types + 0 },
};

static const struct wl_message wl_shell_surface_events[] = {
	{ "ping", "u", types + 0 },
	{ "configure", "uii", types + 0 },
	{ "popup_done", "", types + 0 },
};

WL_EXPORT const struct wl_interface wl_shell_surface_interface = {
	"wl_shell_surface", 1,
	ARRAY_LENGTH(wl_shell_surface_requests), wl_shell_surface_requests,
	ARRAY_LENGTH(wl_shell_surface_events), wl_shell_surface_events,
};

static const struct wl_message wl_surface_requests[] = {
	{ "destroy", "", types + 0 },
	{ "attach", "?oii", types + 64 },
	{ "damage", "iiii", types + 0 },
	{ "frame", "n", types + 67 },
	{ "set_opaque_region", "o", types + 68 },
	{ "set_input_region", "?o", types + 69 },
};

static const struct wl_message wl_surface_events[] = {
	{ "enter", "o", types + 70 },
	{ "leave", "o", types + 71 },
};

WL_EXPORT const struct wl_interface wl_surface_interface = {
	"wl_surface", 1,
	ARRAY_LENGTH(wl_surface_requests), wl_surface_requests,
	ARRAY_LENGTH(wl_surface_events), wl_surface_events,
};

static const struct wl_message wl_seat_requests[] = {
	{ "get_pointer", "n", types + 72 },
	{ "get_keyboard", "n", types + 73 },
	{ "get_touch", "n", types + 74 },
};

static const struct wl_message wl_seat_events[] = {
	{ "capabilities", "u", types + 0 },
};

WL_EXPORT const struct wl_interface wl_seat_interface = {
	"wl_seat", 1,
	ARRAY_LENGTH(wl_seat_requests), wl_seat_requests,
	ARRAY_LENGTH(wl_seat_events), wl_seat_events,
};

static const struct wl_message wl_pointer_requests[] = {
	{ "set_cursor", "u?oii", types + 75 },
};

static const struct wl_message wl_pointer_events[] = {
	{ "enter", "uoff", types + 79 },
	{ "leave", "uo", types + 83 },
	{ "motion", "uff", types + 0 },
	{ "button", "uuuu", types + 0 },
	{ "axis", "uuf", types + 0 },
};

WL_EXPORT const struct wl_interface wl_pointer_interface = {
	"wl_pointer", 1,
	ARRAY_LENGTH(wl_pointer_requests), wl_pointer_requests,
	ARRAY_LENGTH(wl_pointer_events), wl_pointer_events,
};

static const struct wl_message wl_keyboard_events[] = {
	{ "keymap", "uhu", types + 0 },
	{ "enter", "uoa", types + 85 },
	{ "leave", "uo", types + 88 },
	{ "key", "uuuu", types + 0 },
	{ "modifiers", "uuuuu", types + 0 },
};

WL_EXPORT const struct wl_interface wl_keyboard_interface = {
	"wl_keyboard", 1,
	0, NULL,
	ARRAY_LENGTH(wl_keyboard_events), wl_keyboard_events,
};

static const struct wl_message wl_touch_events[] = {
	{ "down", "uuoiff", types + 90 },
	{ "up", "uui", types + 0 },
	{ "motion", "uiff", types + 0 },
	{ "frame", "", types + 0 },
	{ "cancel", "", types + 0 },
};

WL_EXPORT const struct wl_interface wl_touch_interface = {
	"wl_touch", 1,
	0, NULL,
	ARRAY_LENGTH(wl_touch_events), wl_touch_events,
};

static const struct wl_message wl_output_events[] = {
	{ "geometry", "iiiiissi", types + 0 },
	{ "mode", "uiii", types + 0 },
};

WL_EXPORT const struct wl_interface wl_output_interface = {
	"wl_output", 1,
	0, NULL,
	ARRAY_LENGTH(wl_output_events), wl_output_events,
};

static const struct wl_message wl_region_requests[] = {
	{ "destroy", "", types + 0 },
	{ "add", "iiii", types + 0 },
	{ "subtract", "iiii", types + 0 },
};

WL_EXPORT const struct wl_interface wl_region_interface = {
	"wl_region", 1,
	ARRAY_LENGTH(wl_region_requests), wl_region_requests,
	0, NULL,
};

