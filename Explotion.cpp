#include "Explotion.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"

Explotion::Explotion(SDL_Rect pos, SDL_Texture* gfx, ExplotionInstance instance) : graphics(gfx)
{
	explotionType = instance.type;
	rotation = instance.rotation;
	flipType = instance.flipType;

	position.x = pos.x;
	position.y = pos.y;

	//collider
	collider = App->collision->AddCollider({ position.x, position.y, 50, 50 });
	collider->type = CEXPLOTION;

	switch (explotionType) {
		case FOURWAY:
			for (int i = 0; i < 7; i++) {
				animation.frames.push_back({ 0 + (48 * i), 0, 48, 48 });
			}
		break;
		case TWOWAY:
			for (int i = 0; i < 7; i++) {
				animation.frames.push_back({ 0 + (48 * i), 48, 48, 43 });
			}
		break;
		case ENDING:
			for (int i = 0; i < 7; i++) {
				animation.frames.push_back({ 0 + (48 * i), 91, 48, 43 });
			}
		break;
	}
	animation.speed = 0.1f;
	animation.loop = false;
	current_animation = &animation;
	//LOG("EXPLOTION!");
	spriteDest = { position.x, position.y, 50, 50 };
}


Explotion::~Explotion()
{
}

void Explotion::Draw()
{
	if (destroyed == false)
		App->renderer->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()), &spriteDest, rotation, flipType);
	if (current_animation->Finished()) destroyed = true;
}

void Explotion::CleanUp()
{
	collider->to_delete = true;
}
