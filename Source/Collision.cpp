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

bool ss::CollisionShape::is_colliding_with(CollisionShape cs, bool inverse_collision) {
	if (!inverse_collision) {
		return (position.x + size.x > cs.position.x and position.y + size.y > cs.position.y) or
			(position.x < cs.position.x + cs.size.x and position.y < cs.position.y + cs.size.y);
	}
	else {
		return (position.x < cs.position.x or position.y < cs.position.y) or
			(position.x + size.x > cs.position.x + cs.size.x or position.y + size.y > cs.position.y + cs.size.y);
	}
}

void ss::CollisionShape::push_out(CollisionShape& cs) {
	if (is_colliding_with(cs)) {
		Vector cs_center = cs.get_center();
		Vector center = get_center();
		if (cs_center.x <= center.x) {
			cs.position.x = position.x - cs.size.x;
		}
		else {
			cs.position.x = position.x + size.x;
		}

		if (cs_center.y <= center.y) {
			cs.position.y = position.y - cs.size.y;
		}
		else {
			cs.position.y = position.y + size.y;
		}
	}
}

void ss::CollisionShape::push_in(CollisionShape& cs) {
	if (is_colliding_with(cs, true)) {
		if (cs.position.x < position.x) {
			cs.position.x = position.x;
		}
		else if (cs.position.x + cs.size.x > position.x + size.x) {
			cs.position.x = position.x + size.x - cs.size.x;
		}

		if (cs.position.y < position.y) {
			cs.position.y = position.y;
		}
		else if (cs.position.y + cs.size.y > position.y + size.y) {
			cs.position.y = position.y + size.y - cs.size.y;
		}
	}
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
	SDL_SetRenderDrawColor(render, draw_color.r, draw_color.g, draw_color.b, draw_color.a);
	SDL_RenderDrawRect(render, &rect);
	SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
}

double ss::CollisionShape::get_area() {
	return size.x * size.y;
}

ss::Vector ss::CollisionShape::get_center() {
	return position + (size / 2);
}
