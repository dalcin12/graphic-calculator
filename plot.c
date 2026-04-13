#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define WIDTH 1280
#define HEIGHT 720
#define FPS 60

#define RADIUS 1

#define AXES_THICKNESS 3

#define CAMERA_VELOCITY 15
#define CONSTANT_E pow((1 + 1/1e7), 1e7)

#define STEP 0.005

double SIZE_CONSTANT = 10;

double function(double x) {
	double res = x;
	return res;
}

void draw_axes() {
	int number_dashes = 20;
	int k = 0;

	Color grid_color = {130, 130, 130, 100};

	for (int i = 0; i < number_dashes; i++) {
		
		// grid
		
		Vector2 start_grid = {(WIDTH / 2) + k + 1/STEP, -5*HEIGHT};
		Vector2 end_grid = {(WIDTH / 2) + k + 1/STEP, 5*HEIGHT};
		
		DrawLineEx(start_grid, end_grid, AXES_THICKNESS >= 2 ? AXES_THICKNESS - 1 : 1, grid_color); // principal right grid
		
		//start_grid = (Vector2) {(WIDTH / 2) + k/2 + (1/(2*STEP)), -5*HEIGHT};
		//end_grid = (Vector2 ){(WIDTH / 2) + k/2 + (1/(2*STEP)), 5*HEIGHT};
		
		//DrawLineEx(start_grid, end_grid, AXES_THICKNESS, LIGHTGRAY); // secondary right grid

		start_grid = (Vector2) {(WIDTH / 2) - k, -5*HEIGHT};
		end_grid = (Vector2) {(WIDTH / 2) - k, 5*HEIGHT};

		DrawLineEx(start_grid, end_grid, AXES_THICKNESS >= 2 ? AXES_THICKNESS - 1 : 1, grid_color); // principal left grid

		start_grid = (Vector2) {-5*WIDTH, (HEIGHT / 2) - k};
		end_grid = (Vector2) {5*WIDTH, (HEIGHT / 2) - k};

		DrawLineEx(start_grid, end_grid, AXES_THICKNESS >= 2 ? AXES_THICKNESS - 1 : 1, grid_color); // principal up grid

		start_grid = (Vector2) {-5*WIDTH, (HEIGHT / 2) + k};
		end_grid = (Vector2) {5*WIDTH, (HEIGHT / 2) + k};

		DrawLineEx(start_grid, end_grid, AXES_THICKNESS >= 2 ? AXES_THICKNESS - 1 : 1, grid_color); // principal down grid
		
		// dashes
		
		Vector2 start = {(WIDTH / 2) + k, (HEIGHT / 2) - 10};
		Vector2 end = {(WIDTH / 2) + k, (HEIGHT / 2) + 10};

		DrawLineEx(start, end, AXES_THICKNESS, WHITE); // right dashes
		

		start.x = (WIDTH / 2) - k;
		end.x = (WIDTH / 2) - k;

		DrawLineEx(start, end, AXES_THICKNESS, WHITE); // left dashes

		start = (Vector2) {(WIDTH / 2) + 10, (HEIGHT / 2) - k};
		end = (Vector2) {(WIDTH / 2) - 10, (HEIGHT / 2) - k};

		DrawLineEx(start, end, AXES_THICKNESS, WHITE); // up dashes

		start.y = (HEIGHT / 2) + k;
		end.y = (HEIGHT / 2) + k;

		DrawLineEx(start, end, AXES_THICKNESS, WHITE); // down dashes
		
		
		
		k += 1 / STEP;
	}

	Vector2 start_x = {-5 * WIDTH, HEIGHT / 2};
	Vector2 end_x = {5 * WIDTH, HEIGHT / 2};

	Vector2 start_y = {WIDTH / 2, -5*HEIGHT};
	Vector2 end_y = {WIDTH/2, 5*HEIGHT};
	
	DrawLineEx(start_x, end_x, AXES_THICKNESS, WHITE);
	DrawLineEx(start_y, end_y, AXES_THICKNESS, WHITE);
}

void populate(double start, double step, double end, double* image, int size) {
	for (int i = 0; i < size; i++) {
		
		if (start + (i * step) < end) {
			image[i] = function(start + (i * step));
		} else {
			break;
		}
	}

}

void plot_points(double* image, long size, Vector2* middle) {
	double k = -size/2;
	for (long i = 0; i < size; i++) {
		// DrawCircle(middle->x + k, (HEIGHT - image[i] * 30) - middle->y, RADIUS, RAYWHITE);
		DrawPixel(middle->x + k, (HEIGHT - image[i] * (1/STEP)) - middle->y, RAYWHITE);
		k += 1;
	}

}

void add_informations() {
	DrawText("Graphic Calculator v1.0", (WIDTH / 2) - 100, 20, 20, RAYWHITE);
	DrawText("Press R to reset the zoom and the camera position", 5, HEIGHT - 20, 20, RAYWHITE);
	DrawText("Use WASD or the arrow keys to move the camera", 5, HEIGHT - 40, 20, RAYWHITE);
}

Camera2D camera_stuff() {
	Camera2D camera = { 0 };
	camera.target = (Vector2) {WIDTH / 2, HEIGHT / 2};
	camera.offset = (Vector2) {WIDTH / 2, HEIGHT / 2};
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;
	return camera;
}

int main(void) {
	double start = -1e2;
	double step = STEP;
	double end = 1e2;
	int number_points = (end - start) / step;
	double image[number_points];
	
	Vector2 middle = {WIDTH / 2, HEIGHT / 2}; // middle of the screen

	populate(start, step, end, image, number_points);

	InitWindow(WIDTH, HEIGHT, "Graphic Calculator v1.0");

	SetTargetFPS(FPS);

	Camera2D camera = camera_stuff();

	while (!WindowShouldClose()) {
		camera.zoom = expf(logf(camera.zoom) + ((float)GetMouseWheelMove()*0.1f));
		
		if (camera.zoom > 4.0f) {
			camera.zoom = 4.0f;
		} else if (camera.zoom < 0.6f) {
			camera.zoom = 0.6f;
		}

		if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) 	camera.offset.x += CAMERA_VELOCITY;
		if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) 	camera.offset.x -= CAMERA_VELOCITY;
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) 		camera.offset.y += CAMERA_VELOCITY;
		if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) 	camera.offset.y -= CAMERA_VELOCITY;

		if (IsKeyPressed(KEY_R)) {
			camera.target = (Vector2) {WIDTH / 2, HEIGHT / 2};
			camera.offset = (Vector2) {WIDTH / 2, HEIGHT / 2};
			camera.zoom = 1.0f;
		}

		BeginDrawing();
			ClearBackground(BLACK);
			BeginMode2D(camera);
				draw_axes();
				plot_points(image, number_points, &middle);
			EndMode2D();
			
			add_informations();
		EndDrawing();
	}

	CloseWindow();

	return 0;

}
