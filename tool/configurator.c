#include <allegro5/allegro5.h>

#define NK_IMPLEMENTATION
#include <nuklear.h>

#define NK_ALLEGRO5_IMPLEMENTATION
#include <nuklear_allegro5.h>

#include <stdio.h>
#include <stdlib.h>

#include <demo/common/style_configurator.c>
#include <demo/common/overview.c>

#define WIDTH (1080)
#define HEIGHT (720)

typedef struct nk_color color_table_t[NK_COLOR_COUNT];

static void write_style(const char* path, color_table_t* table) {
	ALLEGRO_FILE* file = al_fopen(path, "w");

	size_t write_count = al_fwrite(file, table, sizeof(color_table_t));
	if(write_count < sizeof(color_table_t)) {
		puts("Failed to write style to file");
	}

	al_fclose(file);
}

static void read_style(const char* path, color_table_t* table) {
	ALLEGRO_FILE* file = al_fopen(path, "r");

	size_t read_count = al_fread(file, table, sizeof(color_table_t));
	if(read_count < sizeof(color_table_t)) {
		puts("Failed to read style from file");
	}

	al_fclose(file);
}

int main(int argc, char** argv) {
	static color_table_t color_table;

	if(argc != 2) {
		puts("usage: configurator <output>");
		return EXIT_FAILURE;
	}

	const char* output = argv[1];

	memcpy(color_table, nk_default_color_style, sizeof(color_table_t));

	read_style(output, &color_table);

	al_init();

	al_install_mouse();
	al_install_keyboard();

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));

	const char* font_path = "res/font/unitblock/unitblock.ttf";
	NkAllegro5Font* font = nk_allegro5_font_create_from_file(font_path, 18, 0);

	struct nk_context* context = nk_allegro5_init(
			font, display, WIDTH, HEIGHT);

	nk_style_from_table(context, color_table);

	ALLEGRO_EVENT event;

	bool exit = false;

	while(!exit) {
		al_wait_for_event(queue, &event);

		al_clear_to_color((ALLEGRO_COLOR) { 0.0f, 0.0f, 0.0f, 1.0f });

		nk_input_begin(context);
		do {
			nk_allegro5_handle_event(&event);

			if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				exit = true;
			}
		} while(al_get_next_event(queue, &event));
		nk_input_end(context);

		style_configurator(context, color_table);
		overview(context);

		struct nk_rect area = nk_rect(50, 50, 220, 220);
		enum nk_panel_flags flags =
				NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE;

		if(nk_begin(context, "save", area, flags)) {
			nk_layout_row_static(context, 30, 80, 1);
			if(nk_button_label(context, "Save")) {
				printf("Saving style to \"%s\"\n", output);
				write_style(output, &color_table);
			}
		}
		nk_end(context);

		nk_allegro5_render();
		al_flip_display();
	}

	return EXIT_SUCCESS;
}
