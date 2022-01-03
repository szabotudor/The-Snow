#include<Collision.h>

ss::CollisionShape::CollisionShape(Vector size, Vector position, SDL_Window* window) {
	CollisionShape::position = position;
	CollisionShape::size = size;
	if (window != nullptr) {
		CollisionShape::window = window;
		render = SDL_GetRenderer(window);
		draw_enabled = true;
	}
}

bool ss::CollisionShape::is_colliding_with(CollisionShape cs) {
	return (position.x + size.x > cs.position.x and position.y + size.y > cs.position.y) or
		(position.x < cs.position.x + cs.size.x and position.y < cs.position.y + cs.size.y);
}

void ss::CollisionShape::enable_draw(SDL_Window* window) {
	CollisionShape::window = window;
	render = SDL_GetRenderer(window);
	draw_enabled = true;
}

void ss::CollisionShape::draw() {
	rect.x = position.x;
	rect.y = position.y;
	rect.w = size.x;
	rect.h = size.y;
	SDL_RenderDrawRect(render, &rect);
}
