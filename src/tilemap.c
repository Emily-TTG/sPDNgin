#include <game/tilemap.h>
#include <game/detail.h>
#include <game/script.h>
#include <game/log.h>

#include <ctype.h>

// TODO: This is just using the easy-but-dumb solution of loading the tileset
//		 For every tilemap. In future should decouple these with a loaded
//		 Tileset registry with refcounts, hashing etc.

enum gm_result gm_tileset_script_table_handler(
		struct gm_script* script, const char* path, void* pass) {

	struct gm_tileset* out = pass;

	size_t in_length = (strrchr(path, '/') + 1) - path;

	char* image;
	size_t image_length;

	lua_getfield(script->state, -1, "image");
	{
		image_length = lua_strlen(script->state, -1);

		image = malloc(in_length + image_length + 1);
		strncpy(image, path, in_length);
		strcpy(image + in_length, lua_tostring(script->state, -1));
	}
	lua_pop(script->state, 1);

	out->atlas = al_load_bitmap(image);
	if(!out->atlas) {
		enum gm_result result = GM_LOG_RESULT_PATH(
				al_load_bitmap, image, GM_RESULT_ERROR);

		free(image);
		return result;
	}

	free(image);

	out->dimension = gm_script_table_get_int(script, -1, "tilewidth");

	return GM_RESULT_OK;
}

enum gm_result gm_tilemap_script_table_handler(
		struct gm_script* script, const char* path, void* pass) {

	enum gm_result result;

	struct gm_tilemap* out = pass;

	char* tileset;

	out->width = gm_script_table_get_int(script, -1, "width");

	lua_getfield(script->state, -1, "layers");
	{
		lua_rawgeti(script->state, -1, 1);
		{
			lua_getfield(script->state, -1, "data");
			{
				size_t length = lua_strlen(script->state, -1);
				const char* data = lua_tostring(script->state, -1);
				out->data = malloc(length * sizeof(int));

				for(size_t i = 0; i < length; ++i) {
					char c = data[i];
					int id;

					if(isupper(c)) {
						id = c - 'A';
					}
					else if(islower(c)) {
						id = (c - 'a') + 26;
					}
					else if(isdigit(c)) {
						id = (c - '0') + 52;
					}
					else if(c == '+') {
						id = 62;
					}
					else if(c == '/') {
						id = 63;
					}
					else {
						id = 0;
					}

					out->data[i] = id;
				}
			}
			lua_pop(script->state, 1);
		}
		lua_pop(script->state, 1);
	}
	lua_pop(script->state, 1);

	lua_getfield(script->state, -1, "tilesets");
	{
		lua_rawgeti(script->state, -1, 1);
		{
			lua_getfield(script->state, -1, "filename");
			{
				static const char relative[] = "../";
				static const char extension[] = "lua";

				const char* tileset_path = lua_tostring(script->state, -1);
				char* split = strrchr(tileset_path, '/') + 1;
				char* extension_split = strchr(split, '.') + 1;

				size_t in_path_length = (strrchr(path, '/') + 1) - path;
				size_t path_length = split - tileset_path;
				size_t file_length = extension_split - split;

				size_t total_length =
						in_path_length + path_length + (sizeof(relative) - 1)
						+ file_length + (sizeof(extension) - 1) + 1;

				tileset = malloc(total_length);
				tileset[0] = 0;

				// I know str[n]cat is suboptimal here as we have the lengths
				// But this makes the code substantially more readable without
				// Making any real meaningful impact on performance.
				strncat(tileset, path, in_path_length);
				strncat(tileset, tileset_path, path_length);
				strcat(tileset, relative);
				strncat(tileset, split, file_length);
				strcat(tileset, extension);
			}
			lua_pop(script->state, 1);
		}
		lua_pop(script->state, 1);
	}
	lua_pop(script->state, 1);

	result = gm_script_file_table(
			tileset, gm_tileset_script_table_handler, &out->tileset);

	if(result) {
		(void) GM_LOG_RESULT_PATH(
				gm_script_file_table(gm_tileset_script_table_handler), tileset,
				result);

		free(out->data);
		free(tileset);
		return result;
	}

	free(tileset);

	return GM_RESULT_OK;
}
