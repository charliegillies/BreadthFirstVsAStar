/*
 * Charlie Gillies
 * Year 2 data structures demo application
 * A Star .vs. Breadth First search
 */

#include "App.h"

#include <allegro5\allegro.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>

int main() {

	// attempt to initialize all of our addons
	// use assert to make sure that they have initialized
	assert(al_init());
	assert(al_init_font_addon());
	assert(al_init_ttf_addon());
	assert(al_init_image_addon());
	assert(al_init_primitives_addon());
	assert(al_install_keyboard());

	// application
	App app;

	// initialize allegro
	ALLEGRO_DISPLAY* display = al_create_display(1280, 720);
	ALLEGRO_TIMER* timer = al_create_timer(1.0f / 60);
	ALLEGRO_EVENT_QUEUE* evQueue = al_create_event_queue();

	// register events
	al_register_event_source(evQueue, al_get_display_event_source(display));
	al_register_event_source(evQueue, al_get_timer_event_source(timer));
	al_register_event_source(evQueue, al_get_keyboard_event_source());
	al_start_timer(timer);

	ALLEGRO_COLOR clearColor = al_map_rgb(0, 0, 0);
	al_clear_to_color(clearColor);

	bool tick = false;

	app.initialize();

	while (true) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(evQueue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			tick = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			app.on_key_up(ev.keyboard.keycode);
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// exit the while loop
			break;
		}

		if (tick && al_is_event_queue_empty(evQueue)) {
			// don't tick
			tick = false;

			// clear to color
			al_clear_to_color(clearColor);
			
			// tick
			app.update(1.0 / 60);
			app.render();
			
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
}